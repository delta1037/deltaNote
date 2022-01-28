#ifndef INTER_VAR_H
#define INTER_VAR_H
#include <ctime>
#include <sys/time.h>
#include <string>

enum SyncStatus {
    Sync_login_passwd_error = 1,
    Sync_login_user_not_exits = 2,
    Sync_login_success = 3,
    Sync_login_undefined_error = 4,

    Sync_sign_up_user_exists = 8,
    Sync_sign_up_success = 9,
    Sync_sign_up_undefined_error = 10,

    Sync_user_id_null,

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

#define RET_SUCCESS (0)
#define RET_FAILED (-1)

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

enum TagType {
    TagType_nul,
    TagType_low,
    TagType_mid,
    TagType_high
};
std::string tag_type_str(TagType tag_type);
TagType tag_type_enum(const std::string &tag_type);

#define REMINDER_FORMAT "yyyy-MM-dd hh:mm:ss"

int time_int_s(std::string s_time);

struct TodoItem{
    std::string create_key;
    std::string edit_key;
    OpType op_type;
    IsCheck is_check;
    TagType tag_type;
    std::string reminder; // 题型日期
    std::string data;
    TodoItem();
    TodoItem(
        const std::string &create_key,
        const std::string &edit_key,
        OpType op_type,
        IsCheck is_check,
        TagType tag_type,
        const std::string &reminder,
        const std::string &data
    );
};
bool check_item_valid(const TodoItem &item);

#define TodoList std::list<struct TodoItem>
#define TODO_ITEM_CREATE_KEY    "create_key"
#define TODO_ITEM_EDIT_KEY      "edit_key"
#define TODO_ITEM_OP_TYPE       "op_type"
#define TODO_ITEM_IS_CHECK      "is_check"
#define TODO_ITEM_TAG_TYPE      "tag_type"
#define TODO_ITEM_REMINDER      "reminder"
#define TODO_ITEM_DATA          "data"

#define SYNC_SIGN_IN    "/sign_in"
#define SYNC_SIGN_UP    "/sign_up"
#define SYNC_UPLOAD     "/upload"
#define SYNC_DOWNLOAD   "/download"


std::string get_time_key();
uint64_t get_time_of_ms();
uint64_t get_time_of_s();

#endif //INTER_VAR_H
