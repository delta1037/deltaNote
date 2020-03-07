//
// Created by geniusrabbit on 18-2-27.
//

#include "../include/socket.h"
#include "../include/Log.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

int SocketServer::serverSocketFd;
SocketServer::SocketServer(){
    clientSocketFd = 0;
}

SocketServer::~SocketServer(){
    if(clientSocketFd != 0) {
        LOG_INFO("close client socket")
        close(clientSocketFd);
    }
    socketState = SocketStopped;
}

bool SocketServer::initSocketServer(const char *serverIP, int serverPort) {
    LOG_INFO("start socket server")

    int flag;
    serverSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK(serverSocketFd, SOCKET_ERROR, { LOG_ERROR("Server socket init failed") return false; })

    struct sockaddr_in serverSocketAddr{};
    memset(&serverSocketAddr,0, sizeof(serverSocketAddr));

    serverSocketAddr.sin_family = AF_INET;
    serverSocketAddr.sin_port = htons(static_cast<uint16_t>(serverPort));
    serverSocketAddr.sin_addr.s_addr = inet_addr(serverIP);
    flag = bind(serverSocketFd, (struct sockaddr*)&serverSocketAddr, sizeof(serverSocketAddr));
    CHECK(flag, SOCKET_ERROR, {LOG_ERROR("Server bind error") return false;})

    flag = listen(serverSocketFd, 20);
    CHECK(flag, SOCKET_ERROR, {LOG_ERROR("Start(Listen) error") return false;})

    return true;
}

bool SocketServer::acceptConn(){
    struct sockaddr_in clientAddr{};
    socklen_t clientAddrSize = sizeof(clientAddr);
    clientSocketFd = accept(serverSocketFd, (struct sockaddr*)&clientAddr, &clientAddrSize);
    return clientSocketFd > 0;
}

bool SocketServer::sendMsg(void *buf, size_t size) {
    ssize_t sendSize = send(clientSocketFd, buf, size, 0);
    CHECK(sendSize,0,{LOG_ERROR("Send Size is NULL") return false;})
    return true;
}

bool SocketServer::recvMsg(void *buf, size_t size) {
    ssize_t recvSize=read(clientSocketFd, buf, size);
    CHECK(recvSize,0,{LOG_ERROR("Receive Size is NULL") return false;})
    return true;
}

SocketState SocketServer::closeServer() {
    LOG_INFO("close server Socket")
    close(serverSocketFd);
    return socketState;
}


ServerConnectControl::ServerConnectControl() {
    socketServer= new SocketServer;
}

ServerConnectControl::~ServerConnectControl() {
    delete socketServer;
}

bool ServerConnectControl::initServerConnect(const char *serverIP, int serverPort) {
    return SocketServer::initSocketServer(serverIP,serverPort);
}

bool ServerConnectControl::acceptNewConnect() {
    return socketServer->acceptConn();
}

void ServerConnectControl::processingClientRequest() {
    socketServer->recvMsg(&socketMsgPack, sizeof(SocketMsgPack));

    state = OperateNotDefine;
    dataControl = new ServerDataControl(socketMsgPack);

    switch (socketMsgPack.msgOp){
        case CreateUser:
            createNewUser();
            break;
        case Login:
            loginToServer();
            break;
        case Pull:
            loadFromServer();
            break;
        case Push:
            uploadToServer();
            break;
        default:
            struct SocketMsgPack returnMsg;
            returnMsg.msgState = MSG_OP_NULL;
            returnStateMsg(returnMsg);
    }
    delete dataControl;
}

bool ServerConnectControl::returnStateMsg(struct SocketMsgPack &returnMsg) {
    return socketServer->sendMsg(&returnMsg, sizeof(SocketMsgPack));
}

void ServerConnectControl::loginToServer() {
    if(dataControl->userDoLogin()){
        LOG_INFO("user:%s passwd:****** login success", socketMsgPack.userName)
    }else{
        LOG_ERROR("user:%s passwd:****** login error", socketMsgPack.userName)
    }

    struct SocketMsgPack returnMsg;
    returnMsg.msgState = dataControl->getSqliteState();
    returnStateMsg(returnMsg);
}

void ServerConnectControl::createNewUser() {
    if(dataControl->addNewUser()){
        LOG_INFO("user:%s passwd:****** create success", socketMsgPack.userName)
    }else{
        LOG_ERROR("user:%s passwd:****** create error", socketMsgPack.userName)
    }

    struct SocketMsgPack returnMsg;
    returnMsg.msgState = dataControl->getSqliteState();
    returnStateMsg(returnMsg);
}

void ServerConnectControl::uploadToServer() {
    if(!dataControl->userDoLogin()){
        LOG_ERROR("user:%s passwd:****** login error", socketMsgPack.userName)
        // send ack
        struct SocketMsgPack returnMsg;
        returnMsg.msgState = LoginPasswdError;
        returnStateMsg(returnMsg);
        return;
    }
    vector<SocketMsgOpPack> socketMsgOpList;
    struct SocketMsgPack recv_buffer = socketMsgPack;
    do {
        if(recv_buffer.msgOp != Push){
            LOG_ERROR("user:%s upload data format error", socketMsgPack.userName)
            break;
        }

        for (int i = 0; i < recv_buffer.msgSize; ++i) {
            socketMsgOpList.push_back(recv_buffer.msgQueue[i]);
        }

        if (recv_buffer.msgSeg == MSG_HALF) {
            if(!socketServer->recvMsg(&recv_buffer, sizeof(SocketMsgPack))){
                LOG_ERROR("user:%s wrong size recv", socketMsgPack.userName)
                break;
            }
        } else {
            LOG_INFO("user:%s data upload success", socketMsgPack.userName)
            break;
        }
    } while (true);

    if(!socketMsgOpList.empty()){
        dataControl->saveLocalOpList(socketMsgOpList);
    }

    struct SocketMsgPack returnMsg;
    returnMsg.msgState = PushSuccess;
    returnMsg.msgOp = ACK;
    returnMsg.msgSize = socketMsgOpList.size();
    returnStateMsg(returnMsg);
}

void ServerConnectControl::loadFromServer() {
    if(!dataControl->userDoLogin()){
        LOG_ERROR("user:%s passwd:****** login error", socketMsgPack.userName)
        struct SocketMsgPack returnMsg;
        returnMsg.msgState = PullError;
        returnStateMsg(returnMsg);
        return;
    }
    vector<SocketMsgOpPack> socketMsgList;
    vector<SocketMsgOpPack> socketPack;
    dataControl->loadLocalList(socketMsgList);

    char tempArr[] = "null";
    for(const auto& it : socketMsgList){
        if(socketPack.size() == 5){
            // send to server
            SocketMsgPack msg_buffer(Pull, MSG_HALF, PullSuccess, socketPack.size(), tempArr, tempArr, socketPack);
            socketServer->sendMsg(&msg_buffer, sizeof (SocketMsgPack));
            socketPack.clear();
        }else{
            socketPack.push_back(it);
        }
    }
    if(!socketPack.empty()){
        SocketMsgPack msg_buffer(Pull, MSG_FULL, PullSuccess, socketPack.size(), tempArr, tempArr, socketPack);
        socketServer->sendMsg(&msg_buffer, sizeof (SocketMsgPack));
        socketPack.clear();
    }

    struct SocketMsgPack returnMsg;
    returnMsg.msgState = PullSuccess;
    returnMsg.msgOp = ACK;
    returnMsg.msgSize = socketMsgList.size();
    returnStateMsg(returnMsg);
}


