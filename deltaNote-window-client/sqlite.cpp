#include "sqlite.h"

//
// Created by geniusrabbit on 18-2-27.
//

#include <cstdio>

#define CALLBACK_ARGC_1 1
#define CALLBACK_ARGC_2 2
#define CALLBACK_ARGC_4 4
#define CALLBACK_ARGC_5 5

// user setting table
const char *SQL_CREATE_USER_SETTING_TABLE = "CREATE TABLE IF NOT EXISTS settingTable (setting VARCHAR(32) NOT NULL, value VARCHAR(32) NOT NULL)";
// insert setting table
const char *SQL_USER_SETTING_TABLE_INSERT = "INSERT INTO settingTable (setting, value,) VALUES (%Q, %Q)";
// select setting table
const char *SQL_USER_SETTING_TABLE_SELECT = "SELECT setting, value FROM settingTable WHERE setting = %Q";
// alter setting table
const char *SQL_USER_SETTING_TABLE_UPDATE = "UPDATE settingTable SET value = %Q WHERE setting = %Q";

// user dataSet table
const char *SQL_CREATE_USER_DATASET_TABLE = "CREATE TABLE IF NOT EXISTS %Q (opTimestamp VARCHAR(32) NOT NULL, createTimestamp VARCHAR(32) NOT NULL, isCheck VARCHAR(1) NOT NULL, data VARCHAR(128) NOT NULL)";
// insert dataset
const char *SQL_USER_DATASET_TABLE_INSERT = "INSERT INTO %Q (opTimestamp, createTimestamp, isCheck, data) VALUES (%Q, %Q, %Q, %Q)";
// select dataset
const char *SQL_USER_DATASET_TABLE_SELECT = "SELECT opTimestamp, createTimestamp, isCheck, data from %Q";
// clean dataset
const char *SQL_USER_DATASET_TABLE_DELETE = "DELETE from %Q WHERE opTimestamp == %Q AND createTimestamp == %Q";

// clean dataset
const char *SQL_USER_DATASET_TABLE_CLEAN = "DELETE from %Q";

// user change table
const char *SQL_CREATE_USER_CHANGE_TABLE = "CREATE TABLE IF NOT EXISTS %Q (opTimestamp VARCHAR(32) NOT NULL, createTimestamp VARCHAR(32) NOT NULL, op VARCHAR(1) NOT NULL, isCheck VARCHAR(1) NOT NULL, data VARCHAR(128) NOT NULL)";
// insert change
const char *SQL_USER_CHANGE_TABLE_INSERT = "INSERT INTO %Q (opTimestamp, createTimestamp, op, isCheck, data) VALUES (%Q, %Q, %Q, %Q, %Q)";
// select change
const char *SQL_USER_CHANGE_TABLE_SELECT = "SELECT opTimestamp, createTimestamp, op, isCheck, data from %Q WHERE opTimestamp == %Q and createTimestamp == %Q";
// clean dataset
const char *SQL_USER_CHANGE_TABLE_DELETE = "DELETE from %Q";

vector<MSG_OP_PACK> ClientSqlite::_retChange;
vector<MSG_OP_PACK> ClientSqlite::_retDataSet;
char ClientSqlite::_setting[32];
char ClientSqlite::_value[32];
bool ClientSqlite::_atSettingTable;

int ClientSqlite::retUserDataset(void *data, int argc, char **argv, char **ColName){
    if (CALLBACK_ARGC_4 == argc){
        MSG_OP_PACK pack{};
        makeDataPack(pack, argv[0], argv[1], NULL_OP, argv[2][0], argv[3]);

        ClientSqlite::_retDataSet.push_back(pack);
    } else {
        LOG_ERROR("return data set callback error")
    }

    return 0;
}

int ClientSqlite::retUserChange(void *data, int argc, char **argv, char **ColName){
    if (CALLBACK_ARGC_5 == argc){
        MSG_OP_PACK pack{};
        makeDataPack(pack, argv[0], argv[1], argv[2][0], argv[3][0], argv[4]);

        ClientSqlite::_retChange.push_back(pack);
    } else {
        LOG_ERROR("return change set callback error")
    }

    return 0;
}

int ClientSqlite::retUserSetting(void *data, int argc, char **argv, char **ColName){
    if(argc == CALLBACK_ARGC_2){
        _atSettingTable = true;
        strcpy(_setting, argv[0]);
        strcpy(_value, argv[1]);
    }
}

