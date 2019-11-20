//
// Created by geniusrabbit on 18-2-27.
//

#include "socket.h"
#include "log.h"

SocketClient::SocketClient(){
    serverPort = 0;
    socketState = SocketStopped;
    clientSocketFd = 0;
    ret = 0;
}

SocketClient::SocketClient(char *_serverIP, int _serverPort){
    strcpy(serverIP, _serverIP);
    serverPort = _serverPort;
    socketState = SocketStopped;
    clientSocketFd = 0;

    LOG_INFO("start socket client")

    socketState = SocketRunning;

    clientSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK(clientSocketFd, SOCKET_ERROR, { LOG_ERROR("Client socket init failed") socketState = SocketError; })

    struct sockaddr_in serverSocketAddr;
    memset(&serverSocketAddr,0, sizeof(serverSocketAddr));
    serverSocketAddr.sin_family = AF_INET;
    serverSocketAddr.sin_port = htons(static_cast<uint16_t>(serverPort));
    serverSocketAddr.sin_addr.s_addr = inet_addr(serverIP);

    ret = connect(clientSocketFd, (struct sockaddr*)&serverSocketAddr, sizeof(serverSocketAddr));
    socketState = SocketConnSuccess;

    LOG_INFO("start socket success")
}

int SocketClient::sendMsg(void *buf, size_t size) {
    ssize_t sendSize = send(clientSocketFd, buf, size, 0);
    CHECK(sendSize,0,{LOG_ERROR("Send Size is NULL") socketState = SocketError; return 0;})

    return int(sendSize);
}

int SocketClient::recvMsg(void *buf, size_t size) {
    ssize_t recvSize=read(clientSocketFd, buf, size);
    CHECK(recvSize,0,{LOG_ERROR("Receive Size is NULL") socketState = SocketError;})

    return int(recvSize);
}

SocketState SocketClient::closeClient() {
    LOG_INFO("close client socket")
    close(clientSocketFd);
    socketState = SocketStopped;

    return socketState;
}
