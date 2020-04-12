#include <cstdio>
#include "../../include/common/sqlite.h"

#define CALLBACK_ARGC_1 1
#define CALLBACK_ARGC_2 2
#define CALLBACK_ARGC_4 4
#define CALLBACK_ARGC_5 5

std::vector<struct UserID> Sqlite::userInfosCallBackValue;
std::vector<struct SocketMsgOpPack> Sqlite::RecordsCallBackValue;

// static 初始化数据库
bool Sqlite::initDatabase(const char databaseName[]){
    sqlite3 *db;
    char *zErrMsg = nullptr;

    int ret = sqlite3_open(databaseName, &db);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("OPEN DATABASE SQL error : %s\n", zErrMsg); return false;})

    ret = sqlite3_exec(db, SQL_USER_INFO_TABLE_CREATE , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("SQL_CREATE_USER_SETTING_TABLE SQL error : %s\n", zErrMsg); return false;})

    sqlite3_close(db);
    return true;
}

// call back for user
int Sqlite::retUserInfosCallBack(void *data, int argc, char **argv, char **ColName){
    if(argc == CALLBACK_ARGC_2){
        userInfosCallBackValue.emplace_back(argv[0], argv[1]);
    }else{
        LogCtrl::error("return user info callback error");
    }
    return 0;
}

// call back for records table
int Sqlite::retRecordsCallBack(void *data, int argc, char **argv, char **ColName){
    if (CALLBACK_ARGC_5 == argc){
        RecordsCallBackValue.emplace_back(argv[0], argv[1], argv[2][0], argv[3][0], argv[4]);
    } else {
        LogCtrl::error("return change set callback error");
    }
    return 0;
}

Sqlite::Sqlite() {
    zErrMsg = nullptr;
    db = nullptr;
}

Sqlite::~Sqlite()= default;

// 打开数据库
bool Sqlite::openDataBase(const char databaseName[]){
    int ret = sqlite3_open(databaseName, &db);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("OPEN DATABASE SQL error : %s\n", zErrMsg); return false;})
    return true;
}

bool Sqlite::closeDataBase(){
    return sqlite3_close(db);
}

/********************************************* User Op *************************************************/
bool Sqlite::addUser(const struct UserID &userId){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_INFO_TABLE_ADD, userId.userName, userId.passWord), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("add new user:%s SQL error : %s\n", userId.userName, zErrMsg); return false;})

    // add new user's change table
    char userLocalOpListTableName[DATABASE_TABLE_NAME_SIZE]{};
    strcpy(userLocalOpListTableName, userId.userName);
    strcat(userLocalOpListTableName, "OpTable");
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_CREATE, userLocalOpListTableName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("create databse:%s SQL error : %s\n", userLocalOpListTableName, zErrMsg); return false;})

    // add new user's dataset table
    char userLocalListTableName[DATABASE_TABLE_NAME_SIZE]{};
    strcpy(userLocalListTableName, userId.userName);
    strcat(userLocalListTableName, "DataTable");
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_CREATE, userLocalListTableName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("create databse:%s SQL error : %s\n", userLocalListTableName, zErrMsg); return false;})
    return true;
}

bool Sqlite::delUser(const struct UserID &userId){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_INFO_TABLE_DELETE, userId.userName, userId.passWord), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("add new user:%s SQL error : %s\n", userId.userName, zErrMsg); return false;})
    return true;
}

bool Sqlite::findUser(struct UserID &userId, char password[]){
    userInfosCallBackValue.clear();

    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_INFO_TABLE_SELECT, userId.userName), retUserInfosCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("find user:%s SQL error : %s\n", userId.userName, zErrMsg); return false;})

    if(userInfosCallBackValue.size() == 1){
        strcpy(password, userInfosCallBackValue[0].passWord);
        return true;
    }else{
        return false;
    }
}

