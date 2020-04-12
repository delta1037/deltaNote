//
// Created by geniusrabbit on 18-2-27.
//

#include "common/socket.h"

SocketClient::SocketClient(){
#ifdef WINDOW_CLIENT
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
#ifdef LINUX_CLIENT
    clientSocketFd = -1;
#endif
    ret = 0;
}

SocketClient::~SocketClient(){
    if(clientSocketFd > 0){
        close(clientSocketFd);
        clientSocketFd = -1;
    }
#ifdef WINDOW_CLIENT
    WSACleanup();
#endif
}

bool SocketClient::initSocket(char *initServerIP, int initServerPort){
    clientSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocketFd == INVALID_SOCKET){
        LogCtrl::error("Client socket init failed");
        return false;
    }

    SOCKADDR_IN serverSocketAddr;
    memset(&serverSocketAddr,0, sizeof(serverSocketAddr));
    serverSocketAddr.sin_family = AF_INET;
    serverSocketAddr.sin_port = htons(static_cast<uint16_t>(initServerPort));
    serverSocketAddr.sin_addr.s_addr = inet_addr(initServerIP);

    ret = connect(clientSocketFd, (SOCKADDR*)&serverSocketAddr, sizeof(serverSocketAddr));
    if(ret == SOCKET_ERROR){
        LogCtrl::error("connect server failed");
        return false;
    }
    return true;
}

bool SocketClient::sendMsg(void *buf, size_t size) {
    long sendSize = send(clientSocketFd, (char *)buf, size, 0);
    if(sendSize != long(size)){
        LogCtrl::error("Send Size is Error:%d != %d", sendSize, size);
        return false;
    }
    return true;
}

bool SocketClient::recvMsg(void *buf, size_t size) {
    long recvSize = recv(clientSocketFd, (char *)buf, size, 0);
    if(recvSize != long(size)){
        LogCtrl::error("recvMsg receive Size is Error: %d != %d", recvSize, size);
        return false;
    }
    return true;
}
