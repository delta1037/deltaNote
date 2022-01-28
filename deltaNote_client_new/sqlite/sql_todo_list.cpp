#include "sql_todo_list.h"
#include "log.h"

using namespace std;

SqlTodoList::SqlTodoList(const std::string &db_name, const std::string &table_name) {
    this->db_name = db_name;
    this->table_name = table_name;

    this->sql_base = new SqlBase(this->db_name);
    if(RET_SUCCESS != init_table()){
        d_sql_error("%s", "init table error!!");
    }
}

SqlTodoList::~SqlTodoList() {
    delete sql_base;
}

int SqlTodoList::init_table() {
    static const std::string create_table_sql =
            "CREATE TABLE IF NOT EXISTS %Q ("\
                "create_key VARCHAR(32) NOT NULL, "\
                "edit_key VARCHAR(32) PRIMARY Key, "\
                "op_type VARCHAR(8) NOT NULL, "\
                "is_check VARCHAR(8) NOT NULL, "\
                "tag_type VARCHAR(8) NOT NULL, " \
                "reminder VARCHAR(32) NOT NULL, "\
                "data VARCHAR(128) NOT NULL"\
            ");";
    SqlRetList sql_ret_list;
    ErrorCode error_code = CODE_NO_ERROR;
    int ret = sql_base->exec(
            sqlite3_mprintf(create_table_sql.c_str(), table_name.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlTodoList %s exec %s error", db_name.c_str(), create_table_sql.c_str())
        return RET_FAILED;
    }else{
        d_sql_debug("SqlTodoList %s exec %s success", db_name.c_str(), create_table_sql.c_str())
        return RET_SUCCESS;
    }
}

void SqlTodoList::turn_to_struct(const SqlRetList &sql_ret_list, TodoList &ret_struct_list) {
    for(auto ret_it : sql_ret_list){
        ret_struct_list.emplace_back(
                (ret_it)[TODO_ITEM_CREATE_KEY],
                (ret_it)[TODO_ITEM_EDIT_KEY],
                op_type_enum((ret_it)[TODO_ITEM_OP_TYPE]),
                is_check_enum((ret_it)[TODO_ITEM_IS_CHECK]),
                tag_type_enum((ret_it)[TODO_ITEM_TAG_TYPE]),
                (ret_it)[TODO_ITEM_REMINDER],
                (ret_it)[TODO_ITEM_DATA]);
        d_sql_debug("create_key:%s, edit_key:%s, op_type:%s, is_check:%s, tag_type:%s, reminder:%s, data:%s",
                    (ret_it)[TODO_ITEM_CREATE_KEY].c_str(),
                    (ret_it)[TODO_ITEM_EDIT_KEY].c_str(),
                    (ret_it)[TODO_ITEM_OP_TYPE].c_str(),
                    (ret_it)[TODO_ITEM_IS_CHECK].c_str(),
                    (ret_it)[TODO_ITEM_TAG_TYPE].c_str(),
                    (ret_it)[TODO_ITEM_REMINDER].c_str(),
                    (ret_it)[TODO_ITEM_DATA].c_str())
    }
}

int SqlTodoList::add(
        const std::string &create_key,
        const std::string &edit_key,
        OpType op_type,
        IsCheck is_check,
        TagType tag_type,
        const std::string &reminder,
        const std::string &data,
        ErrorCode &error_code) {
    // 生成时间戳
    // string time_key = std::to_string(get_time_of_ms());

    static const std::string add_sql =
            "INSERT INTO %Q (create_key, edit_key, op_type, is_check, tag_type, reminder, data) "\
                "VALUES (%Q, %Q, %Q, %Q, %Q, %Q, %Q)";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(
                    add_sql.c_str(),
                    table_name.c_str(),
                    create_key.c_str(),
                    edit_key.c_str(),
                    op_type_str(op_type).c_str(),
                    is_check_str(is_check).c_str(),
                    tag_type_str(tag_type).c_str(),
                    reminder.c_str(),
                    data.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlTodoList %s exec %s error", db_name.c_str(), add_sql.c_str())
        return RET_FAILED;
    }else{
        d_sql_debug("SqlTodoList %s exec %s success", db_name.c_str(), add_sql.c_str())
        return RET_SUCCESS;
    }
}

int SqlTodoList::del(const std::string &create_key, ErrorCode &error_code) {
    static const std::string del_sql =
            "DELETE from %Q WHERE create_key == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(
                    del_sql.c_str(),
                    table_name.c_str(),
                    create_key.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlTodoList %s exec %s error", db_name.c_str(), del_sql.c_str())
        return RET_FAILED;
    }else{
        d_sql_debug("SqlTodoList %s exec %s success", db_name.c_str(), del_sql.c_str())
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
        d_sql_error("SqlTodoList %s exec %s error", db_name.c_str(), del_sql.c_str())
        return RET_FAILED;
    }else{
        d_sql_debug("SqlTodoList %s exec %s success", db_name.c_str(), del_sql.c_str())
        return RET_SUCCESS;
    }
}

