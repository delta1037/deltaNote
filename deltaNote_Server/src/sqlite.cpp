//
// Created by geniusrabbit on 18-2-27.
//

#include <cstdio>
#include "../include/sqlite.h"
#include "../include/Log.h"
#include "../include/untils.h"

#define CALLBACK_ARGC_1 1
#define CALLBACK_ARGC_2 2
#define CALLBACK_ARGC_4 4
#define CALLBACK_ARGC_5 5

// users table
const char *SQL_CREATE_TABLE_USER_PASSWD = "CREATE TABLE IF NOT EXISTS user_passwd(user_name KEY NOT NULL, passwd CHAR(256) NOT NULL)";
// add new user
const char *SQL_ADD_NEW_USER = "INSERT INTO user_passwd (user_name, passwd) VALUES(%Q, %Q)";
// choose user with username and passwd
const char *SQL_FIND_ONE_USER = "SELECT user_name, passwd From user_passwd WHERE user_name = %Q";

// user dataSet table
const char *SQL_CREATE_USER_DATASET_TABLE = "CREATE TABLE %Q (opTimestamp VARCHAR(32) NOT NULL, createTimestamp VARCHAR(32) NOT NULL, isCheck VARCHAR(1) NOT NULL, data VARCHAR(128) NOT NULL)";
// insert dataset
const char *SQL_USER_DATASET_TABLE_INSERT = "INSERT INTO %Q (opTimestamp, createTimestamp, isCheck, data) VALUES (%Q, %Q, %Q, %Q)";
// select dataset
const char *SQL_USER_DATASET_TABLE_SELECT = "SELECT opTimestamp, createTimestamp, isCheck, data from %Q";
// clean dataset
const char *SQL_USER_DATASET_TABLE_DELETE = "DELETE from %Q";

// user change table
const char *SQL_CREATE_USER_CHANGE_TABLE = "CREATE TABLE %Q (opTimestamp VARCHAR(32) NOT NULL, createTimestamp VARCHAR(32) NOT NULL, op VARCHAR(1) NOT NULL, isCheck VARCHAR(1) NOT NULL, data VARCHAR(128) NOT NULL)";
// insert change
const char *SQL_USER_CHANGE_TABLE_INSERT = "INSERT INTO %Q (opTimestamp, createTimestamp, op, isCheck, data) VALUES (%Q, %Q, %Q, %Q, %Q)";
// select change
const char *SQL_USER_CHANGE_TABLE_SELECT = "SELECT opTimestamp, createTimestamp, op, isCheck, data from %Q WHERE opTimestamp == %Q and createTimestamp == %Q";
// clean dataset
const char *SQL_USER_CHANGE_TABLE_DELETE = "DELETE from %Q";

vector<MSG_OP_PACK> ServerSqlite::_retChange;
vector<pair<char*, char*>> ServerSqlite::_resUserPasswd;
vector<MSG_OP_PACK> ServerSqlite::_retDataSet;
int ServerSqlite::atChangeTableSize;

int ServerSqlite::retUserDatasetCallback(void *data, int argc, char **argv, char **ColName){
    if (CALLBACK_ARGC_4 == argc){
        MSG_OP_PACK pack{};
        makeDataPack(pack, argv[0], argv[1], NULL_OP, argv[2][0], argv[3]);

        ServerSqlite::_retDataSet.push_back(pack);
    } else {
        LOG_ERROR("change table callback error")
    }

    return 0;
}

int ServerSqlite::newChangeAtChangeTableCallback(void *data, int argc, char **argv, char **ColName){
    atChangeTableSize++;
    return 0;
}

int ServerSqlite::userPasswdTableCallback(void *data, int argc, char **argv, char **ColName){
    if (CALLBACK_ARGC_2 == argc) {
        char *username = new char[8];
        char *passwd = new char[128];
        strcpy(username, argv[0]);
        strcpy(passwd, argv[1]);
        ServerSqlite::_resUserPasswd.emplace_back(username, passwd);
    } else {
        LOG_ERROR("user_passwd table callback error")
    }

    return 0;
}

ServerSqlite::ServerSqlite(const char *databaseName, char *userName, const char *passwd) {
    strcpy(g_databaseName, databaseName);
    strcpy(g_usersChangeTableName, userName);
    strcat(g_usersChangeTableName, "_changeTable");
    strcpy(g_usersDatasetTableName, userName);
    strcat(g_usersDatasetTableName, "_datasetTable");

    strcpy(_userName, userName);
    strcpy(_passwd, passwd);
    sqliteState = SqliteStopped;
    sqliteOpState = UndefinedError;

    zErrMsg = nullptr;
    ret = 0;
    db = nullptr;

    ret = sqlite3_open(g_databaseName, &db);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("OPEN DATABASE SQL error : %s\n", zErrMsg)})

    ret = sqlite3_exec(db, SQL_CREATE_TABLE_USER_PASSWD , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("CREATE USERS TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError; exit(-1);})
}

SqliteState ServerSqlite::cleanSqlite() {
    cleanDataSet();
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_CHANGE_TABLE_DELETE, _userName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_CHANGE_TABLE_DELETE : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})
    return sqliteState;
}

