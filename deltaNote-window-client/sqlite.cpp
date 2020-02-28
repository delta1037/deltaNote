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
const char *SQL_CREATE_USER_SETTING_TABLE = "CREATE TABLE IF NOT EXISTS settingTable (setting VARCHAR(64) NOT NULL, value VARCHAR(128))";
// insert setting table
const char *SQL_USER_SETTING_TABLE_INSERT_SV = "INSERT INTO settingTable (setting, value) VALUES (%Q, %Q)";
// insert setting table
const char *SQL_USER_SETTING_TABLE_INSERT_S = "INSERT INTO settingTable (setting) VALUES (%Q)";
// select setting table
const char *SQL_USER_SETTING_TABLE_SELECT = "SELECT setting, value FROM settingTable WHERE setting == %Q";
// alter setting table
const char *SQL_USER_SETTING_TABLE_UPDATE = "UPDATE settingTable SET value = %Q WHERE setting == %Q";

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

extern char g_username[G_ARR_SIZE_USERNAME];
extern char g_passwd[G_ARR_SIZE_PASSWD];

extern char g_server[G_ARR_SIZE_SERVER];
extern int g_port;

extern bool isLogin;
extern bool isLocked;
extern bool isAutoStart;

extern QColor fontColor;
extern QColor iconColor;
extern int transparentPos;

extern int xPos;
extern int yPos;

extern int frameWigth;
extern int frameHeight;

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
        if(argv[1]){
            strcpy(_value, argv[1]);
        }
    }
    return 0;
}

ClientSqlite::ClientSqlite(const char *databaseName, const char *userName) {
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

    ret = sqlite3_exec(db, SQL_CREATE_USER_SETTING_TABLE , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_CREATE_USER_SETTING_TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError;})

    // add new user's change table
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_CREATE_USER_CHANGE_TABLE, g_usersChangeTableName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_CREATE_USER_CHANGE_TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError;})

    // add new user's dataset table
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_CREATE_USER_DATASET_TABLE, g_usersDatasetTableName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_CREATE_USER_DATASET_TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError;})
}

void ClientSqlite::makeDataPack(MSG_OP_PACK &opPack, char *opTimestamp, char *createTimestamp, char op, char isCheck, char *data){
    strcpy(opPack.createTimestamp, createTimestamp);
    strcpy(opPack.opTimestamp, opTimestamp);
    strcpy(opPack.data, data);
    opPack.op = op;
    opPack.isCheck = isCheck;
}

SqliteState ClientSqlite::initSetting(){
    // init setting
    char value[128];
    selectSettingValue("username", value);
    if(!_atSettingTable){
        insertSetting("username");
    }else if(value[0] != '\0'){
        strcpy(g_username, value);
    }

    selectSettingValue("passwd", value);
    if(!_atSettingTable){
        insertSetting("passwd");
    }else if(strcmp(value, "nullptr") != 0){
        strcpy(g_passwd, value);
    }

    selectSettingValue("server", value);
    if(!_atSettingTable){
        insertSettingValue("server", "39.96.162.190");
        strcpy(g_server, "39.96.162.190");
    }else if(value[0] != '\0'){
        strcpy(g_server, value);
    }

    selectSettingValue("port", value);
    if(!_atSettingTable){
        insertSettingValue("port", "8888");
        g_port = 8888;
    }else if(value[0] != '\0'){
        g_port = atoi(value);
    }

    selectSettingValue("fontColor", value);
    if(!_atSettingTable){
        fontColor = QColor(0, 0, 0);
        insertSettingValue("fontColor", fontColor.name().toUtf8().data());
    }else if(value[0] != '\0'){
        fontColor = QColor(value);
    }

    selectSettingValue("iconColor", value);
    if(!_atSettingTable){
        iconColor = QColor(0, 0, 0);
        insertSettingValue("iconColor", iconColor.name().toUtf8().data());
    }else if(value[0] != '\0'){
        iconColor = QColor(value);
    }

    selectSettingValue("transparentPos", value);
    if(!_atSettingTable){
        insertSettingValue("transparentPos", "30");
        transparentPos = 30;
    }else if(value[0] != '\0'){
        transparentPos = atoi(value);
    }

    selectSettingValue("xPos", value);
    if(!_atSettingTable){
        insertSettingValue("xPos", "600");
        xPos = 600;
    }else if(value[0] != '\0'){
        xPos = atoi(value);
    }

    selectSettingValue("yPos", value);
    if(!_atSettingTable){
        insertSettingValue("yPos", "300");
        yPos = 300;
    }else if(value[0] != '\0'){
        yPos = atoi(value);
    }

    selectSettingValue("frameHeight", value);
    if(!_atSettingTable){
        insertSettingValue("frameHeight", "560");
        frameHeight = 560;
    }else if(value[0] != '\0'){
        frameHeight = atoi(value);
    }

    selectSettingValue("isLocked", value);
    if(!_atSettingTable){
        isLocked = false;
        insertSettingValue("isLocked", "0");
    }else{
        //LOG_INFO("lock: %s", value)
        isLocked = bool(value[0] - '0');
    }

    selectSettingValue("isAutoStart", value);
    if(!_atSettingTable){
        isAutoStart = false;
        insertSettingValue("isAutoStart", "0");
    }else{
        //LOG_INFO("start: %s", value)
        isAutoStart = bool(value[0] - '0');
    }
}

SqliteState ClientSqlite::insertSetting(const char settingName[]){
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_INSERT_S, settingName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_SETTING_TABLE_INSERT_S SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    return sqliteState;
}
SqliteState ClientSqlite::insertSettingValue(const char settingName[], const char value[]){
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_INSERT_SV, settingName, value), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_SETTING_TABLE_INSERT_SV SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    return sqliteState;
}
SqliteState ClientSqlite::alterSetting(const char settingName[], const char value[]){
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_UPDATE, value, settingName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_SETTING_TABLE_UPDATE SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    return sqliteState;
}

SqliteState ClientSqlite::selectSettingValue(const char settingName[], char value[]){
    _atSettingTable = false;
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_SELECT, settingName), retUserSetting, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_SETTING_TABLE_SELECT SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})
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

