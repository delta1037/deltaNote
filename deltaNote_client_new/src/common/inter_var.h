#ifndef INTER_VAR_H
#define INTER_VAR_H
#include <ctime>
#include <sys/time.h>
#include <string>

// 一些表的名字，数据库的名称定义
#define DB_NAME "db_test"
#define DB_SETTING_TABLE_NAME "setting_table"
#define DB_TODO_TABLE_NAME "todo_table"

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

#define RET_SUCCESS (0)
#define RET_FAILED (-1)

/* 获取MS时间 -3 */
static uint64_t get_time_of_ms()
{
    struct timeval tv{};
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * (uint64_t)1000 + tv.tv_usec / 1000;
}

#endif //INTER_VAR_H
