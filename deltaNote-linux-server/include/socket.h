//
// Created by geniusrabbit on 18-2-27.
//

#ifndef GENIUSNOTE1_0_SOCKETSERVER_H
#define GENIUSNOTE1_0_SOCKETSERVER_H

#include <string>
#include <netdb.h>
#include <sys/socket.h>
#include "sqlite.h"
#include "pack.h"

namespace deltaNote{
using namespace std;

#define SOCKET_ERROR (-1)
#define SOCKET_SUCCESS (0)

enum SocketState {
    SocketRunning = 1,
    SocketStopped = 2,
    SocketConnSuccess = 3,
    SocketKilled = 4,
    SocketError=0
};

class SocketServer{
public:
    static int serverSocketFd;
    SocketServer(SocketServer &socket);
    SocketServer(const char *_serverIP, int _serverPort);
    ~SocketServer();

    int acceptConn();
    int sendMsg(void* buf,size_t size);
    int recvMsg(void* buf,size_t size);
    SocketState closeServer();

private:
    SocketState socketState;
    int clientSocketFd;
};
}

#endif //GENIUSNOTE1_0_SOCKETSERVER_H

