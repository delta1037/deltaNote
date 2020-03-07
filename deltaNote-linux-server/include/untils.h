//
// Created by delta on 11/20/19.
//

#ifndef DELTANOTE_UNTILS_H
#define DELTANOTE_UNTILS_H

#include <cstring>
#include <vector>

using namespace std;

#define PATH_SIZE 1024

#define G_ARR_SIZE_USERNAME 16
#define G_ARR_SIZE_PASSWD 64

#define G_MAX_MSG_OP_RECV_SIZE 5

#define G_TIMESTAMP_SIZE 32
#define G_DATA_TRANS_SIZE 128

#define G_DATABASE_NAME_SIZE 1024
#define G_DATABASE_USERNAME_SIZE 16
#define G_DATABASE_TABLE_NAME_SIZE 32

#define G_USERDATA_SETTING_SIZE 32
#define G_USERDATA_VALUE_SIZE 32

enum MSG_State {
    LoginPasswdError = 1,
    LoginUserNotExits = 2,
    LoginSuccess = 3,
    LoginUndefinedError = 4,

    CreateUserUserExists = 8,
    CreateUserSuccess = 9,
    CreateUserUndefinedError = 10,

    ConnectError = 16,

    PushSuccess = 24,
    PushError = 25,

    CleanSuccess = 26,
    CleanError = 27,

    OperateNotDefine = 36,

    PullSuccess,
    PullError,

    UndefinedError
};

enum MSG_OP {
    CreateUser = '0',
    Login = '1',
    Pull = '2',
    Push = '3',
    Delete = '4',
    RET = '5',
    ACK = '6',
    MSG_OP_NULL
};

enum MSG_SEG {
    MSG_FULL = '0',
    MSG_HALF = '1',
    MSG_NONE
};

enum Check {
    Checked = '1',
    UnCheck = '0'
};

enum TODO_OP{
    TODO_ADD = '1',
    TODO_DEL = '2',
    TODO_ALTER = '3',
    TODO_CHECK = '4',
    TODO_NULL_OP
};

struct UserInfo {
    char userName[G_ARR_SIZE_USERNAME];
    char passWord[G_ARR_SIZE_PASSWD];

    UserInfo(){
        memset(userName, 0, G_ARR_SIZE_USERNAME);
        memset(passWord, 0, G_ARR_SIZE_PASSWD);
    }

    UserInfo(char *userName, char *passWord){
        strcpy(this->userName, userName);
        strcpy(this->passWord, passWord);
    }

    UserInfo(UserInfo &userInfo){
        strcpy(this->userName, userInfo.userName);
        strcpy(this->passWord, userInfo.passWord);
    }
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

    struct SocketMsgOpPack msgQueue[G_MAX_MSG_OP_RECV_SIZE];

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

        strcpy(this->userName, userName);
        strcpy(this->passwd, passwd);
    }

    SocketMsgPack(char msgOp,
                  char msgSeg,
                  char msgState,
                  int msgSize,
                  char userName[],
                  char passwd[],
                  vector<struct SocketMsgOpPack> &msgQueue){
        this->msgOp = msgOp;
        this->msgSeg = msgSeg;
        this->msgState = msgState;
        this->msgSize = msgSize;

        strcpy(this->userName, userName);
        strcpy(this->passwd, passwd);

        for(int i = 0 ; i < this->msgSize; ++i){
            this->msgQueue[i] = msgQueue[i];
        }
    }

    SocketMsgPack(SocketMsgPack &msg){
        this->msgOp = msg.msgOp;
        this->msgSeg = msg.msgSeg;
        this->msgState = msg.msgState;

        this->msgSize = msg.msgSize;

        strcpy(this->userName, msg.userName);
        strcpy(this->passwd, msg.passwd);

        for(int i = 0 ; i < this->msgSize; ++i){
            this->msgQueue[i] = msg.msgQueue[i];
        }
    }
};
#endif //DELTANOTE_UNTILS_H
