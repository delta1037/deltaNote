#include "log.h"
#include "sql_key_value.h"

KeyValueItem::KeyValueItem(std::string &key, std::string &value) {
    this->key = key;
    this->value = value;
}

SqlKeyValue::SqlKeyValue(const std::string &db_name, const std::string &table_name) {
    this->db_name = db_name;
    this->table_name = table_name;

    this->sql_base = new SqlBase(this->db_name);
    if(RET_SUCCESS != init_table()){
        d_sql_error("%s", "init table error!!");
    }
}

SqlKeyValue::~SqlKeyValue() {
    delete sql_base;
}

int SqlKeyValue::init_table() {
    static const std::string create_table_sql =
            "CREATE TABLE IF NOT EXISTS %Q (key VARCHAR(64) PRIMARY Key, value VARCHAR(128))";
    SqlRetList sql_ret_list;
    ErrorCode error_code = CODE_NO_ERROR;
    int ret = sql_base->exec(
            sqlite3_mprintf(create_table_sql.c_str(), table_name.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlSetting %s exec %s error", db_name.c_str(), create_table_sql.c_str())
        return RET_FAILED;
    }else{
        d_sql_debug("SqlSetting %s exec %s success", db_name.c_str(), create_table_sql.c_str())
        return RET_SUCCESS;
    }
}

void SqlKeyValue::turn_to_struct(const SqlRetList &sql_ret_list, KeyValueList &ret_struct_list) {
    for(auto ret_it : sql_ret_list){
        ret_struct_list.emplace_back(KeyValueItem(
                (ret_it)["key"],
                (ret_it)["value"]));
        d_sql_debug("key:%s, value:%s",
                    (ret_it)["key"].c_str(),
                    (ret_it)["value"].c_str())
    }
}

int SqlKeyValue::add(const std::string &key, const std::string &value, ErrorCode &error_code) {
    static const std::string add_sql =
            "INSERT OR REPLACE INTO %Q (key, value) VALUES (%Q, %Q)";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(add_sql.c_str(), table_name.c_str(), key.c_str(), value.c_str()),
            sql_ret_list,
            error_code
            );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlSetting %s exec %s error", db_name.c_str(), add_sql.c_str())
        return RET_FAILED;
    }else{
        d_sql_debug("SqlSetting %s exec %s success", db_name.c_str(), add_sql.c_str())
        return RET_SUCCESS;
    }
}

int SqlKeyValue::del(const std::string &key, ErrorCode &error_code) {
    static const std::string add_sql =
            "DELETE from %Q WHERE key == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(add_sql.c_str(), table_name.c_str(), key.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlSetting %s exec %s error", db_name.c_str(), add_sql.c_str())
        return RET_FAILED;
    }else{
        d_sql_debug("SqlSetting %s exec %s success", db_name.c_str(), add_sql.c_str())
        return RET_SUCCESS;
    }
}

int SqlKeyValue::sel(const std::string &key, std::string &value, ErrorCode &error_code) {
    static const std::string sel_sql =
            "SELECT key, value FROM %Q WHERE key == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(sel_sql.c_str(), table_name.c_str(), key.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlSetting %s exec %s error", db_name.c_str(), sel_sql.c_str())
        return RET_FAILED;
    }
    d_sql_debug("SqlSetting %s exec %s success", db_name.c_str(), sel_sql.c_str())
    // 获取数值
    if(sql_ret_list.size() != 1){
        d_sql_warn("ret list size = %d, key=%s", sql_ret_list.size(), key.c_str())
    }
    // 只取第一行的值
    if(!sql_ret_list.empty()){
        value = (*sql_ret_list.begin())["value"];
    }
    return RET_SUCCESS;
}

int SqlKeyValue::sel(KeyValueList &key_value_list, ErrorCode &error_code) {
    static const std::string sel_sql =
            "SELECT key, value FROM %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(sel_sql.c_str(), table_name.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlSetting %s exec %s error", db_name.c_str(), sel_sql.c_str())
        return RET_FAILED;
    }
    d_sql_debug("SqlSetting %s exec %s success", db_name.c_str(), sel_sql.c_str())
    turn_to_struct(sql_ret_list, key_value_list);
    return RET_SUCCESS;
}

int SqlKeyValue::alt(const std::string &key, const std::string &value, ErrorCode &error_code) {
    static const std::string alt_sql =
            "UPDATE %Q SET value = %Q WHERE key == %Q";
    SqlRetList sql_ret_list;
    int ret = sql_base->exec(
            sqlite3_mprintf(alt_sql.c_str(), table_name.c_str(), value.c_str(), key.c_str()),
            sql_ret_list,
            error_code
    );
    if(ret == SQLITE_ERROR){
        d_sql_error("SqlSetting %s exec %s error", db_name.c_str(), alt_sql.c_str())
        return RET_FAILED;
    }else{
        d_sql_debug("SqlSetting %s exec %s success", db_name.c_str(), alt_sql.c_str())
        return RET_SUCCESS;
    }
}
