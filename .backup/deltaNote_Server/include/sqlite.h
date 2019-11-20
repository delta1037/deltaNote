//
// Created by geniusrabbit on 18-2-27.
//

#ifndef GENIUSNOTE1_0_SQLITESERVER_H
#define GENIUSNOTE1_0_SQLITESERVER_H

#include <string>
#include <vector>
#include <cstring>
#include "sqlite3.h"
#include "pack.h"

/**
 * 一个数据库，
 * 一个用户的表，包含用户名（要求不同），密码，email等信息
 * 对于每个用户，拥有一张独立的note表（以用户名命名）
 *
 * 查询等过程：
 *  用户发过来用户名和加密的密码，查询是否符合条件
 *  符合条件即可进行下一步操作，否则认证失败
 */

namespace deltaNote {
using namespace std;

enum SqliteState {
    SqliteRunning=1,
    SqliteStopped=2,
    SqliteError=0
};

struct OpChange {
    char *opTimestamp;
    char *createTimestamp;
    char op;
    char isCheck;
    char *data;

    OpChange(char *_opTimestamp, char *_createTimestamp, char _op, char _isCheck, char *_data){
        opTimestamp = new char[strlen(_opTimestamp) + 1]();
        strcpy(opTimestamp, _opTimestamp);

        createTimestamp = new char[strlen(_createTimestamp) + 1]();
        strcpy(createTimestamp, _createTimestamp);

        op = _op;
        isCheck = _isCheck;

        data = new char[strlen(_data) + 1]();
        strcpy(data, _data);
    }

    ~OpChange(){
        delete []opTimestamp;
        delete []createTimestamp;
        delete []data;
    }
};

class ServerSqlite {
public:
    ServerSqlite(const char *databaseName, const char *usersTableName, const char *userName, const char *passwd);

    SqliteState cleanSqlite();

    // add new user & create new note table
    SqliteState addUser(); // add new user, Running is add success and login, other is fail

    // login system
    SqliteState loginRes(); // return Running is login other is no login

    // add change
    SqliteState addChange(vector<OpChange *> &changeArr); // push change to table

    // syn change with client
    SqliteState synChange(char *timestamp, vector<OpChange *> &resChange); // pull change from table

    MSG_State getSqliteOpState();

    static int changeTableCallback(void *data, int argc, char **argv, char **ColName);

    static int userPasswdTableCallback(void *data, int argc, char **argv, char **ColName);
private:
    const char *g_databaseName;
    const char *g_usersTableName;

    const char *_userName; // = userTableName
    const char *_passwd;

    SqliteState sqliteState;
    MSG_State sqliteOpState;

    sqlite3 *db;
    char *zErrMsg;
    int ret;

    static vector<OpChange *> _resChange;
    static vector<pair<char*, char*>> _resUserPasswd;
};
}
#endif //GENIUSNOTE1_0_SQLITESERVER_H
