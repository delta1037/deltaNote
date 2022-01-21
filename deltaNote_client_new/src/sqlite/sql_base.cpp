#include "sql_base.h"
#include "log.h"

using namespace std;

sqlite3 *SqlBase::db_handle = nullptr;
SqlRetList SqlBase::sql_ret_list;
SqlBase::SqlBase(const std::string &db_name){
    db_handle = nullptr;
    this->db_name = db_name;
    this->db_err_msg = nullptr;

    ErrorCode error_code = NO_ERROR;
    if (RET_FAILED == open_db(error_code)){
        // 打开失败，重置变量
        db_handle = nullptr;
        this->db_err_msg = nullptr;
    }
}

SqlBase::~SqlBase(){
    ErrorCode error_code = NO_ERROR;
    close_db(error_code);
}

bool SqlBase::check_status(){
    if(db_handle == nullptr){
        return false;
    }
    return true;
}

int SqlBase::open_db(ErrorCode &error_code){
    int ret = sqlite3_open(db_name.c_str(), &db_handle);
    if(ret == SQLITE_ERROR){
        sqlite_error("open db %s error: %s", db_name.c_str(), db_err_msg)
        return RET_FAILED;
    }else{
        sqlite_debug("open db %s success", db_name.c_str())
        return RET_SUCCESS;
    }
}

int SqlBase::close_db(ErrorCode &error_code){
    if(db_handle != nullptr){
        sqlite3_close(db_handle);
        sqlite_debug("close db %s success", db_name.c_str())
    }
    return RET_SUCCESS;
}

int SqlBase::exec_callback(void *param, int col_count, char **col_val, char **col_name) {
    // 向静态变量中填充数据
    sqlite_debug("run %s get data", (const char*)param)
    map<string, string> one_line;
    for(int i = 0; i < col_count; i++){
        sqlite_debug("name:%s,value:%s", col_name[i], col_val[i])
        one_line[string(col_name[i])] = string(col_val[i]);
    }
    sql_ret_list.push_back(one_line);
    sqlite_debug("data collect rows %d", sql_ret_list.size())
    return 0;
}

int SqlBase::exec(const std::string &sql, SqlRetList &ret_list, ErrorCode &error_code) {
    int ret = sqlite3_exec(db_handle, sql.c_str(), (SqlCallback)&(SqlBase::exec_callback), (void *)sql.c_str(), &db_err_msg);
    if(ret == SQLITE_ERROR){
        sqlite_error("db %s exec %s error: %s", db_name.c_str(), sql.c_str(), db_err_msg)
        return RET_FAILED;
    }else{
        sqlite_debug("db %s exec %s success", db_name.c_str(), sql.c_str())
        // 获取数据
        get_exec_data(ret_list);
        return RET_SUCCESS;
    }
}

void SqlBase::get_exec_data(SqlRetList &ret_list) {
    sqlite_debug("get data from %s", this->db_name.c_str());
    for(const auto& list_it : sql_ret_list){
        ret_list.push_back(list_it);
    }
    sql_ret_list.clear();
}


