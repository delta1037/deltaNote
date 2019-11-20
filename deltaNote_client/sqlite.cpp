#include "sqlite.h"

//
// Created by geniusrabbit on 18-2-27.
//

#include <cstdio>

#define CALLBACK_ARGC_1 1
#define CALLBACK_ARGC_2 2
#define CALLBACK_ARGC_4 4
#define CALLBACK_ARGC_5 5

// create table
const char *SQL_CREATE_CHANGE_TABLE = "CREATE TABLE IF NOT EXISTS changeTable ()";
const char *SQL_CREATE_DISPLAY_TABLE = "CREATE TABLE IF NOT EXISTS displayTable ()";

// user change table
const char *SQL_CREATE_USER_CHANGE_TABLE = "CREATE TABLE %Q (opTimestamp VARCHAR(32) NOT NULL, createTimestamp VARCHAR(32) NOT NULL, op VARCHAR(1) NOT NULL, isCheck VARCHAR(1) NOT NULL, data TEXT NOT NULL)";
// insert change
const char *SQL_USER_CHANGE_TABLE_INSERT = "INSERT INTO %Q (opTimestamp, createTimestamp, op, isCheck, data) VALUES (%Q, %Q, %Q, %Q, %Q)";
// select change
const char *SQL_USER_CHANGE_TABLE_SELECT = "SELECT opTimestamp, createTimestamp, op, isCheck, data from %Q WHERE opTimestamp > %Q";

//vector<OpChange *> ServerSqlite::_resChange;
//vector<pair<char*, char*>> ServerSqlite::_resUserPasswd;

int ServerSqlite::changeTableCallback(void *data, int argc, char **argv, char **ColName){
    if (CALLBACK_ARGC_5 == argc){
    } else {
        LOG_ERROR("change table callback error")
    }

    return 0;
}

int ServerSqlite::userPasswdTableCallback(void *data, int argc, char **argv, char **ColName){
    if (CALLBACK_ARGC_2 == argc) {
    } else {
        LOG_ERROR("user_passwd table callback error")
    }

    return 0;
}

ServerSqlite::ServerSqlite() {
    databaseName = "localDB";

    sqliteState = SqliteStopped;
    sqliteOpState = UndefinedError;

    zErrMsg = nullptr;
    ret = 0;
    db = nullptr;

    // open db
    ret = sqlite3_open(databaseName, &db);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("OPEN DATABASE SQL error : %s\n", zErrMsg) sqliteState = SqliteError; exit(-1);})

    ret = sqlite3_exec(db, SQL_CREATE_CHANGE_TABLE , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("CREATE CHANGE TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError; exit(-1);})

    ret = sqlite3_exec(db, SQL_CREATE_DISPLAY_TABLE , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("CREATE DISPLAY TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError; exit(-1);})
}

SqliteState ServerSqlite::cleanSqlite() {
    return sqliteState;
}

SqliteState ServerSqlite::addChange() {

}

SqliteState ServerSqlite::synChange() {

}

MSG_State ServerSqlite::getSqliteOpState() {
    return sqliteOpState;
}