// 获取所有数据时带有规则排序
int SqlTodoList::sel(TodoList &ret_list, ErrorCode &error_code) {
    static const std::string sel_sql =
            "SELECT create_key, edit_key, op_type, is_check, tag_type, reminder, data from %Q  ORDER BY tag_type, create_key ASC";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(
                    sel_sql.c_str(),
                    table_name.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlTodoList %s exec %s error", db_name.c_str(), sel_sql.c_str())
        return RET_FAILED;
    }
    d_sql_debug("SqlTodoList %s exec %s success", db_name.c_str(), sel_sql.c_str())
    turn_to_struct(sql_ret_list, ret_list);
    return RET_SUCCESS;
}

int SqlTodoList::sel(const std::string &create_key, TodoList &ret_list, ErrorCode &error_code) {
    static const std::string sel_sql =
            "SELECT create_key, edit_key, op_type, is_check, tag_type, reminder, data from %Q WHERE create_key == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(
                    sel_sql.c_str(),
                    table_name.c_str(),
                    create_key.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlTodoList %s exec %s error", db_name.c_str(), sel_sql.c_str())
        return RET_FAILED;
    }
    d_sql_debug("SqlTodoList %s exec %s success", db_name.c_str(), sel_sql.c_str())
    turn_to_struct(sql_ret_list, ret_list);
    return RET_SUCCESS;
}

int SqlTodoList::sel(const std::string &create_key, OpType op_type, TodoList &ret_list, ErrorCode &error_code) {
    static const std::string sel_sql =
            "SELECT create_key, edit_key, op_type, is_check, tag_type, reminder, data from %Q WHERE create_key == %Q and op_type == %Q;";
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
        d_sql_error("SqlTodoList %s exec %s error", db_name.c_str(), sel_sql.c_str())
        return RET_FAILED;
    }
    d_sql_debug("SqlTodoList %s exec %s success", db_name.c_str(), sel_sql.c_str())
    turn_to_struct(sql_ret_list, ret_list);
    return RET_SUCCESS;
}

// 修改数据（根据创建时间）
int SqlTodoList::alt(
        const std::string &create_key,
        const std::string &edit_key,
        OpType op_type,
        IsCheck is_check,
        TagType tag_type,
        const std::string &reminder,
        const std::string &data,
        ErrorCode &error_code) {
    static const std::string alt_sql =
            "UPDATE %Q SET edit_key = %Q, op_type == %Q, is_check = %Q, tag_type = %Q, reminder = %Q, data = %Q "\
                "WHERE create_key == %Q;";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(
                    alt_sql.c_str(),
                    table_name.c_str(),
                    edit_key.c_str(),
                    op_type_str(op_type).c_str(),
                    is_check_str(is_check).c_str(),
                    tag_type_str(tag_type).c_str(),
                    reminder.c_str(),
                    data.c_str(),
                    create_key.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlTodoList %s exec %s error", db_name.c_str(), alt_sql.c_str())
        return RET_FAILED;
    }else{
        d_sql_debug("SqlTodoList %s exec %s success", db_name.c_str(), alt_sql.c_str())
        return RET_SUCCESS;
    }
}

// 修改数据（根据创建时间+操作类型）
int SqlTodoList::alt(
        const std::string &create_key,
        OpType op_type,
        const std::string &edit_key,
        IsCheck is_check,
        TagType tag_type,
        const std::string &reminder,
        const std::string &data,
        ErrorCode &error_code){
    static const std::string alt_sql =
            "UPDATE %Q SET edit_key = %Q, is_check = %Q, tag_type = %Q, reminder = %Q, data = %Q "\
                "WHERE create_key == %Q and op_type == %Q;";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(
                    alt_sql.c_str(),
                    table_name.c_str(),
                    edit_key.c_str(),
                    is_check_str(is_check).c_str(),
                    tag_type_str(tag_type).c_str(),
                    reminder.c_str(),
                    data.c_str(),
                    create_key.c_str(),
                    op_type_str(op_type).c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlTodoList %s exec %s error", db_name.c_str(), alt_sql.c_str())
        return RET_FAILED;
    }else{
        d_sql_debug("SqlTodoList %s exec %s success", db_name.c_str(), alt_sql.c_str())
        return RET_SUCCESS;
    }
}
