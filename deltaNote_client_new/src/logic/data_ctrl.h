//
// Created by delta1037 on 2022/1/21.
//

#ifndef DATA_CTRL_H
#define DATA_CTRL_H
#include <string>

#include "sql_todo_list.h"

enum ListType{
    ListType_NO,
    ListType_OP,
    ListType_UI,
};

class DataCtrl {
public:
    DataCtrl();
    ~DataCtrl();

    // 新增一条
    int add_todo(const std::string &create_key, const std::string &data, ErrorCode &error_code);
    // 修改
    int alt_todo(const std::string &create_key, IsCheck is_check, const std::string &data, ErrorCode &error_code);
    // 删除
    int del_todo(const std::string &create_key, ErrorCode &error_code);
    // 获取所有
    int sel_todo(ListType list_type, TodoList &ret_list, ErrorCode &error_code);

private:
    SqlTodoList *op_list;
    SqlTodoList *ui_list;
};
#endif //DATA_CTRL_H
