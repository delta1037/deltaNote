#ifndef SQL_TODO_LIST_H
#define SQL_TODO_LIST_H
#include <string>

#include "sql_base.h"

struct TodoItem{
    std::string create_key;
    std::string edit_key;
    OpType op_type;
    IsCheck is_check;
    std::string data;
    TodoItem(const std::string &create_key, const std::string &edit_key, OpType op_type, IsCheck is_check, const std::string &data);
};

#define TodoList std::list<struct TodoItem>

class SqlTodoList {
public:
    explicit SqlTodoList(const std::string &db_name, const std::string &table_name);
    ~SqlTodoList();

    // 新增一条数据
    int add(const std::string &create_key, const std::string &edit_key, OpType op_type, IsCheck is_check, const std::string &data, ErrorCode &error_code);
    // 删除指定的数据
    int del(const std::string &create_key, const std::string &edit_key, ErrorCode &error_code);
    // 清空表
    int del(ErrorCode &error_code);
    // 查询所有数据
    int sel(TodoList &ret_list, ErrorCode &error_code);
    // 查询指定数据（创建时间+编辑时间）
    int sel(const std::string &create_key, const std::string &edit_key, TodoList &ret_list, ErrorCode &error_code);
    // 查询指定数据（创建时间+操作类型）
    int sel(const std::string &create_key, OpType op_type, TodoList &ret_list, ErrorCode &error_code);
    // 修改数据
    int alt(const std::string &create_key, const std::string &edit_key, OpType op_type, IsCheck is_check, const std::string &data, ErrorCode &error_code);

private:
    int init_table();

    static void turn_to_struct(const SqlRetList &ret_list, TodoList &ret_struct_list);

private:
    SqlBase *sql_base;
    std::string db_name;
    std::string table_name;
};


#endif //SQL_TODO_LIST_H
