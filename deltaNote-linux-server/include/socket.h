//
// Created by geniusrabbit on 18-2-27.
//

#ifndef GENIUSNOTE1_0_SOCKETSERVER_H
#define GENIUSNOTE1_0_SOCKETSERVER_H

#include <string>
#include <netdb.h>
#include <sys/socket.h>
#include "sqlite.h"

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
    static bool initSocketServer(const char *serverIP, int serverPort);

    SocketServer();
    ~SocketServer();

    bool acceptConn();
    bool sendMsg(void* buf,size_t size);
    bool recvMsg(void* buf,size_t size);
    SocketState closeServer();

private:
    SocketState socketState;
    int clientSocketFd;
};

class ServerConnectControl {
public:
    static bool initServerConnect(const char *serverIP, int serverPort);

    ServerConnectControl();
    ~ServerConnectControl();

    bool acceptNewConnect();

    void processingClientRequest();
    //
    bool returnStateMsg(struct SocketMsgPack &returnMsg);

    void loginToServer();

    void createNewUser();
    void loadFromServer();
    void uploadToServer();
private:
    MSG_State state;
    struct SocketMsgPack socketMsgPack;
    SocketServer *socketServer;
    struct ServerDataControl *dataControl;
};
#endif //GENIUSNOTE1_0_SOCKETSERVER_H

