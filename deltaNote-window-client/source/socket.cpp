//
// Created by geniusrabbit on 18-2-27.
//

#include "socket.h"
#include "log.h"
#include "notify.h"

SocketClient::SocketClient(){
    serverPort = 0;
    socketState = SocketStopped;
    clientSocketFd = 0;
    ret = 0;
}

SocketClient::SocketClient(char *_serverIP, int _serverPort){
    strcpy(serverIP, _serverIP);
    serverPort = _serverPort;
    clientSocketFd = 0;
    socketState = SocketRunning;
    WSAStartup(MAKEWORD(2, 2), &wsa);
    //LOG_INFO("start socket client")
}

SocketClient::~SocketClient(){
    LOG_INFO("close client socket")
    close(clientSocketFd);
    socketState = SocketStopped;
    //close ws2_32.dll
    WSACleanup();
}

bool SocketClient::initSocket(){
    clientSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocketFd == INVALID_SOCKET){
        LOG_ERROR("Client socket init failed")
        return false;
    }

    SOCKADDR_IN serverSocketAddr;
    memset(&serverSocketAddr,0, sizeof(serverSocketAddr));
    serverSocketAddr.sin_family = AF_INET;
    serverSocketAddr.sin_port = htons(static_cast<uint16_t>(serverPort));
    serverSocketAddr.sin_addr.s_addr = inet_addr(serverIP);

    ret = connect(clientSocketFd, (SOCKADDR*)&serverSocketAddr, sizeof(serverSocketAddr));
    if(ret == SOCKET_ERROR){
        LOG_ERROR("connect server failed")
        return false;
    }
    LOG_INFO("start socket success")
    return true;
}

bool SocketClient::sendMsg(void *buf, int size) {
    ssize_t sendSize = send(clientSocketFd, (char *)buf, size, 0);
    if(sendSize != size){
        LOG_ERROR("Send Size is Error:%d != %d", sendSize, size)
        return false;
    }
    return true;
}

bool SocketClient::recvMsg(void *buf, int size) {
    ssize_t recvSize = recv(clientSocketFd, (char*)buf, size, 0);
    if(recvSize != size){
        LOG_ERROR("recvMsg receive Size is Error: %d != %d", recvSize, size)
        return false;
    }
    return true;
}

SocketState SocketClient::closeClient() {
    if(clientSocketFd > 0){
        LOG_INFO("close client socket")
        close(clientSocketFd);
        clientSocketFd = -1;
    }
    return socketState;
}

SocketState SocketClient::getSocketOpState() {
    return socketState;
}

ServerConnectControl::ServerConnectControl(){
    init = false;
}
ServerConnectControl::~ServerConnectControl(){
    socketClient->closeClient();
    delete socketClient;
}

MSG_State ServerConnectControl::getState(){
    return state;
}

bool ServerConnectControl::initConnect(){
    // connect server
    //LOG_INFO("server:%s, port:%d", g_server, g_port)
    socketClient = new SocketClient(g_server, g_port);
    if(!socketClient->initSocket()){
        LOG_ERROR("init Socket Error")
        return false;
    }
    init = true;
    return true;
}

bool ServerConnectControl::isIPAddr(const char* pStr){
    bool bRet = true;
    if (nullptr == pStr) return -1;
        const char* p = pStr;
    for (; *p != '\0'; p++)
    {
        if ((isalpha(*p)) && (*p != '.'))
        {
            bRet = false;
            break;
        }
    }
    return bRet;
}

int ServerConnectControl::getIPbyDomain(const char* domain, char* ip){
    struct hostent *answer;
    answer = gethostbyname(domain);
    if (nullptr == answer)
    {
        //herror("gethostbyname");//the error function of itself
        return -1;
    }
    if (answer->h_addr_list[0]){
        sockaddr_in in;
        memcpy(&in.sin_addr, (answer->h_addr_list)[0], sizeof ((answer->h_addr_list)[0]));
        strcpy(ip, inet_ntoa(in.sin_addr));
    }else
        return -1;
    return 0;
}
void ServerConnectControl::parserServerPort(char *serverPort){
    bool isServer = true;
    char server[G_ARR_SIZE_SERVER] = {0};
    int port = 0;
    for(int i = 0; serverPort[i] != '\0'; ++i){
        if(serverPort[i] == ':'){
            isServer = false;
            server[i] = '\0';
            continue;
        }

        if(isServer){
            server[i] = serverPort[i];
        } else {
            port = port * 10 + serverPort[i] - '0';
        }
    }

    //check
    if(server[0] == '\0' || port == 0){
        MessagesBox::warn(nullptr, SOCKET_SERVER_FORMAT_ERROR);
        LOG_ERROR("server and port parser error!")
        return;
    }

    if(!isIPAddr(server)){
        char serverIP[G_ARR_SIZE_SERVER] = {0};
        int ret = getIPbyDomain(server, serverIP);

        if(ret != 0){
            MessagesBox::warn(nullptr, SOCKET_SERVER_ADDR_ERROR);
            LOG_ERROR("host name error");
            return;
        }
    }

    // store log info
    strcpy(g_server, server);
    g_port = port;
}

