//
// Created by geniusrabbit on 18-2-27.
//

#include <cstdio>
#include "../include/sqlite.h"
#include "../include/Log.h"

namespace deltaNote{
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

// user change table
const char *SQL_CREATE_USER_CHANGE_TABLE = "CREATE TABLE %Q (opTimestamp VARCHAR(32) NOT NULL, createTimestamp VARCHAR(32) NOT NULL, op VARCHAR(1) NOT NULL, isCheck VARCHAR(1) NOT NULL, data TEXT NOT NULL)";
// insert change
const char *SQL_USER_CHANGE_TABLE_INSERT = "INSERT INTO %Q (opTimestamp, createTimestamp, op, isCheck, data) VALUES (%Q, %Q, %Q, %Q, %Q)";
// select change
const char *SQL_USER_CHANGE_TABLE_SELECT = "SELECT opTimestamp, createTimestamp, op, isCheck, data from %Q WHERE opTimestamp > %Q";

vector<OpChange *> ServerSqlite::_resChange;
vector<pair<char*, char*>> ServerSqlite::_resUserPasswd;

int ServerSqlite::changeTableCallback(void *data, int argc, char **argv, char **ColName){
    if (CALLBACK_ARGC_5 == argc){
        auto *op_group = new OpChange(argv[0], argv[1], argv[2][0], argv[3][0], argv[4]);
        ServerSqlite::_resChange.push_back(op_group);
    } else {
        LOG_ERROR("change table callback error")
    }

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

ServerSqlite::ServerSqlite(const char *databaseName, const char *usersTableName, const char *userName, const char *passwd) {
    g_databaseName = databaseName;
    g_usersTableName = usersTableName;

    _userName = userName;
    _passwd = passwd;
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
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_CREATE_USER_CHANGE_TABLE, _userName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("CREATE USER TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

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

SqliteState ServerSqlite::addChange(std::vector<deltaNote::OpChange *> &changeArr) {
    char opArr[2];
    opArr[1] = '\0';
    char isCheckArr[2];
    isCheckArr[1] = '\0';

    for(auto & i : changeArr){
        opArr[0] = i->op;
        isCheckArr[0] = i->isCheck;
        ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_CHANGE_TABLE_INSERT, _userName, i->opTimestamp, i->createTimestamp, opArr, isCheckArr, i->data), nullptr, nullptr, &zErrMsg);
        CHECK(ret, SQLITE_ERROR, {LOG_ERROR("ADD CHANGE : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})
    }

    return sqliteState;
}

SqliteState ServerSqlite::synChange(char *timestamp, std::vector<deltaNote::OpChange *> &resChange) {
    // clean _resChange
    for(auto & i : _resChange){
        delete i;
    }
    _resChange.erase(_resChange.begin(), _resChange.end());

    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_CHANGE_TABLE_SELECT, _userName, timestamp), changeTableCallback, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SYN CHANGE : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    // _resChange mem need to free
    resChange = _resChange;

    return sqliteState;
}

MSG_State ServerSqlite::getSqliteOpState() {
    return sqliteOpState;
}
}
