#ifndef UNTILS_H
#define UNTILS_H

#include <QColor>
#include <QRect>
#include <QWidget>

//#define EN_STRING_TABLE
#ifdef EN_STRING_TABLE
#define WARNING_MESSAGE                     "Warn: "
#define ERROR_MESSAGE                       "Error: "
#define LOGIN_SERVER_NULL                   "server & port is null!"
#define LOGIN_USERNAME_NULL                 "username is null!"
#define LOGIN_PASSWORD_NULL                 "password is null!"
#define LOGIN_PASSWORD_ERROR                "user name or password error!"
#define LOGIN_USER_N_EXITS                  "user is not exists!"
#define LOGIN_SERVER_ERROR                  "server undefined error!"
#define LOGIN_SERVER_CON_ERROR              "server cant connect"

#define NEW_USER_SERVER_NULL                LOGIN_SERVER_NULL
#define NEW_USER_USERNAME_NULL              LOGIN_USERNAME_NULL
#define NEW_USER_PASSWORD_NULL              LOGIN_PASSWORD_NULL
#define NEW_USER_PASSWD_INCPABLE            "password is inconpable!"
#define NEW_USER_USER_EXITS                 "username is exists!"
#define NEW_USER_SERVER_ERROR               LOGIN_SERVER_ERROR
#define NEW_USER_SERVER_CON_ERROR           LOGIN_SERVER_CON_ERROR

#define MAIN_WIN_OFFICIAL_SITE              "OfficialSite"
#define MAIN_WIN_SETTING                    "Setting"
#define MAIN_WIN_QUIT                       "Quit"
#define MAIN_WIN_CLEAR_DONE                 "Clear Done"

#define SOCKET_SERVER_FORMAT_ERROR          "server_port format error"
#define SOCKET_SERVER_ADDR_ERROR            "host name unreachable"
#else
#define WARNING_MESSAGE                     "警告: "
#define ERROR_MESSAGE                       "错误: "
#define LOGIN_SERVER_NULL                   "服务器地址不能为空"
#define LOGIN_USERNAME_NULL                 "用户名不能为空"
#define LOGIN_PASSWORD_NULL                 "密码不能为空"
#define LOGIN_PASSWORD_ERROR                "密码错误"
#define LOGIN_USER_N_EXITS                  "用户名不存在"
#define LOGIN_SERVER_ERROR                  "服务器处理错误"
#define LOGIN_SERVER_CON_ERROR              "服务器无法连接"

#define NEW_USER_SERVER_NULL                LOGIN_SERVER_NULL
#define NEW_USER_USERNAME_NULL              LOGIN_USERNAME_NULL
#define NEW_USER_PASSWORD_NULL              LOGIN_PASSWORD_NULL
#define NEW_USER_PASSWD_INCPABLE            "密码不一致"
#define NEW_USER_USER_EXITS                 "用户名已存在"
#define NEW_USER_SERVER_ERROR               LOGIN_SERVER_ERROR
#define NEW_USER_SERVER_CON_ERROR           LOGIN_SERVER_CON_ERROR

#define MAIN_WIN_OFFICIAL_SITE              "官网"
#define MAIN_WIN_SETTING                    "设置"
#define MAIN_WIN_QUIT                       "退出"
#define MAIN_WIN_CLEAR_DONE                 "清理已做"

#define SOCKET_SERVER_FORMAT_ERROR          "服务器格式错误"
#define SOCKET_SERVER_ADDR_ERROR            "服务器不可达"
#endif

#define PATH_SIZE 1024

#define G_ARR_SIZE_SERVER_PORT 64
#define G_ARR_SIZE_SERVER 16

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

#define REGEDIT_AUTO_START_PATH "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define REGEDIT_KEY "deltaNote"





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


extern char g_username[G_ARR_SIZE_USERNAME];
extern char g_passwd[G_ARR_SIZE_PASSWD];

extern char g_server[G_ARR_SIZE_SERVER];
extern int g_port;

extern bool isLogin;
extern bool isLocked;
extern bool isAutoStart;

extern QColor fontColor;
extern QColor iconColor;
extern int transparentPos;

extern int xPos;
extern int yPos;

extern int frameWidth;
extern int frameHeight;

extern int fontSize;

extern QRect desktopRect;
extern QWidget *mainWindowWidget;

#endif // UNTILS_H
