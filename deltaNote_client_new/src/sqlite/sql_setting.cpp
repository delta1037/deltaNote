#include "sql_setting.h"
#include "log.h"

SettingItem::SettingItem(std::string &name, std::string &value) {
    this->name = name;
    this->value = value;
}

SqlSetting::SqlSetting(const std::string &db_name, const std::string &table_name) {
    this->db_name = db_name;
    this->table_name = table_name;

    this->sql_base = new SqlBase(this->db_name);
    if(RET_SUCCESS != init_table()){
        sqlite_error("%s", "init table error!!");
    }
}

SqlSetting::~SqlSetting() {
    delete sql_base;
}

int SqlSetting::init_table() {
    static const std::string create_table_sql =
            "CREATE TABLE IF NOT EXISTS %Q (name VARCHAR(64) PRIMARY Key, value VARCHAR(128))";
    SqlRetList sql_ret_list;
    ErrorCode error_code = NO_ERROR;
    int ret = sql_base->exec(
            sqlite3_mprintf(create_table_sql.c_str(), table_name.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        sqlite_error("SqlSetting %s exec %s error", db_name.c_str(), create_table_sql.c_str())
        return RET_FAILED;
    }else{
        sqlite_debug("SqlSetting %s exec %s success", db_name.c_str(), create_table_sql.c_str())
        return RET_SUCCESS;
    }
}

void SqlSetting::turn_to_struct(const SqlRetList &sql_ret_list, SettingList &ret_struct_list) {
    for(auto ret_it : sql_ret_list){
        ret_struct_list.emplace_back(SettingItem(
                (ret_it)["name"],
                (ret_it)["value"]));
        sqlite_debug("name:%s, value:%s",
                     (ret_it)["name"].c_str(),
                     (ret_it)["value"].c_str())
    }
}

int SqlSetting::add(const std::string &setting_name, const std::string &setting_value, ErrorCode &error_code) {
    static const std::string add_sql =
            "INSERT OR REPLACE INTO %Q (name, value) VALUES (%Q, %Q)";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(add_sql.c_str(), table_name.c_str(), setting_name.c_str(), setting_value.c_str()),
            sql_ret_list,
            error_code
            );
    if(ret == SQLITE_ERROR){
        sqlite_error("SqlSetting %s exec %s error", db_name.c_str(), add_sql.c_str())
        return RET_FAILED;
    }else{
        sqlite_debug("SqlSetting %s exec %s success", db_name.c_str(), add_sql.c_str())
        return RET_SUCCESS;
    }
}

int SqlSetting::del(const std::string &setting_name, ErrorCode &error_code) {
    static const std::string add_sql =
            "DELETE from %Q WHERE name == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(add_sql.c_str(), table_name.c_str(), setting_name.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        sqlite_error("SqlSetting %s exec %s error", db_name.c_str(), add_sql.c_str())
        return RET_FAILED;
    }else{
        sqlite_debug("SqlSetting %s exec %s success", db_name.c_str(), add_sql.c_str())
        return RET_SUCCESS;
    }
}

int SqlSetting::sel(const std::string &setting_name, std::string &setting_value, ErrorCode &error_code) {
    static const std::string sel_sql =
            "SELECT name, value FROM %Q WHERE name == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(sel_sql.c_str(), table_name.c_str(), setting_name.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        sqlite_error("SqlSetting %s exec %s error", db_name.c_str(), sel_sql.c_str())
        return RET_FAILED;
    }
    sqlite_debug("SqlSetting %s exec %s success", db_name.c_str(), sel_sql.c_str())
    // 获取数值
    if(sql_ret_list.size() != 1){
        sqlite_warn("ret list size = %d, setting_name=%s", sql_ret_list.size(), setting_name.c_str())
    }
    // 只取第一行的值
    if(!sql_ret_list.empty()){
        setting_value = (*sql_ret_list.begin())["value"];
    }
    return RET_SUCCESS;
}

int SqlSetting::sel(SettingList &setting_list, ErrorCode &error_code) {
    static const std::string sel_sql =
            "SELECT name, value FROM %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(sel_sql.c_str(), table_name.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        sqlite_error("SqlSetting %s exec %s error", db_name.c_str(), sel_sql.c_str())
        return RET_FAILED;
    }
    sqlite_debug("SqlSetting %s exec %s success", db_name.c_str(), sel_sql.c_str())
    turn_to_struct(sql_ret_list, setting_list);
    return RET_SUCCESS;
}

int SqlSetting::alt(const std::string &setting_name, const std::string &setting_value, ErrorCode &error_code) {
    static const std::string alt_sql =
            "UPDATE %Q SET value = %Q WHERE name == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(alt_sql.c_str(), table_name.c_str(), setting_value.c_str(), setting_name.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        sqlite_error("SqlSetting %s exec %s error", db_name.c_str(), alt_sql.c_str())
        return RET_FAILED;
    }else{
        sqlite_debug("SqlSetting %s exec %s success", db_name.c_str(), alt_sql.c_str())
        return RET_SUCCESS;
    }
}
