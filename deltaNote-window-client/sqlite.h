
#ifndef SQLITE_H
#define SQLITE_H

#include <string>
#include <vector>
#include <cstring>

#include "pack.h"
#include "log.h"
#include "sqlite3.h"

extern char dbPATH[PATH_SIZE];
using namespace std;

enum SqliteState {
    SqliteRunning=1,
    SqliteStopped=2,
    SqliteError=0
};

class ClientSqlite {
public:
    ClientSqlite(const char *databaseName = dbPATH, const char userName[] = "local");
    ~ClientSqlite();

    // clean local and server data
    SqliteState cleanChangeTable();
    SqliteState cleanDatasetTable();

    SqliteState insertDatasetItem(MSG_OP_PACK &item);
    SqliteState deleteDatasetItem(MSG_OP_PACK &item);

    // logout solve data
    SqliteState insertChangeItem(MSG_OP_PACK &item);

    // local data need syn
    SqliteState getLocalChange(std::vector<MSG_OP_PACK> &userDataset);

    // save dataset local, when reset get
    SqliteState getDataset(std::vector<MSG_OP_PACK> &userDataset);

    MSG_State getSqliteOpState();

    // save user setting
    SqliteState initSetting();
    SqliteState insertSetting(const char settingName[]);
    SqliteState insertSettingValue(const char settingName[], const char value[]);
    SqliteState alterSetting(const char settingName[], const char value[]);
    SqliteState selectSettingValue(const char settingName[], char value[]);

    static void makeDataPack(MSG_OP_PACK &opPack, char *opTimestamp, char *createTimestamp, char op, char isCheck, char *data);
    static int retUserSetting(void *data, int argc, char **argv, char **ColName);
    static int retUserDataset(void *data, int argc, char **argv, char **ColName);
    static int retUserChange(void *data, int argc, char **argv, char **ColName);
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

    static char _setting[G_USERDATA_SETTING_SIZE];
    static char _value[G_USERDATA_VALUE_SIZE];
    static bool _atSettingTable;
    static vector<MSG_OP_PACK> _retChange;
    static vector<MSG_OP_PACK> _retDataSet;
};

#endif // SQLITE_H