ClientSqlite::ClientSqlite(const char *databaseName, char *userName) {
    strcpy(g_databaseName, databaseName);
    strcpy(g_usersChangeTableName, userName);
    strcat(g_usersChangeTableName, "_changeTable");
    strcpy(g_usersDatasetTableName, userName);
    strcat(g_usersDatasetTableName, "_datasetTable");

    strcpy(_userName, userName);

    sqliteState = SqliteStopped;
    sqliteOpState = UndefinedError;

    zErrMsg = nullptr;
    ret = 0;
    db = nullptr;

    ret = sqlite3_open(databaseName, &db);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("OPEN DATABASE SQL error : %s\n", zErrMsg)})
    /*
    ret = sqlite3_exec(db, SQL_CREATE_USER_SETTING_TABLE , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_CREATE_USER_SETTING_TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError;})
    // init setting
    initSetting();
    */
    // add new user's change table
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_CREATE_USER_CHANGE_TABLE, g_usersChangeTableName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_CREATE_USER_CHANGE_TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError;})

    // add new user's dataset table
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_CREATE_USER_DATASET_TABLE, g_usersDatasetTableName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_CREATE_USER_DATASET_TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError;})
}

SqliteState ClientSqlite::initSetting(){
    // init setting
    _atSettingTable = false;
    selectSetting("username", nullptr);
    if(!_atSettingTable){
        insertSetting("username", nullptr);
    }

    _atSettingTable = false;
    selectSetting("passwd", nullptr);
    if(!_atSettingTable){
        insertSetting("passwd", nullptr);
    }

    _atSettingTable = false;
    selectSetting("server_port", nullptr);
    if(!_atSettingTable){
        insertSetting("server_port", nullptr);
    }

    _atSettingTable = false;
    selectSetting("fontColor", nullptr);
    if(!_atSettingTable){
        insertSetting("fontColor", nullptr);
    }

    _atSettingTable = false;
    selectSetting("iconColor", nullptr);
    if(!_atSettingTable){
        insertSetting("iconColor", nullptr);
    }

    _atSettingTable = false;
    selectSetting("transparentPos", nullptr);
    if(!_atSettingTable){
        insertSetting("transparentPos", nullptr);
    }
}

SqliteState ClientSqlite::insertSetting(char *settingName, char *value){
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_INSERT, settingName, value), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_SETTING_TABLE_INSERT SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    return sqliteState;
}
SqliteState ClientSqlite::alterSetting(char *settingName, char *value){
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_UPDATE, value, settingName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_SETTING_TABLE_UPDATE SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    return sqliteState;
}
SqliteState ClientSqlite::selectSetting(char *settingName, char *value){
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATASET_TABLE_SELECT, settingName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_SELECT SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})
    if(value != nullptr){
        strcpy(value, _value);
    }
    return sqliteState;
}

SqliteState ClientSqlite::insertDatasetItem(MSG_OP_PACK &item){
    char *isCheck = new char[2]();
    isCheck[0] = item.isCheck;

    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATASET_TABLE_INSERT, g_usersDatasetTableName, item.opTimestamp, item.createTimestamp, isCheck, item.data), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_INSERT SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    return sqliteState;
}

SqliteState ClientSqlite::deleteDatasetItem(MSG_OP_PACK &item){
    if(item.opTimestamp[0] == '\0' || item.createTimestamp[0] == '\0'){
        return SqliteError;
    }
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATASET_TABLE_DELETE, g_usersDatasetTableName, item.opTimestamp, item.createTimestamp), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_DELETE SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    return sqliteState;
}

SqliteState ClientSqlite::insertChangeItem(MSG_OP_PACK &item){
    char *op = new char[2]();
    char *isCheck = new char[2]();

    op[0] = item.op;
    isCheck[0] = item.isCheck;
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_CHANGE_TABLE_INSERT, g_usersChangeTableName, item.opTimestamp, item.createTimestamp, op, isCheck, item.data), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_CHANGE_TABLE_INSERT SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    return sqliteState;
}

SqliteState ClientSqlite::getLocalChange(std::vector<MSG_OP_PACK> &userChange){
    // clean _retChange
    _retChange.erase(_retChange.begin(), _retChange.end());

    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATASET_TABLE_SELECT, g_usersChangeTableName), retUserChange, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_CHANGE_TABLE_SELECT : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    // _retChange mem need to free
    for(auto i : _retChange){
        userChange.push_back(i);
    }

    return sqliteState;
}

SqliteState ClientSqlite::getDataset(std::vector<MSG_OP_PACK> &userDataset){
    // clean _retChange
    _retDataSet.erase(_retDataSet.begin(), _retDataSet.end());

    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATASET_TABLE_SELECT, g_usersDatasetTableName), retUserDataset, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_SELECT : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    // _retChange mem need to free
    for(auto i : _retDataSet){
        userDataset.push_back(i);
    }

    return sqliteState;
}

SqliteState ClientSqlite::cleanChangeTable() {
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_CHANGE_TABLE_DELETE, _userName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_CHANGE_TABLE_DELETE : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})
    return sqliteState;
}

SqliteState ClientSqlite::cleanDatasetTable() {
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATASET_TABLE_CLEAN, g_usersDatasetTableName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_CLEAN : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    return sqliteState;
}
ClientSqlite::~ClientSqlite(){
    //sqlite3_close(db);
}

MSG_State ClientSqlite::getSqliteOpState() {
    return sqliteOpState;
}

