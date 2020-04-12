#ifndef DELTA_NOTE_SOCKET
#define DELTA_NOTE_SOCKET

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "untils.h"
#include "../logCtrl/logCtrl.h"

#define SOCKET_ERROR (-1)
#define SOCKET_SUCCESS (0)

class Socket{
public:
    // server
    static int serverSocketFd;
    static bool initSocketServer(const char[], int);

public:
    Socket();
    ~Socket();

    bool acceptConn();                      // new client connect
    bool sendMsg(void* buf,size_t size);    // send msg to client
    bool recvMsg(void* buf,size_t size);    // recv msg from client

    void getClientIPAddr(char r_clientIPAddr[]);

private:
    // client connect
    int clientSocketFd;
    char clientIPAddr[CLIENT_IP_ADDR_SIZE];
};
#endif //DELTA_NOTE_SOCKET

