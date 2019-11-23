//
// Created by geniusrabbit on 18-2-27.
//

#include "../include/socket.h"
#include "../include/Log.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

namespace deltaNote {
int SocketServer::serverSocketFd;

SocketServer::SocketServer(SocketServer &socket){
    socketState = SocketStopped;
    clientSocketFd = 0;
}

SocketServer::~SocketServer(){
    if(clientSocketFd != 0) {
        LOG_INFO("close client socket")
        close(clientSocketFd);
    }
    socketState = SocketStopped;
}

SocketServer::SocketServer(const char *serverIP, int serverPort){
    socketState = SocketStopped;
    serverSocketFd = 0;
    clientSocketFd = 0;

    LOG_INFO("start socket server")

    int flag;
    socketState = SocketRunning;

    serverSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK(serverSocketFd, SOCKET_ERROR, { LOG_ERROR("Server socket init failed") socketState = SocketError; })

    struct sockaddr_in serverSocketAddr{};
    memset(&serverSocketAddr,0, sizeof(serverSocketAddr));

    serverSocketAddr.sin_family = AF_INET;
    serverSocketAddr.sin_port = htons(static_cast<uint16_t>(serverPort));
    serverSocketAddr.sin_addr.s_addr = inet_addr(serverIP);

    flag = bind(serverSocketFd, (struct sockaddr*)&serverSocketAddr, sizeof(serverSocketAddr));
    CHECK(flag, SOCKET_ERROR, {LOG_ERROR("Server bind error") socketState = SocketError;})

    flag = listen(serverSocketFd, 20);
    CHECK(flag, SOCKET_ERROR, {LOG_ERROR("Start(Listen) error") socketState = SocketError;})
}

int SocketServer::acceptConn(){
    struct sockaddr_in clientAddr{};
    socklen_t clientAddrSize = sizeof(clientAddr);

    clientSocketFd = accept(serverSocketFd, (struct sockaddr*)&clientAddr, &clientAddrSize);
    socketState = SocketConnSuccess;

    return socketState;
}

int SocketServer::sendMsg(void *buf, size_t size) {
    ssize_t sendSize = send(clientSocketFd, buf, size, 0);
    CHECK(sendSize,0,{LOG_ERROR("Send Size is NULL") socketState = SocketError;})

    return (int)sendSize;
}

int SocketServer::recvMsg(void *buf, size_t size) {
    ssize_t recvSize=read(clientSocketFd, buf, size);
    CHECK(recvSize,0,{LOG_ERROR("Receive Size is NULL") socketState = SocketError;})

    return (int)recvSize;
}

SocketState SocketServer::closeServer() {
    LOG_INFO("close server Socket")
    close(serverSocketFd);
    socketState = SocketKilled;

    return socketState;
}
}
