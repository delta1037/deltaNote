#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <unistd.h>
#include <cstring>
#include <winsock.h>

#include "untils.h"

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

class SocketClient{
public:
    SocketClient();
    SocketClient(char *_serverIP, int _serverPort);
    ~SocketClient();

    int sendMsg(void *buf,int size);
    int recvMsg(void* buf,int size);
    SocketState closeClient();
    SocketState getSocketOpState();

private:
    char serverIP[16]{};
    int serverPort;

    int ret;
    SocketState socketState;

    SOCKET clientSocketFd;
};
#endif // SOCKET_H
