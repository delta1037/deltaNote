#ifndef DELTANOTE_UNTILS
#define DELTANOTE_UNTILS

#include <cstdio>
#include <cstring>
#include <ctime>
#include <vector>
#include <cstdint>
#include <string>

/*************************服务端相关变量*********************************/
/* 操作记录*/
#define PACK_TIMESTAMP_SIZE 32
#define PACK_TODO_DATA_SIZE 128

/* 用户 */
#define PACK_USERNAME_SIZE 16
#define PACK_PASSWORD_SIZE 64

/* 数据库 */
#define DATABASE_NAME_SIZE          1024
#define DATABASE_USERNAME_SIZE      16
#define DATABASE_TABLE_NAME_SIZE    32

/* 下载链接 */
#define DOWNLOAD_LINK_SIZE 128

/* 包的大小 */
#define MSG_HEAD_SIZE           sizeof(struct MsgHead)
#define MSG_BODY_OP_SIZE        sizeof(struct MsgBodyOp)
#define MSG_BODY_ACK_SIZE       sizeof(struct MsgBodyAck)
#define MSG_BODY_DOWNLOAD_SIZE  sizeof(struct MsgBodyDownload)
#define MSG_OP_PACK_SIZE        sizeof(struct SocketMsgOpPack)

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

struct UserID {
    char userName[PACK_USERNAME_SIZE];
    char passWord[PACK_PASSWORD_SIZE];

    UserID(){
        memset(userName, 0, PACK_USERNAME_SIZE);
        memset(passWord, 0, PACK_PASSWORD_SIZE);
    }

    UserID(char userName[], char passWord[]){
        strcpy(this->userName, userName);
        strcpy(this->passWord, passWord);
    }

    UserID(const UserID &userId){
        strcpy(this->userName, userId.userName);
        strcpy(this->passWord, userId.passWord);
    }
};

struct SocketMsgOpPack {
    char operation;                                 // 数据的操作，插入，修改，删除
    char isCheck;                                   // 该信息有没有check

    char createTimestamp[PACK_TIMESTAMP_SIZE];      // 数据创建的时间，作为消息的唯一标志
    char operateTimestamp[PACK_TIMESTAMP_SIZE];     // 数据最后修改的时间，初始值等于消息创建的时间

    char data[PACK_TODO_DATA_SIZE];                 // 数据

    SocketMsgOpPack(){
        this->operation = TODO_NULL_OP;
        this->isCheck = UnCheck;

        memset(createTimestamp, 0, PACK_TIMESTAMP_SIZE);
        memset(operateTimestamp, 0, PACK_TIMESTAMP_SIZE);
        memset(data, 0, PACK_TODO_DATA_SIZE);
    }

    SocketMsgOpPack(char *operateTimestamp, char *createTimestamp, char operation, char isCheck, char *data){
        strcpy(this->operateTimestamp, operateTimestamp);
        strcpy(this->createTimestamp, createTimestamp);
        strcpy(this->data, data);
        this->operation = operation;
        this->isCheck = isCheck;
    }

    SocketMsgOpPack(const SocketMsgOpPack &msg){
        strncpy(this->operateTimestamp, msg.operateTimestamp, PACK_TIMESTAMP_SIZE);
        strncpy(this->createTimestamp, msg.createTimestamp, PACK_TIMESTAMP_SIZE);
        strncpy(this->data, msg.data, PACK_TODO_DATA_SIZE);
        this->operation = msg.operation;
        this->isCheck = msg.isCheck;
    }
};

enum MsgStatus {
    LoginPasswdError = 1,
    LoginUserNotExits = 2,
    LoginSuccess = 3,
    LoginUndefinedError = 4,

    CreateUserUserExists = 8,
    CreateUserSuccess = 9,
    CreateUserUndefinedError = 10,

    UserIDNull,

    ConnectError = 16,

    PushSuccess = 24,
    PushError = 25,

    CleanSuccess = 26,
    CleanError = 27,

    OperateNotDefine = 36,

    PullSuccess,
    PullError,

    VersionError,
    RecvSizeError,

    GetUpdateLink,
    NoUpdateLink,
    AlreadyLatest,

    UndefinedError
};

enum DeviceType {
    WINDOW_DEVICE = 1,
    LINUX_DEVICE,
    ANDROID_DEVICE,
    CHROME_DEVICE,
    UNKNOWN_DEVICE = 99
};

enum MsgType {
    /* 操作类型 */
    OP_CREATE_USER_TYPE = '0',
    OP_LOGIN_TYPE,
    OP_PULL_TYPE,
    OP_PUSH_TYPE,

    /* 确认包类型 */
    ACK_STATUS_TYPE = '6',

    /* 软件自动更新类型 */
    UPDATE_GET_LINK_TYPE = '7',

    /* 未定义类型 */
    NONE_TYPE = 'N'
};

struct MsgHead {
    uint16_t    todoVersion;                        /* 版本 */
    MsgType    msgType;                             /* 包的类型 */
    uint32_t    msgBodySize;                        /* 除去包头需要接收的大小 */