void ServerConnectControl::communicateWithServer(MSG_OP op){
    if((!init) && (!initConnect())){
        isLogin = false;
        return;
    }

    LOG_INFO("user:%s, passwd:%s", g_username, g_passwd);
    if(strlen(g_username) == 0 || strlen(g_passwd) == 0){
        isLogin = false;
        return;
    }

    SocketMsgPack send_buffer(op, MSG_FULL, '\0', 0, g_username, g_passwd);
    socketClient->sendMsg(&send_buffer, sizeof (SocketMsgPack));
    LOG_INFO("send_buffer:user:%s, passwd:%s", send_buffer.userName, send_buffer.passwd);

    SocketMsgPack recv_buffer;
    socketClient->recvMsg(&recv_buffer, sizeof(SocketMsgPack));

    if (recv_buffer.msgState == LoginSuccess || recv_buffer.msgState == CreateUserSuccess) {
        isLogin = true;
    }else{
        isLogin = false;
    }
    state = (MSG_State)recv_buffer.msgState;
}

void ServerConnectControl::createNewUser(){
    LOG_INFO("create new user")
    communicateWithServer(CreateUser);
}

void ServerConnectControl::loginToServer(){
    LOG_INFO("login to server")
    communicateWithServer(Login);
}
// TODO： set bool
bool ServerConnectControl::loadFromServer(std::vector<MsgOpPack> &msgQueue){
    if((!init) && (!initConnect())){
        isLogin = false;
        return false;
    }

    SocketMsgPack send_buffer(Pull, MSG_FULL, '\0', 0, g_username, g_passwd);
    socketClient->sendMsg(&send_buffer, sizeof (SocketMsgPack));
    //LOG_INFO("sent pull message")
    SocketMsgPack recv_buffer;
    socketClient->recvMsg(&recv_buffer, sizeof (SocketMsgPack));
    //LOG_INFO("recv first pack")
    if(recv_buffer.msgOp != ACK){
        do {
            if(recv_buffer.msgOp != Pull && recv_buffer.msgState != PullSuccess){
                LOG_ERROR("user:%s pull data error", send_buffer.userName)
                msgQueue.clear();
                return false;
            }
            for (int i = 0; i < recv_buffer.msgSize; ++i) {
                msgQueue.push_back(recv_buffer.msgQueue[i]);
            }

            if (recv_buffer.msgSeg == MSG_HALF) {
                if(!socketClient->recvMsg(&recv_buffer, sizeof(SocketMsgPack))){
                    msgQueue.clear();
                    return false;
                }
            } else {
                break;
            }
        } while (true);

        // recv ack
        socketClient->recvMsg(&recv_buffer, sizeof(SocketMsgPack));
    }

    // ack & size right & PullSuccess
    if(recv_buffer.msgState == PullSuccess
            && recv_buffer.msgOp == ACK
            && recv_buffer.msgSize == (int)msgQueue.size()){
        return true;
    }else{
        if(recv_buffer.msgState != PullSuccess){
            LOG_ERROR("load from server fail:%s", recv_buffer.msgState)
        }else if(recv_buffer.msgOp != ACK){
            LOG_ERROR("wrong pack recv:%s", recv_buffer.msgOp)
        }else if(recv_buffer.msgSize == (int)msgQueue.size()){
            LOG_ERROR("wrong size get:%d != %d", recv_buffer.msgSize, msgQueue.size())
        }else{
            LOG_ERROR("load from server undefine error")
        }
        msgQueue.clear();
        return false;
    }
}

bool ServerConnectControl::uploadToServer(std::vector<MsgOpPack> &msgQueue){
    if((!init) && (!initConnect())){
        isLogin = false;
        return false;
    }

    std::vector<MsgOpPack> socketPack;
    for(auto it : msgQueue){
        if(socketPack.size() == 5){
            // send to server
            SocketMsgPack msg_buffer(Push, MSG_HALF, '\0', socketPack.size(), g_username, g_passwd, socketPack);
            socketClient->sendMsg(&msg_buffer, sizeof (SocketMsgPack));
            socketPack.clear();
        }
        socketPack.push_back(it);
    }
    if(socketPack.size() != 0){
        SocketMsgPack msg_buffer(Push, MSG_FULL, '\0', socketPack.size(), g_username, g_passwd, socketPack);
        socketClient->sendMsg(&msg_buffer, sizeof (SocketMsgPack));
        socketPack.clear();
    }

    LOG_INFO("send size:%d", msgQueue.size())

    // recv ack
    SocketMsgPack recv_buffer;
    socketClient->recvMsg(&recv_buffer, sizeof(SocketMsgPack));
    // get LoginPasswdError, msgSize error, msgSize success
    if(recv_buffer.msgState == PushSuccess
            && recv_buffer.msgOp == ACK
            && recv_buffer.msgSize == (int)msgQueue.size()){
        return true;
    }else{
        if(recv_buffer.msgState != PushSuccess){
            LOG_ERROR("push to  server fail:%s", recv_buffer.msgState)
        }else if(recv_buffer.msgOp != ACK){
            LOG_ERROR("wrong pack recv:%s", recv_buffer.msgOp)
        }else if(recv_buffer.msgSize != (int)msgQueue.size()){
            LOG_ERROR("wrong size get:%d != %d", recv_buffer.msgSize, msgQueue.size())
        }else if(recv_buffer.msgState == LoginPasswdError){
            isLogin = false;
            LOG_ERROR("password is wrong or user not exits")
        }else{
            LOG_ERROR("push to server undefine error")
        }
        return false;
    }
}
