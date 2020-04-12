#ifndef LANGUAGE_H
#define LANGUAGE_H
/**
  * 语言设置：英语和汉语
  */
//#define EN_STRING_TABLE
#ifdef EN_STRING_TABLE
#define INFO_MESSAGE                        "Info: "
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

#define UPDATE_CONNECT_ERROR                LOGIN_SERVER_CON_ERROR
#define UPDATE_ALREADY_NEW                  "version already newest"
#define UPDATE_FAIL                         "update fail"

#define MAIN_WIN_OFFICIAL_SITE              "OfficialSite"
#define MAIN_WIN_SETTING                    "Setting"
#define MAIN_WIN_UPDATE                     "CheckUpdate"
#define MAIN_WIN_QUIT                       "Quit"
#define MAIN_WIN_CLEAR_DONE                 "Clear Done"

#define SOCKET_SERVER_FORMAT_ERROR          "server_port format error"
#define SOCKET_SERVER_ADDR_ERROR            "host name unreachable"
#else
#define INFO_MESSAGE                        "通知: "
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

#define UPDATE_CONNECT_ERROR                LOGIN_SERVER_CON_ERROR
#define UPDATE_ALREADY_NEW                  "当期版本已是最新"
#define UPDATE_FAIL                         "更新失败"

#define MAIN_WIN_OFFICIAL_SITE              "官网"
#define MAIN_WIN_SETTING                    "设置"
#define MAIN_WIN_UPDATE                     "检查更新"
#define MAIN_WIN_QUIT                       "退出"
#define MAIN_WIN_CLEAR_DONE                 "清理已做"

#define SOCKET_SERVER_FORMAT_ERROR          "服务器格式错误"
#define SOCKET_SERVER_ADDR_ERROR            "服务器不可达"
#endif

#endif // LANGUAGE_H
