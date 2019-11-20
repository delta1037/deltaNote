#ifndef PACK_H
#define PACK_H

enum MSG_State {
    LoginSuccess = 0,
    LoginPasswdError = 1,
    LoginUserNotExits = 2,
    LoginUndefinedError = 3,

    CreateUserUserExists = 8,
    CreateUserSuccess = 9,
    CreateUserUndefinedError = 10,

    ConnectError = 16,

    PushSuccess = 24,
    PushError = 25,

    UndefinedError
};

static MSG_State  g_msgState;

enum MSG_OP {
    CreateUser = '0',
    Login = '1',
    Pull = '2',
    Push = '3'
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
    CHECK = '4'
};

struct MSG_OP_PACK {
    char opTimestamp[32];
    char createTimestamp[32];
    char data[128];
    char op;
    char isCheck;
};

struct MSG {
    char msgOp;
    char msgState;
    char userName[8];
    char timestamp[32];
    char passwd[128];

    int msgSize;
    char msg_seg;
    MSG_OP_PACK msgQueue[5];
};

#endif // PACK_H
