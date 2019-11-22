#ifndef PACK_H
#define PACK_H

#define G_ARR_SIZE_SERVER 16
#define G_ARR_SIZE_USERNAME 8
#define G_ARR_SIZE_PASSWD 12
#define G_MAX_MSG_OP_RECV_SIZE 5
#define G_TIMESTAMP_SIZE 32
#define G_DATA_TRANS_SIZE 128

enum MSG_State {
    LoginUndefinedError = 0,
    LoginPasswdError = 1,
    LoginUserNotExits = 2,
    LoginSuccess = 3,

    CreateUserUserExists = 8,
    CreateUserSuccess = 9,
    CreateUserUndefinedError = 10,

    ConnectError = 16,

    PushSuccess = 24,
    PushError = 25,

    CleanSuccess = 26,
    CleanError = 27,

    UndefinedError
};

static MSG_State  g_msgState;

enum MSG_OP {
    CreateUser = '0',
    Login = '1',
    Pull = '2',
    Push = '3',
    Delete = '4',
    RET = '5'
};

enum MSG_SEG {
    MSG_FULL = '0',
    MSG_HALF = '1'
};

enum Check {
    Checked = '1',
    UnCheck = '0'
};

enum TODO_OP{
    ADD = '1',
    DEL = '2',
    ALTER = '3',
    CHECK = '4',
    NULL_OP
};

struct MSG_OP_PACK {
    char opTimestamp[G_TIMESTAMP_SIZE];
    char createTimestamp[G_TIMESTAMP_SIZE];
    char data[G_DATA_TRANS_SIZE];
    char op;
    char isCheck;
};

struct MSG {
    char msgOp;
    char msgState;
    char userName[G_ARR_SIZE_USERNAME];
    char passwd[G_ARR_SIZE_PASSWD];

    int msgSize;
    char msg_seg;
    MSG_OP_PACK msgQueue[G_MAX_MSG_OP_RECV_SIZE];
};
#endif // PACK_H
