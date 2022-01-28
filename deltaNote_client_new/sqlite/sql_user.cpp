#include "sql_user.h"
#include "log.h"

UserItem::UserItem(std::string &username, std::string &password, std::string &token) {
    this->username = username;
    this->password = password;
    this->token = token;
}

SqlUser::SqlUser(const std::string &db_name, const std::string &table_name) {
    this->db_name = db_name;
    this->table_name = table_name;

    this->sql_base = new SqlBase(this->db_name);
    if(RET_SUCCESS != init_table()){
        d_sql_error("%s", "init table error!!");
    }
}

SqlUser::~SqlUser() {
    delete sql_base;
}

int SqlUser::init_table() {
    static const std::string create_table_sql =
            "CREATE TABLE IF NOT EXISTS %Q ("
                "username VARCHAR(64) PRIMARY KEY, "
                "password VARCHAR(128) NOT NULL,"
                "token VARCHAR(128)"
                ");";
    SqlRetList sql_ret_list;
    ErrorCode error_code = CODE_NO_ERROR;
    int ret = sql_base->exec(
            sqlite3_mprintf(create_table_sql.c_str(), table_name.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlUser %s exec %s error", db_name.c_str(), create_table_sql.c_str())
        return RET_FAILED;
    }else{
        d_sql_debug("SqlUser %s exec %s success", db_name.c_str(), create_table_sql.c_str())
        return RET_SUCCESS;
    }
}

void SqlUser::turn_to_struct(const SqlRetList &sql_ret_list, UserList &ret_struct_list) {
    for(auto ret_it : sql_ret_list){
        ret_struct_list.emplace_back(UserItem(
                (ret_it)["username"],
                (ret_it)["password"],
                (ret_it)["token"]));
        d_sql_debug("username:%s,password:%s, token:%s",
                    (ret_it)["username"].c_str(),
                    (ret_it)["password"].c_str(),
                    (ret_it)["token"].c_str())
    }
}

int SqlUser::add(const std::string &username, const std::string &password, ErrorCode &error_code) {
    static const std::string add_sql =
            "INSERT OR REPLACE INTO %Q (username, password) VALUES (%Q, %Q)";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(add_sql.c_str(), table_name.c_str(), username.c_str(), password.c_str()),
            sql_ret_list,
            error_code
            );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlUser %s exec %s error", db_name.c_str(), add_sql.c_str())
        return RET_FAILED;
    }else{
        d_sql_debug("SqlUser %s exec %s success", db_name.c_str(), add_sql.c_str())
        return RET_SUCCESS;
    }
}

int SqlUser::del(const std::string &username, ErrorCode &error_code) {
    static const std::string add_sql =
            "DELETE from %Q WHERE username == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(add_sql.c_str(), table_name.c_str(), username.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlUser %s exec %s error", db_name.c_str(), add_sql.c_str())
        return RET_FAILED;
    }else{
        d_sql_debug("SqlUser %s exec %s success", db_name.c_str(), add_sql.c_str())
        return RET_SUCCESS;
    }
}

int SqlUser::sel(const std::string &username, std::string &password, ErrorCode &error_code) {
    static const std::string sel_sql =
            "SELECT username, password FROM %Q WHERE username == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(sel_sql.c_str(), table_name.c_str(), username.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlUser %s exec %s error", db_name.c_str(), sel_sql.c_str())
        return RET_FAILED;
    }
    d_sql_debug("SqlUser %s exec %s success", db_name.c_str(), sel_sql.c_str())
    // 获取数值
    if(sql_ret_list.size() != 1){
        d_sql_warn("ret list size = %d, username=%s", sql_ret_list.size(), username.c_str())
    }
    // 只取第一行的值
    if(!sql_ret_list.empty()){
        password = (*sql_ret_list.begin())["password"];
    }
    return RET_SUCCESS;
}

int SqlUser::sel(UserList &key_value_list, ErrorCode &error_code) {
    static const std::string sel_sql =
            "SELECT username, password, token FROM %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(sel_sql.c_str(), table_name.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlUser %s exec %s error", db_name.c_str(), sel_sql.c_str())
        return RET_FAILED;
    }
    d_sql_debug("SqlUser %s exec %s success", db_name.c_str(), sel_sql.c_str())
    turn_to_struct(sql_ret_list, key_value_list);
    return RET_SUCCESS;
}

int SqlUser::alt(const std::string &key, const std::string &value, ErrorCode &error_code) {
    static const std::string alt_sql =
            "UPDATE %Q SET value = %Q WHERE key == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(alt_sql.c_str(), table_name.c_str(), value.c_str(), key.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlUser %s exec %s error", db_name.c_str(), alt_sql.c_str())
        return RET_FAILED;
    }else{
        d_sql_debug("SqlUser %s exec %s success", db_name.c_str(), alt_sql.c_str())
        return RET_SUCCESS;
    }
}