    MsgHead(){
        todoVersion = 0;
        msgType = NONE_TYPE;
        msgBodySize = 0;
    }
};

struct MsgBodyOp {
    struct      UserID userID;                      /* 用户的信息 */
    uint32_t    msgOpQueueSize;                     /* 操作队列的大小 */
    struct      SocketMsgOpPack *operationQueue;    /* 操作队列 */

    MsgBodyOp(){
        msgOpQueueSize = 0;
        operationQueue = nullptr;
    }
};

struct MsgBodyAck {
    MsgStatus   msgStatus;                           /* 信息传输返回的状态信息 */
    uint32_t    msgQueueAckSize;                     /* 操作队列的大小 */

    MsgBodyAck(){
        msgStatus = UndefinedError;
        msgQueueAckSize = 0;
    }
};

struct MsgBodyDownload {
    char downloadLink[DOWNLOAD_LINK_SIZE];   /* 下载链接 */
    uint32_t updateMessageSize;
    char     *updateMessage;

    MsgBodyDownload(){
        memset(downloadLink, 0, DOWNLOAD_LINK_SIZE);
        updateMessageSize = 0;
        updateMessage = nullptr;
    }
};

class MsgPack {
private:
    /* 包头 */
    struct MsgHead msgHead;

    /* 操作包包体：登录，新建用户，上传包，下载包 */
    struct MsgBodyOp msgBodyOp;

    /* 确认包包体：返回操作状态，成功的包个数，更新链接 */
    struct MsgBodyAck msgBodyAck;

    /* 下载更新包 */
    struct MsgBodyDownload msgBodyDownload;
public:
    ~MsgPack();

    struct MsgHead &getMsgHead();
    struct MsgBodyOp &getMsgBodyOp();
    struct MsgBodyAck &getMsgBodyAck();
    struct MsgBodyDownload &getMsgBodyDownLoad();

    void setTodoVersion(uint16_t);
    uint16_t getTodoVersion();

    void setTodoDeviceType(DeviceType);
    DeviceType getTodoDeviceType();

    void setMSgType(MsgType);
    MsgType getMsgType();

    void setMsgBodySize(uint32_t);
    uint32_t getMsgBodySize();

    void setUserID(struct UserID &);
    struct UserID &getUserID();

    uint32_t getOpQueueSize();
    void setOpQueueSize(uint32_t);

    void setOperationQueue(std::vector<struct SocketMsgOpPack>&);
    void getOperationQueue(std::vector<struct SocketMsgOpPack>&);

    void setMsgState(MsgStatus);
    MsgStatus getMsgState();

    void setOpQueueAckSize(uint32_t);
    uint32_t getOpQueueAckSize();

    void setDownloadLink(const char []);
    void setDownloadLink(std::string&);
    char *getDownloadLink();

    void setDownloadMessageSize(uint32_t);
    uint32_t getDownloadMessageSize();

    void setDownloadMessage(const char *);
    char *getDownloadMessage();
};

/********************************服务端本地变量***************************************/
/* 路径相关变量 */
#define PATH_SIZE 1024

/* 客户端地址 */
#define CLIENT_IP_ADDR_SIZE 32

/* 服务端版本： 3.1 */
#define MAIN_VERSION 3
#define NEXT_VERSION 1
#define VERSION_ID ((MAIN_VERSION * 10) + NEXT_VERSION)

/* 类型获取 */
#define DEVICE_TYPE_MOD 1000

/* 下载链接与说明 */
#define LINUX_CLIENT_MESSAGE "<h1>Linux 更新说明</h1><p>这是一个段落</p>"
#define LINUX_CLIENT_DOWNLOAD_LINK "http://39.96.162.190:233/deltaNote_client"
#define WINDOW_CLIENT_MESSAGE "<h1>Window 更新说明</h1><p>这是一个段落</p>"
#define WINDOW_CLIENT_DOWNLOAD_LINK "http://39.96.162.190:233/deltaNote_client.exe"

/* 数据库SQL操作 */
extern const char *SQL_USER_INFO_TABLE_CREATE;
extern const char *SQL_USER_INFO_TABLE_ADD;
extern const char *SQL_USER_INFO_TABLE_DELETE;
extern const char *SQL_USER_INFO_TABLE_SELECT;
extern const char *SQL_USER_INFO_TABLE_SELECT_ALL;
extern const char *SQL_USER_CHANGE_TABLE_UPDATE;

extern const char *SQL_USER_DATABASE_TABLE_CREATE;
extern const char *SQL_USER_DATABASE_TABLE_INSERT;
extern const char *SQL_USER_DATABASE_TABLE_DELETE_ONE;
extern const char *SQL_USER_DATABASE_TABLE_DELETE_ALL;
extern const char *SQL_USER_DATABASE_TABLE_SELECT_ONE;
extern const char *SQL_USER_DATABASE_TABLE_SELECT_ALL;
extern const char *SQL_USER_DATABASE_TABLE_SELECT_OP;
extern const char *SQL_USER_NOTE_RECORDS_TABLE_UPDATE;
#endif // DELTANOTE_UNTILS