SqliteState ServerSqlite::addUser() {
    // check new user
    for(auto & i : _resUserPasswd){
        delete []i.first;
        delete []i.second;
    }
    _resUserPasswd.erase(_resUserPasswd.begin(), _resUserPasswd.end());
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_FIND_ONE_USER, _userName), userPasswdTableCallback, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SELECT USER : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    if (_resUserPasswd.empty()) {
        sqliteState = SqliteRunning;
        sqliteOpState = CreateUserSuccess;
        LOG_INFO("create user success")
    } else if (1 == _resUserPasswd.size()){
        sqliteState = SqliteStopped;
        sqliteOpState = CreateUserUserExists;
        LOG_ERROR("user %s exits", _userName)
        return sqliteState;
    } else {
        sqliteState = SqliteError;
        sqliteOpState = CreateUserUndefinedError;
        LOG_ERROR("create user error")
        return sqliteState;
    }

    // add new user
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_ADD_NEW_USER, _userName, _passwd), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("ADD USER SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    // add new user's change table
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_CREATE_USER_CHANGE_TABLE, g_usersChangeTableName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("CREATE USER TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    // add new user's dataset table
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_CREATE_USER_DATASET_TABLE, g_usersDatasetTableName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_CREATE_USER_DATASET_TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    return sqliteState;
}

SqliteState ServerSqlite::loginRes() {
    for(auto & i : _resUserPasswd){
        delete []i.first;
        delete []i.second;
    }
    _resUserPasswd.erase(_resUserPasswd.begin(), _resUserPasswd.end());
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_FIND_ONE_USER, _userName), userPasswdTableCallback, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("LOGIN SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    if (_resUserPasswd.empty()) {
        sqliteState = SqliteStopped;
        sqliteOpState = LoginUserNotExits;
        LOG_ERROR("user %s not exits", _userName)
        return sqliteState;
    } else if (1 == _resUserPasswd.size()){
        if (strcmp(_resUserPasswd[0].second, _passwd) != 0){
            sqliteState = SqliteStopped;
            sqliteOpState = LoginPasswdError;
            LOG_ERROR("user %s's passwd %s is error", _userName, _resUserPasswd[0].second)
            return sqliteState;
        }else{
            sqliteState = SqliteRunning;
            sqliteOpState = LoginSuccess;
            LOG_INFO("login success")
        }
    } else {
        sqliteState = SqliteError;
        sqliteOpState = LoginUndefinedError;
        LOG_ERROR("login undefined error")
        return sqliteState;
    }

    return sqliteState;
}

SqliteState ServerSqlite::addChange(std::vector<MSG_OP_PACK> &changeArr) {
    char *op = new char[2]();
    char *isCheck = new char[2]();
    vector<MSG_OP_PACK> userDataset;
    returnDataSet(userDataset);

    for(auto & i : changeArr){
        if(newChangeAtChangeTable(i)){
            // change is exists
            continue;
        }
        if(ADD == i.op){
            MSG_OP_PACK newData{};
            makeDataPack(newData, i.opTimestamp, i.createTimestamp, i.op, i.isCheck, i.data);
            userDataset.push_back(newData);
        } else {
            for(int dataIndex = 0; dataIndex < userDataset.size(); ++dataIndex){
                if(0 == strcmp(userDataset[dataIndex].createTimestamp, i.createTimestamp)){
                    if (DEL == i.op) {
                        userDataset.erase(userDataset.begin() + dataIndex);
                    } else if (ALTER == i.op) {
                        makeDataPack(userDataset[dataIndex], i.opTimestamp, i.createTimestamp, i.op, i.isCheck, i.data);
                    } else if (CHECK == i.op) {
                        makeDataPack(userDataset[dataIndex], i.opTimestamp, i.createTimestamp, i.op, i.isCheck, i.data);
                    } else {
                        LOG_ERROR("unknown op")
                    }
                    break;
                }
            }
        }

        // insert into change table
        op[0] = i.op;
        isCheck[0] = i.isCheck;
        ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_CHANGE_TABLE_INSERT, g_usersChangeTableName, i.opTimestamp, i.createTimestamp, op, isCheck, i.data), nullptr, nullptr, &zErrMsg);
        CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_CHANGE_TABLE_INSERT SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})
    }

    // push new data set to dataset table
    cleanDataSet();
    pushDataSet(userDataset);
    return sqliteState;
}

bool ServerSqlite::newChangeAtChangeTable(MSG_OP_PACK &msg){
    atChangeTableSize = 0;

    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_CHANGE_TABLE_SELECT, g_usersChangeTableName, msg.opTimestamp, msg.createTimestamp), newChangeAtChangeTableCallback, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_CHANGE_TABLE_SELECT : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    return atChangeTableSize >= 1;
}

SqliteState ServerSqlite::returnDataSet(std::vector<MSG_OP_PACK> &userDataset){
    // clean _retDataSet
    _retDataSet.erase(_retDataSet.begin(), _retDataSet.end());

    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATASET_TABLE_SELECT, g_usersDatasetTableName), retUserDatasetCallback, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_SELECT : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    // _retDataSet mem need to free
    for(auto i : _retDataSet){
        userDataset.push_back(i);
    }

    return sqliteState;
}

SqliteState ServerSqlite::cleanDataSet(){
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATASET_TABLE_DELETE, g_usersDatasetTableName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_DELETE : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})
    return sqliteState;
}

SqliteState ServerSqlite::pushDataSet(std::vector<MSG_OP_PACK> &userDataset){
    char *op = new char[2]();
    char *isCheck = new char[2]();

    for(auto & i : userDataset){
        op[0] = i.op;
        isCheck[0] = i.isCheck;
        ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATASET_TABLE_INSERT, g_usersDatasetTableName, i.opTimestamp, i.createTimestamp, isCheck, i.data), nullptr, nullptr, &zErrMsg);
        CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_INSERT : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})
    }
    return sqliteState;
}

MSG_State ServerSqlite::getSqliteOpState() {
    return sqliteOpState;
}