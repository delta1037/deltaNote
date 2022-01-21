#ifndef INTER_VAR_H
#define INTER_VAR_H
#include <ctime>
#include <sys/time.h>
#include <string>

#define RET_SUCCESS (0)
#define RET_FAILED (-1)

// 一些表的名字，数据库的名称定义
#define DB_NAME "db_test"
#define DB_SETTING_TABLE_NAME "setting_table"
#define DB_TODO_TABLE_NAME "todo_table"
#define DB_TODO_OP_TABLE_NAME "todo_op_table"


// 定义错误码
enum ErrorCode {
    NO_ERROR = 0,

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
    OpType_add = '1',
    OpType_del = '2',
    OpType_alt = '3',
    OpType_chk = '4',
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

    uint32_t font_size; // 字体大小配置
    uint32_t tran_pos;  // 透明度记录

    // 颜色名称记录
    std::string font_color;
    std::string icon_color;

    // 窗体定位信息
    uint32_t    x_pos;
    uint32_t    y_pos;
    uint32_t    height;
    uint32_t    width;

    NoteSetting();
};
std::string bool_str(bool val);
bool str_bool(const std::string &val);



/* 获取MS时间 -3 */
static uint64_t get_time_of_ms()
{
    struct timeval tv{};
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * (uint64_t)1000 + tv.tv_usec / 1000;
}

#endif //INTER_VAR_H
