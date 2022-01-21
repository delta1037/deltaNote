#ifndef SQL_SETTING_H
#define SQL_SETTING_H

#include <string>

#include "sql_base.h"

struct SettingItem{
    std::string name;
    std::string value;

    SettingItem(std::string &name, std::string &value);
};
#define SettingList std::list<struct SettingItem>

class SqlSetting {
public:
    SqlSetting(const std::string &db_name, const std::string &table_name);
    ~SqlSetting();

    // 增
    int add(const std::string &setting_name, const std::string &setting_value, ErrorCode &error_code);
    // 删
    int del(const std::string &setting_name, ErrorCode &error_code);
    // 查
    int sel(const std::string &setting_name, std::string &setting_value, ErrorCode &error_code);
    int sel(SettingList &setting_list, ErrorCode &error_code);
    // 改
    int alt(const std::string &setting_name, const std::string &setting_value, ErrorCode &error_code);

private:
    int init_table();
    static void turn_to_struct(const SqlRetList &sql_ret_list, SettingList &ret_struct_list);
private:
    SqlBase *sql_base;
    std::string db_name;
    std::string table_name;
};
#endif //SQL_SETTING_H
