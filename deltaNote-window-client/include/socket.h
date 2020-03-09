#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <unistd.h>
#include <cstring>
#include <winsock.h>

#include "log.h"
#include "todolistcontrol.h"

using namespace std;

//#define SOCKET_ERROR (-1)
#define SOCKET_SUCCESS (0)


enum SocketState {
    SocketRunning = 1,
    SocketStopped = 2,
    SocketConnSuccess = 3,
    SocketKilled = 4,
    SocketError=0
};

struct SocketMsgOpPack {
    char operation;                                // 数据的操作，插入，修改，删除
    char isCheck;                           // 该信息有没有check

    char createTimestamp[G_TIMESTAMP_SIZE]; // 数据创建的时间，作为消息的唯一标志
    char operateTimestamp[G_TIMESTAMP_SIZE];     // 数据最后修改的时间，初始值等于消息创建的时间

    char data[G_DATA_TRANS_SIZE];           // 数据

    SocketMsgOpPack(){
        this->operation = TODO_NULL_OP;
        this->isCheck = UnCheck;

        memset(createTimestamp, 0, G_TIMESTAMP_SIZE);
        memset(operateTimestamp, 0, G_TIMESTAMP_SIZE);
        memset(data, 0, G_DATA_TRANS_SIZE);
    }

    SocketMsgOpPack(char *operateTimestamp, char *createTimestamp, char operation, char isCheck, char *data){
        strcpy(this->operateTimestamp, operateTimestamp);
        strcpy(this->createTimestamp, createTimestamp);
        strcpy(this->data, data);
        this->operation = operation;
        this->isCheck = isCheck;
    }

    SocketMsgOpPack(MsgOpPack &msg){
        this->operation = msg.operation;
        this->isCheck = msg.isCheck;

        sprintf(this->createTimestamp, "%lld", msg.createTime);
        sprintf(this->operateTimestamp, "%lld", msg.operateTime);
        strcpy(this->data, msg.data.toUtf8().data());
    }

    SocketMsgOpPack(const SocketMsgOpPack &msg){
        strncpy(this->operateTimestamp, msg.operateTimestamp, G_TIMESTAMP_SIZE);
        strncpy(this->createTimestamp, msg.createTimestamp, G_TIMESTAMP_SIZE);
        strncpy(this->data, msg.data, G_DATA_TRANS_SIZE);
        this->operation = msg.operation;
        this->isCheck = msg.isCheck;
    }
};

struct SocketMsgPack {
    char msgOp;
    char msgSeg;
    char msgState;

    int msgSize;

    char userName[G_ARR_SIZE_USERNAME];
    char passwd[G_ARR_SIZE_PASSWD];

    SocketMsgOpPack msgQueue[G_MAX_MSG_OP_RECV_SIZE];

    SocketMsgPack(){
        this->msgState = UndefinedError;
        this->msgSize = 0;
    }

    SocketMsgPack(char msgOp,
                  char msgSeg,
                  char msgState,
                  int msgSize,
                  char userName[],
                  char passwd[]){
        this->msgOp = msgOp;
        this->msgSeg = msgSeg;
        this->msgState = msgState;
        this->msgSize = msgSize;

        strncpy(this->userName, userName, G_ARR_SIZE_USERNAME);
        strncpy(this->passwd, passwd, G_ARR_SIZE_PASSWD);
    }

    SocketMsgPack(char msgOp,
                  char msgSeg,
                  char msgState,
                  int msgSize,
                  char userName[],
                  char passwd[],
                  std::vector<MsgOpPack> &msgQueue){
        this->msgOp = msgOp;
        this->msgSeg = msgSeg;
        this->msgState = msgState;
        this->msgSize = msgSize;

        strncpy(this->userName, userName, G_ARR_SIZE_USERNAME);
        strncpy(this->passwd, passwd, G_ARR_SIZE_PASSWD);

        for(int i = 0 ; i < this->msgSize; ++i){
            this->msgQueue[i] = SocketMsgOpPack(msgQueue[i]);
        }
    }

    SocketMsgPack(SocketMsgPack &msg){
        this->msgOp = msg.msgOp;
        this->msgSeg = msg.msgSeg;
        this->msgState = msg.msgState;

        this->msgSize = msg.msgSize;

        strncpy(this->userName,  msg.userName, G_ARR_SIZE_USERNAME);
        strncpy(this->passwd, msg.passwd, G_ARR_SIZE_PASSWD);

        for(int i = 0 ; i < this->msgSize; ++i){
            this->msgQueue[i] = msg.msgQueue[i];
        }
    }
};

class SocketClient{
public:
    SocketClient();
    SocketClient(char *_serverIP, int _serverPort);
    ~SocketClient();

    bool initSocket();

    bool sendMsg(void *buf,int size);
    bool recvMsg(void* buf,int size);
    SocketState closeClient();
    SocketState getSocketOpState();

private:
    char serverIP[G_ARR_SIZE_SERVER]{};
    int serverPort;

    WSADATA wsa;

    int ret;
    SocketState socketState;
    SOCKET clientSocketFd;
};

class ServerConnectControl {
public:
    ServerConnectControl();
    ~ServerConnectControl();

    bool initConnect();

    void parserServerPort(char *serverPort);

    void loginToServer();
    void createNewUser();
    bool loadFromServer(std::vector<MsgOpPack> &msgQueue);
    bool uploadToServer(std::vector<MsgOpPack> &msgQueue);

    MSG_State getState();

private:
    bool isIPAddr(const char* pStr);
    int getIPbyDomain(const char* domain, char* ip);
    void communicateWithServer(MSG_OP op);

private:
    bool init;
    SocketClient *socketClient;
    MSG_State state;
};

#endif // SOCKET_H
