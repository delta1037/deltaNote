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
    clientSocketFd = 0;
    socketState = SocketRunning;
    LOG_INFO("start socket client")

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2),&wsa);

    clientSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK(clientSocketFd, SOCKET_ERROR, { LOG_ERROR("Client socket init failed") socketState = SocketError; })

    SOCKADDR_IN serverSocketAddr;
    memset(&serverSocketAddr,0, sizeof(serverSocketAddr));
    serverSocketAddr.sin_family = AF_INET;
    serverSocketAddr.sin_port = htons(static_cast<uint16_t>(serverPort));
    serverSocketAddr.sin_addr.s_addr = inet_addr(serverIP);

    ret = connect(clientSocketFd, (SOCKADDR*)&serverSocketAddr, sizeof(serverSocketAddr));
    CHECK(clientSocketFd, SOCKET_ERROR, { LOG_ERROR("connect server failed") socketState = SocketError; })
    LOG_INFO("start socket success")
}

int SocketClient::sendMsg(void *buf, int size) {
    ssize_t sendSize = send(clientSocketFd, (char *)buf, size, 0);
    CHECK(sendSize,0,{LOG_ERROR("Send Size is NULL") socketState = SocketError; return 0;})

    return int(sendSize);
}

int SocketClient::recvMsg(void *buf, int size) {
    ssize_t recvSize = recv(clientSocketFd, (char*)buf, size, 0);
    CHECK(recvSize,0,{LOG_ERROR("Receive Size is NULL") socketState = SocketError;})

    return int(recvSize);
}
 SocketClient::~SocketClient(){
     LOG_INFO("close client socket")
     close(clientSocketFd);
     socketState = SocketStopped;
     //close ws2_32.dll
     WSACleanup();
}
SocketState SocketClient::closeClient() {
    LOG_INFO("close client socket")
    close(clientSocketFd);
    socketState = SocketStopped;

    return socketState;
}

SocketState SocketClient::getSocketOpState() {
    return socketState;
}