bool Sqlite::findAllUser(std::vector<struct UserID> &userIds){
    userInfosCallBackValue.clear();
    userIds.clear();

    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_INFO_TABLE_SELECT_ALL), retUserInfosCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("get all user SQL error : %s\n", zErrMsg); return false;})

    for(const auto& it : userInfosCallBackValue){
        userIds.emplace_back(it);
    }
    return true;
}

bool Sqlite::updateUser(const struct UserID &oldUserID, const struct UserID &newUserID){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_CHANGE_TABLE_UPDATE, newUserID.userName, newUserID.passWord, oldUserID.userName, oldUserID.passWord), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("update user:%s SQL error : %s\n", oldUserID.userName, zErrMsg); return false;})
    return true;
}


/********************************************* note records *************************************************/
bool Sqlite::addRecord(const char tableName[], const struct SocketMsgOpPack &record){
    char *operation = new char[2]();
    char *isCheck = new char[2]();
    operation[0] = record.operation;
    isCheck[0] = record.isCheck;
    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_INSERT,
                               tableName,
                               record.operateTimestamp,
                               record.createTimestamp,
                               operation,
                               isCheck,
                               record.data), nullptr, nullptr, &zErrMsg);
    delete[] operation;
    delete[] isCheck;
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("table:%s add record SQL error : %s\n", tableName, zErrMsg); return false;})
    return true;
}
bool Sqlite::delRecord(const char tableName[], const struct SocketMsgOpPack &record){
    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_DELETE_ONE,
                               tableName,
                               record.operateTimestamp,
                               record.createTimestamp), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("table %s delete record SQL error : %s\n", tableName, zErrMsg); return false;})
    return true;
}
bool Sqlite::delAllRecord(const char tableName[]){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_DELETE_ALL, tableName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("delete all record SQL error : %s\n", zErrMsg); return false;})
    return true;
}
bool Sqlite::findRecord(const char tableName[], struct SocketMsgOpPack &record){
    RecordsCallBackValue.clear();

    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_SELECT_ONE,
                               tableName,
                               record.operateTimestamp,
                               record.createTimestamp), retRecordsCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("table:%s find record SQL error : %s\n", tableName, zErrMsg); return false;})

    // _retChange mem need to free
    if(RecordsCallBackValue.size() == 1){
        return true;
    }else{
        return false;
    }
}

bool Sqlite::findAllRecord(const char tableName[], std::vector<struct  SocketMsgOpPack> &records){
    RecordsCallBackValue.clear();
    records.clear();

    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_SELECT_ALL, tableName), retRecordsCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("table:%s find records SQL error : %s\n", tableName, zErrMsg); return false;})

    // _retChange mem need to free
    for(const auto & it : RecordsCallBackValue){
        records.push_back(it);
    }
    return true;
}

// 查询某条目有没有执行过某操作
bool Sqlite::findOpRecord(const char tableName[], struct SocketMsgOpPack &record, char op){
    RecordsCallBackValue.clear();

    char *operation = new char[2]();
    operation[0] = op;
    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_SELECT_OP,
                               tableName,
                               record.createTimestamp,
                               operation), retRecordsCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("table:%s find op record SQL error : %s\n", tableName, zErrMsg); return false;})

    // _retChange mem need to free
    return RecordsCallBackValue.size() == 1;
}

bool Sqlite::updateRecord(const char tableName[], const struct SocketMsgOpPack &oldRecord, const struct SocketMsgOpPack &newRecord){
    char *operation = new char[2]();
    char *isCheck = new char[2]();
    operation[0] = newRecord.operation;
    isCheck[0] = newRecord.isCheck;
    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_NOTE_RECORDS_TABLE_UPDATE,
                               tableName,
                               newRecord.operateTimestamp,
                               newRecord.createTimestamp,
                               operation,
                               isCheck,
                               newRecord.data,
                               oldRecord.operateTimestamp,
                               oldRecord.createTimestamp), nullptr, nullptr, &zErrMsg);
    delete[] operation;
    delete[] isCheck;
    CHECK(ret, SQLITE_ERROR, {LogCtrl::error("table:%s update record SQL error : %s\n", tableName, zErrMsg); return false;})
    return true;
}