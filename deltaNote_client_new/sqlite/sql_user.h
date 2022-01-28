#ifndef SQL_SETTING_H
#define SQL_SETTING_H

#include <string>

#include "sql_base.h"

struct UserItem{
    std::string username;
    std::string password;
    std::string token;

    UserItem(std::string &, std::string &, std::string &);
};
#define UserList std::list<struct UserItem>

class SqlUser {
public:
    SqlUser(const std::string &, const std::string &);
    ~SqlUser();

    // 增
    int add(const std::string &, const std::string &, ErrorCode &);
    // 删
    int del(const std::string &, ErrorCode &);
    // 查
    int sel(const std::string &, std::string &, ErrorCode &);
    int sel(UserList &, ErrorCode &error_code);
    // 改
    int alt(const std::string &, const std::string &, ErrorCode &);

private:
    int init_table();
    static void turn_to_struct(const SqlRetList &, UserList &);
private:
    SqlBase *sql_base;
    std::string db_name;
    std::string table_name;
};
#endif //SQL_SETTING_H
