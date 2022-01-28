#ifndef CLIENT_DATA_CTRL_H
#define CLIENT_DATA_CTRL_H
#include <string>

#include "sql_todo_list.h"

enum ListType{
    ListType_NO,
    ListType_OP,
    ListType_UI,
};

class CDataCtrl {
public:
    CDataCtrl();
    ~CDataCtrl();

    // 新增一条
    int add_todo(const TodoItem &data_item, ErrorCode &error_code);
    // 修改
    int alt_todo(const TodoItem &data_item, ErrorCode &error_code);
    // 删除
    int del_todo(const std::string &create_key, ErrorCode &error_code);
    // 获取所有
    int sel_todo(ListType list_type, TodoList &ret_list, ErrorCode &error_code);
    // 按照指定的类型和create_key获取
    int sel_todo(ListType list_type, const std::string &create_key, TodoList &ret_list, ErrorCode &error_code);
private:
    SqlTodoList *op_list;
    SqlTodoList *ui_list;
};
#endif //CLIENT_DATA_CTRL_H
