#ifndef SQL_KEY_VALUE_H
#define SQL_KEY_VALUE_H
#include <string>
#include "sql_base.h"

struct KeyValueItem{
    std::string key;
    std::string value;

    KeyValueItem(std::string &key, std::string &value);
};
#define KeyValueList std::list<struct KeyValueItem>

class SqlKeyValue {
public:
    SqlKeyValue(const std::string &, const std::string &);
    ~SqlKeyValue();

    // 增
    int add(const std::string &, const std::string &, ErrorCode &);
    // 删
    int del(const std::string &, ErrorCode &);
    // 查
    int sel(const std::string &, std::string &, ErrorCode &);
    int sel(KeyValueList &, ErrorCode &error_code);
    // 改
    int alt(const std::string &, const std::string &, ErrorCode &);

private:
    int init_table();
    static void turn_to_struct(const SqlRetList &, KeyValueList &);
private:
    SqlBase *sql_base;
    std::string db_name;
    std::string table_name;
};
#endif //SQL_KEY_VALUE_H
