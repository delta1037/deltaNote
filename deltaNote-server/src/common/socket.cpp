#include "../../include/common/socket.h"

int Socket::serverSocketFd;
Socket::Socket(){
    clientSocketFd = 0;
}

Socket::~Socket(){
    if(clientSocketFd > 0) {
        LogCtrl::debug("close client socket");
        close(clientSocketFd);
    }
}

bool Socket::initSocketServer(const char initServerIP[], int initServerPort) {
    LogCtrl::info("start socket server");

    int flag;
    serverSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK(serverSocketFd, SOCKET_ERROR, { LogCtrl::error("Server socket init failed"); return false; })

    struct sockaddr_in serverSocketAddr{};
    memset(&serverSocketAddr,0, sizeof(serverSocketAddr));

    serverSocketAddr.sin_family = AF_INET;
    serverSocketAddr.sin_port = htons(static_cast<uint16_t>(initServerPort));
    serverSocketAddr.sin_addr.s_addr = inet_addr(initServerIP);
    flag = bind(serverSocketFd, (struct sockaddr*)&serverSocketAddr, sizeof(serverSocketAddr));
    CHECK(flag, SOCKET_ERROR, {LogCtrl::error("Server bind error"); return false;})

    flag = listen(serverSocketFd, 20);
    CHECK(flag, SOCKET_ERROR, {LogCtrl::error("Start(Listen) error"); return false;})

    return true;
}

void Socket::getClientIPAddr(char r_clientIPAddr[]){
    strcpy(r_clientIPAddr, this->clientIPAddr);
}

bool Socket::acceptConn(){
    struct sockaddr_in clientAddr{};
    socklen_t clientAddrSize = sizeof(clientAddr);
    clientSocketFd = accept(serverSocketFd, (struct sockaddr*)&clientAddr, &clientAddrSize);
    sprintf(clientIPAddr, "%s", inet_ntoa(clientAddr.sin_addr));
    return clientSocketFd > 0;
}

bool Socket::sendMsg(void *buf, size_t size) {
    if(clientSocketFd == -1){
        // 对端关闭连接
        return false;
    }
    ssize_t sendSize = send(clientSocketFd, buf, size, 0);
    if(sendSize != size){
        LogCtrl::error("Send Size is error : send:%d != needSend:%d", (int)sendSize, (int)size);
        return false;
    }
    return true;
}

bool Socket::recvMsg(void *buf, size_t size) {
    ssize_t recvSize=read(clientSocketFd, buf, size);
    if(recvSize != size){
        LogCtrl::error("Receive Size is error: recv:%d != needRecv:%d", (int)recvSize, (int)size);
        if(recvSize <= 0){
            //  当接收到的大小小于等于0时判断为对端关闭连接
            LogCtrl::debug("close client socket");
            close(clientSocketFd);
            clientSocketFd = -1;
        }
        return false;
    }
    return true;
}