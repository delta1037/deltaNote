
#ifndef SQLITE_H
#define SQLITE_H

#include <string>
#include <vector>
#include <cstring>
#include "untils.h"
#include "sqlite3.h"

using namespace std;

enum SqliteState {
    SqliteRunning=1,
    SqliteStopped=2,
    SqliteError=0
};

class ServerSqlite {
public:
    ServerSqlite();

    SqliteState cleanSqlite();

    // add change
    SqliteState addChange(); // push change to table

    // syn change with client
    SqliteState synChange(); // pull change from table

    MSG_State getSqliteOpState();

    static int changeTableCallback(void *data, int argc, char **argv, char **ColName);

    static int userPasswdTableCallback(void *data, int argc, char **argv, char **ColName);
private:
    const char *databaseName;

    SqliteState sqliteState;
    MSG_State sqliteOpState;

    sqlite3 *db;
    char *zErrMsg;
    int ret;
};
#endif // SQLITE_H
