#ifndef INTER_VAR_H
#define INTER_VAR_H
#include <ctime>
#include <sys/time.h>
#include <string>

#include "language.h"

enum NetStatus {
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
    SERVER_DEVICE = 9,
    UNKNOWN_DEVICE = 10
};

/*************************客户端相关变量*********************************/
/* 路径变量 */
#define PATH_SIZE 1024

/* 时间戳类型 */
#define TIME_STAMP long long

/* 设置 */
#define SETTING_SERVER_IP_SIZE      32
#define SETTING_SERVER_PORT_SIZE    16
#define SETTING_NAME_SIZE 32
#define SETTING_VALUE_SIZE 32

/* 定时器 */
#define TIMER_REFRESH   (1000*60*10)
#define TIMER_UPLOAD    (1000*60*2)
#ifdef LINUX_CLIENT
#define TIMER_SAVE_DATA (1000*5)
#endif
/* 客户端版本： 3.1 */
#define MAIN_VERSION 3
#define NEXT_VERSION 1
#define VERSION_ID ((MAIN_VERSION * 10) + NEXT_VERSION)

/* 客户端类型 */
#define DEVICE_TYPE_MOD 1000

/* 客户端注册表信息，在Linux客户端中被隐藏 */
#define REGEDIT_AUTO_START_PATH "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define REGEDIT_KEY "logic"

/* 更新程序名字 */
#ifdef LINUX_CLIENT
#define UPDATE_PROGRAM "updateDeltaNote"
#endif
#ifdef WINDOW_CLIENT
#define UPDATE_PROGRAM "updateDeltaNote.exe"
#endif

#define RET_SUCCESS (0)
#define RET_FAILED (-1)

// 一些表的名字，数据库的名称定义
#define DB_NAME "db_test"
#define DB_SETTING_TABLE_NAME "setting_table"
#define DB_TODO_UI_TABLE_NAME "todo_ui_table"
#define DB_TODO_OP_TABLE_NAME "todo_op_table"


// 定义错误码
enum ErrorCode {
    CODE_NO_ERROR = 0,
    // 数据库部分
    DB_CREATE_ERROR = 0x00010000, // 数据库创建失败
};


enum IsCheck {
    Check_true = '1',
    Check_false = '0',
    Check_null = 0
};
std::string is_check_str(IsCheck is_check);
IsCheck is_check_enum(const std::string &is_check);
enum OpType{
    OpType_add = '1', // 新增
    OpType_del = '2', // 修改
    OpType_alt = '3', // 删除
    OpType_nul = 0,
};
std::string op_type_str(OpType op_type);
OpType op_type_enum(const std::string &op_type);

#define SETTING_USERNAME        "username"
#define SETTING_PASSWORD        "password"
#define SETTING_SERVER          "server"
#define SETTING_IS_LOGIN        "is_login"
#define SETTING_IS_LOCK         "is_lock"
#define SETTING_IS_AUTO_START   "is_auto_start"
#define SETTING_FONT_SIZE       "font_size"
#define SETTING_TRAN_POS        "tran_pos"
#define SETTING_FONT_COLOR      "font_color"
#define SETTING_ICON_COLOR      "icon_color"
#define SETTING_X_POS           "x_pos"
#define SETTING_Y_POS           "y_pos"
#define SETTING_HEIGHT          "height"
#define SETTING_WIDTH           "width"
struct NoteSetting{
    std::string username;
    std::string password;

    std::string server; // ip:port

    bool is_login;
    bool is_lock;
    bool is_auto_start;

    int font_size; // 字体大小配置
    int tran_pos;  // 透明度记录

    // 颜色名称记录
    std::string font_color;
    std::string icon_color;

    // 窗体定位信息
    int    x_pos;
    int    y_pos;
    int    height;
    int    width;

    NoteSetting();
};

struct TodoItem{
    std::string create_key;
    std::string edit_key;
    OpType op_type;
    IsCheck is_check;
    std::string data;
    TodoItem();
    TodoItem(const std::string &create_key, const std::string &edit_key, OpType op_type, IsCheck is_check, const std::string &data);
};
bool check_item_valid(const TodoItem &item);

#define TodoList std::list<struct TodoItem>
#define TODO_ITEM_CREATE_KEY    "create_key"
#define TODO_ITEM_EDIT_KEY      "edit_key"
#define TODO_ITEM_OP_TYPE       "op_type"
#define TODO_ITEM_IS_CHECK      "is_check"
#define TODO_ITEM_DATA          "data"

/* 获取MS时间 -3 */
static uint64_t get_time_of_ms()
{
    struct timeval tv{};
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * (uint64_t)1000 + tv.tv_usec / 1000;
}

static std::string get_time_key(){
    return std::to_string(get_time_of_ms());
}

#endif //INTER_VAR_H
