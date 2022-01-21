#include "sql_todo_list.h"
#include "log.h"

using namespace std;

TodoItem::TodoItem(const std::string &create_key, const std::string &edit_key, OpType op_type, IsCheck is_check, const std::string &data) {
    this->create_key = create_key;
    this->edit_key = edit_key;
    this->op_type = op_type;
    this->is_check = is_check;
    this->data = data;
}

SqlTodoList::SqlTodoList(const std::string &db_name, const std::string &table_name) {
    this->db_name = db_name;
    this->table_name = table_name;

    this->sql_base = new SqlBase(this->db_name);
    if(RET_SUCCESS != init_table()){
        sqlite_error("%s", "init table error!!");
    }
}

SqlTodoList::~SqlTodoList() {
    delete sql_base;
}

int SqlTodoList::init_table() {
    static const std::string create_table_sql =
            "CREATE TABLE IF NOT EXISTS %Q ("\
                "create_key VARCHAR(32) PRIMARY Key, "\
                "edit_key VARCHAR(32) NOT NULL, "\
                "op_type VARCHAR(8) NOT NULL, "\
                "is_check VARCHAR(8) NOT NULL, "\
                "data VARCHAR(128) NOT NULL"\
            ");";
    SqlRetList sql_ret_list;
    ErrorCode error_code = NO_ERROR;
    int ret = sql_base->exec(
            sqlite3_mprintf(create_table_sql.c_str(), table_name.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        sqlite_error("SqlTodoList %s exec %s error", db_name.c_str(), create_table_sql.c_str())
        return RET_FAILED;
    }else{
        sqlite_debug("SqlTodoList %s exec %s success", db_name.c_str(), create_table_sql.c_str())
        return RET_SUCCESS;
    }
}

void SqlTodoList::turn_to_struct(const SqlRetList &sql_ret_list, TodoList &ret_struct_list) {
    for(auto ret_it : sql_ret_list){
        ret_struct_list.emplace_back(
                (ret_it)["create_key"],
                (ret_it)["edit_key"],
                op_type_enum((ret_it)["op_type"]),
                is_check_enum((ret_it)["is_check"]),
                (ret_it)["data"]);
        sqlite_debug("create_key:%s, edit_key:%s, op_type:%s, is_check:%s, data:%s",
                     (ret_it)["create_key"].c_str(),
                     (ret_it)["edit_key"].c_str(),
                     (ret_it)["op_type"].c_str(),
                     (ret_it)["is_check"].c_str(),
                     (ret_it)["data"].c_str())
    }
}

int SqlTodoList::add(const std::string &create_key, const std::string &edit_key, OpType op_type, IsCheck is_check, const std::string &data, ErrorCode &error_code) {
    // 生成时间戳
    // string time_key = std::to_string(get_time_of_ms());

    static const std::string add_sql =
            "INSERT INTO %Q (create_key, edit_key, op_type, is_check, data) "\
                "VALUES (%Q, %Q, %Q, %Q, %Q)";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(
                    add_sql.c_str(),
                    table_name.c_str(),
                    create_key.c_str(),
                    edit_key.c_str(),
                    op_type_str(op_type).c_str(),
                    is_check_str(is_check).c_str(),
                    data.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        sqlite_error("SqlTodoList %s exec %s error", db_name.c_str(), add_sql.c_str())
        return RET_FAILED;
    }else{
        sqlite_debug("SqlTodoList %s exec %s success", db_name.c_str(), add_sql.c_str())
        return RET_SUCCESS;
    }
}

int SqlTodoList::del(const std::string &create_key, const std::string &edit_key, ErrorCode &error_code) {
    static const std::string del_sql =
            "DELETE from %Q WHERE create_key == %Q AND edit_key == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(
                    del_sql.c_str(),
                    table_name.c_str(),
                    create_key.c_str(),
                    edit_key.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        sqlite_error("SqlTodoList %s exec %s error", db_name.c_str(), del_sql.c_str())
        return RET_FAILED;
    }else{
        sqlite_debug("SqlTodoList %s exec %s success", db_name.c_str(), del_sql.c_str())
        return RET_SUCCESS;
    }
}

