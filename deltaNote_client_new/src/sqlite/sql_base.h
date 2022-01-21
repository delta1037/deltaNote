#ifndef SQLITE_BASE_H
#define SQLITE_BASE_H

#include <string>
#include <list>
#include <map>
#include "sqlite3.h"
#include "inter_var.h"

#define SqlRetList std::list<std::map<std::string, std::string>>
typedef int (*SqlCallback)(void*,int,char**,char**);

// 数据库接口层
class SqlBase {
public:
    explicit SqlBase(const std::string &db_name);
    ~SqlBase();

    int exec(const std::string &sql, SqlRetList &ret_list, ErrorCode &error_code);

protected:
    static int exec_callback(void *data, int argc, char **argv, char **ColName);

private:
    // 创建数据库
    int open_db(ErrorCode &error_code);
    int close_db(ErrorCode &error_code);
    // 检查实例状态
    static bool check_status();
    // 从回调中获取数据
    void get_exec_data(SqlRetList &ret_list);

private:
    // 全局只有一个数据库操作对象
    static sqlite3 *db_handle;
    std::string db_name;
    char *db_err_msg;

    static SqlRetList sql_ret_list;
};


#endif //SQLITE_BASE_H
