#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <unistd.h>
#include <cstring>

#include "log/logCtrl.h"
#include "common/untils.h"

#ifdef WINDOW_CLIENT
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib" ) // linker must use this lib for sockets

#define SOCKET_SUCCESS (0)
#endif

#ifdef LINUX_CLIENT
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SOCKET_ERROR (-1)
#define SOCKET_SUCCESS (0)
#define INVALID_SOCKET SOCKET_ERROR
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#endif

class SocketClient{
public:
    SocketClient();
    virtual ~SocketClient();

    bool initSocket(char *, int);
    bool sendMsg(void *buf,size_t size);
    bool recvMsg(void* buf,size_t size);

private:
    int ret;
#ifdef WINDOW_CLIENT
    WSADATA wsa;
    SOCKET clientSocketFd;
#endif
#ifdef LINUX_CLIENT
    int clientSocketFd;
#endif
};
#endif // SOCKET_H