int SqlTodoList::del(ErrorCode &error_code) {
    static const std::string del_sql =
            "DELETE from %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(
                    del_sql.c_str(),
                    table_name.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        sqlite_error("SqlTodoList %s exec %s error", db_name.c_str(), del_sql.c_str())
        return RET_FAILED;
    }else{
        sqlite_debug("SqlTodoList %s exec %s success", db_name.c_str(), del_sql.c_str())
        return RET_SUCCESS;
    }
}

int SqlTodoList::sel(TodoList &ret_list, ErrorCode &error_code) {
    static const std::string sel_sql =
            "SELECT create_key, edit_key, op_type, is_check, data from %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(
                    sel_sql.c_str(),
                    table_name.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        sqlite_error("SqlTodoList %s exec %s error", db_name.c_str(), sel_sql.c_str())
        return RET_FAILED;
    }
    sqlite_debug("SqlTodoList %s exec %s success", db_name.c_str(), sel_sql.c_str())
    turn_to_struct(sql_ret_list, ret_list);
    return RET_SUCCESS;
}

int SqlTodoList::sel(const std::string &create_key, const std::string &edit_key, TodoList &ret_list, ErrorCode &error_code) {
    static const std::string sel_sql =
            "SELECT create_key, edit_key, op_type, is_check, data from %Q WHERE create_key == %Q and edit_key == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(
                    sel_sql.c_str(),
                    table_name.c_str(),
                    create_key.c_str(),
                    edit_key.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        sqlite_error("SqlTodoList %s exec %s error", db_name.c_str(), sel_sql.c_str())
        return RET_FAILED;
    }
    sqlite_debug("SqlTodoList %s exec %s success", db_name.c_str(), sel_sql.c_str())
    turn_to_struct(sql_ret_list, ret_list);
    return RET_SUCCESS;
}

int SqlTodoList::sel(const std::string &create_key, OpType op_type, TodoList &ret_list, ErrorCode &error_code) {
    static const std::string sel_sql =
            "SELECT create_key, edit_key, op_type, is_check, data from %Q WHERE create_key == %Q and op_type == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(
                    sel_sql.c_str(),
                    table_name.c_str(),
                    create_key.c_str(),
                    op_type_str(op_type).c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        sqlite_error("SqlTodoList %s exec %s error", db_name.c_str(), sel_sql.c_str())
        return RET_FAILED;
    }
    sqlite_debug("SqlTodoList %s exec %s success", db_name.c_str(), sel_sql.c_str())
    turn_to_struct(sql_ret_list, ret_list);
    return RET_SUCCESS;
}

int SqlTodoList::alt(const std::string &create_key, const std::string &edit_key, OpType op_type, IsCheck is_check, const std::string &data, ErrorCode &error_code) {
    // 生成时间戳
    string time_key = std::to_string(get_time_of_ms());

    static const std::string alt_sql =
            "UPDATE %Q SET edit_key = %Q, op_type = %Q, is_check = %Q, data = %Q "\
                "WHERE create_key == %Q and edit_key == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(
                    alt_sql.c_str(),
                    table_name.c_str(),
                    time_key.c_str(),
                    op_type_str(op_type).c_str(),
                    is_check_str(is_check).c_str(),
                    data.c_str(),
                    create_key.c_str(),
                    edit_key.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        sqlite_error("SqlTodoList %s exec %s error", db_name.c_str(), alt_sql.c_str())
        return RET_FAILED;
    }else{
        sqlite_debug("SqlTodoList %s exec %s success", db_name.c_str(), alt_sql.c_str())
        return RET_SUCCESS;
    }
}
