//
// Created by geniusrabbit on 18-2-27.
//

#ifndef GENIUSNOTE1_0_SQLITESERVER_H
#define GENIUSNOTE1_0_SQLITESERVER_H

#include <string>
#include <vector>
#include <cstring>
#include "sqlite3.h"
#include "untils.h"

/**
 * 一个数据库，
 * 一个用户的表，包含用户名（要求不同），密码，email等信息
 * 对于每个用户，拥有一张独立的note表（以用户名命名）
 *
 * 查询等过程：
 *  用户发过来用户名和加密的密码，查询是否符合条件
 *  符合条件即可进行下一步操作，否则认证失败
 */
using namespace std;

enum SqliteState {
    SqliteRunning=1,
    SqliteStopped=2,
    SqliteError=0
};

class ServerSqlite {
public:
    ServerSqlite(const char *databaseName, char *userName, const char *passwd);

    SqliteState cleanSqlite();

    // add new user & create new note table
    SqliteState addUser(); // add new user, Running is add success and login, other is fail

    // login system
    SqliteState loginRes(); // return Running is login other is no login

    // add change
    SqliteState addChange(std::vector<MSG_OP_PACK> &changeArr); // push change to table

    // syn change with client
    //SqliteState synChange(char *timestamp, std::vector<MSG_OP_PACK> &resChange); // pull change from table

    bool newChangeAtChangeTable(MSG_OP_PACK &msg);

    SqliteState returnDataSet(std::vector<MSG_OP_PACK> &userDataset);

    SqliteState cleanDataSet();

    SqliteState pushDataSet(std::vector<MSG_OP_PACK> &userDataset);

    MSG_State getSqliteOpState();

    static int newChangeAtChangeTableCallback(void *data, int argc, char **argv, char **ColName);

    static int retUserDatasetCallback(void *data, int argc, char **argv, char **ColName);

    static int userPasswdTableCallback(void *data, int argc, char **argv, char **ColName);
private:
    char g_databaseName[G_DATABASE_NAME_SIZE];

    char _userName[G_DATABASE_USERNAME_SIZE];
    char g_usersChangeTableName[G_DATABASE_TABLE_NAME_SIZE];
    char g_usersDatasetTableName[G_DATABASE_TABLE_NAME_SIZE];

    char _passwd[G_ARR_SIZE_PASSWD];

    SqliteState sqliteState;
    MSG_State sqliteOpState;

    sqlite3 *db;
    char *zErrMsg;
    int ret;

    static vector<MSG_OP_PACK> _retChange;
    static vector<pair<char*, char*>> _resUserPasswd;
    static vector<MSG_OP_PACK> _retDataSet;
    static int atChangeTableSize;
};

#endif //GENIUSNOTE1_0_SQLITESERVER_H
