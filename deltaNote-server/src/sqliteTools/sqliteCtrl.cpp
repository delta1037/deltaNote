#include "../../include/sqliteTools/sqliteCtrl.h"

char SqliteControl::serverDatabaseName[DATABASE_NAME_SIZE]{};

bool SqliteControl::initControlEnv(const char dbPath[]) {
    strcpy(serverDatabaseName, dbPath);
    return Sqlite::initDatabase(serverDatabaseName);
}

SqliteControl::SqliteControl(){
    sqlite = new Sqlite;
    sqlite->openDataBase(serverDatabaseName);
}

SqliteControl::~SqliteControl(){
    sqlite->closeDataBase();
    delete sqlite;
}

// 用户是否存在
bool SqliteControl::findUser(struct UserID &userInfo){
    char password[64];
    return sqlite->findUser(userInfo, password);
}

// 返回所有用户
void SqliteControl::findAllUser(std::vector<struct UserID> &userInfos){
    sqlite->findAllUser(userInfos);
}

// 加载指定用户的记录
void SqliteControl::loadTodoRecords(const char userName[], std::vector<struct SocketMsgOpPack> &todoRecords, bool isOpRecord){
    char tableName[DATABASE_TABLE_NAME_SIZE];

    if(isOpRecord){
        strcpy(tableName, userName);
        strcat(tableName, "OpTable");
    }else{
        strcpy(tableName, userName);
        strcat(tableName, "DataTable");
    }

    sqlite->findAllRecord(tableName, todoRecords);
}