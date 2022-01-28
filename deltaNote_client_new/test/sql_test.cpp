#include <QCoreApplication>
#include <QDebug>
#include <unistd.h>
#include "sql_key_value.h"
#include "sql_todo_list.h"
#include "log.h"

void setting_add_value(SqlSetting &sql_setting){
    ErrorCode error_code;
    sql_setting.add("name_1", "value1", error_code);
    sql_setting.add("name_2", "value2", error_code);
    sql_setting.add("name_2", "value3", error_code);
    sql_setting.add("name_3", "value3", error_code);
}

void setting_del_value(SqlSetting &sql_setting){
    ErrorCode error_code;
    sql_setting.del("name_3", error_code);
}

void setting_sel_value(SqlSetting &sql_setting){
    ErrorCode error_code;
    std::string ret_value;
    sql_setting.sel("name_2", ret_value, error_code);
    test_debug("sel %s value=%s", "name_2", ret_value.c_str())
}

void setting_alt_value(SqlSetting &sql_setting){
    ErrorCode error_code;
    sql_setting.alt("name_2", "value_2", error_code);
}

void sql_setting(){
    SqlSetting sql_setting = SqlSetting("./test_db", "setting_table_1");
    setting_add_value(sql_setting);
    setting_del_value(sql_setting);
    setting_sel_value(sql_setting);
    setting_alt_value(sql_setting);
}

void todo_add_value(SqlTodoList &sql_todo){
    ErrorCode error_code;
    //sql_todo.add(OpType_add, Check_false, "测试数据1", error_code);
    sleep(1);
    //sql_todo.add(OpType_del, Check_false, "测试数据2", error_code);
    sleep(1);
    //sql_todo.add(OpType_alt, Check_true, "测试数据3", error_code);
    sleep(1);
    //sql_todo.add(OpType_chk, Check_true, "测试数据4", error_code);
    sleep(1);
    //sql_todo.add(OpType_nul, Check_true, "测试数据5", error_code);
}

void todo_del_value(SqlTodoList &sql_todo){
    ErrorCode error_code;
    sql_todo.del("1642753704877", error_code);
}

void todo_sel_value(SqlTodoList &sql_todo){
    ErrorCode error_code;
    TodoList ret_list;
    //sql_todo.sel("1642753706925", "1642754153835", ret_list, error_code);
    //sql_todo.sel(ret_list, error_code);
    sql_todo.sel("1642753706925", OpType_add, ret_list, error_code);
}

void todo_alt_value(SqlTodoList &sql_todo){
    ErrorCode error_code;
    sql_todo.alt("1642753706925", "1642754036302", OpType_add, Check_false, "data_new_new", error_code);
}

void sql_todo_list(){
    SqlTodoList sql_todo = SqlTodoList("./test_db", "todo_table_1");
    // todo_add_value(sql_todo);
    // todo_del_value(sql_todo);
    // todo_alt_value(sql_todo);
    todo_sel_value(sql_todo);
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    //sql_setting();
    sql_todo_list();

    return QCoreApplication::exec();
}
