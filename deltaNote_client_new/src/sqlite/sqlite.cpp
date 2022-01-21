#include "sqlite.h"
#include "common/socket.h"
#include "log.h"

using namespace std;

#define CALLBACK_ARGC_1 1
#define CALLBACK_ARGC_2 2
#define CALLBACK_ARGC_4 4
#define CALLBACK_ARGC_5 5

std::vector<struct SettingPair> Sqlite::SettingsCallBackValue;
std::vector<struct SocketMsgOpPack> Sqlite::RecordsCallBackValue;

bool Sqlite::createTable(const char *databaseName, const char *sql){
    sqlite3 *db;
    char *zErrMsg;

    int ret = sqlite3_open(databaseName, &db);
    CHECK(ret, SQLITE_ERROR, {sqlite_error("%s", "OPEN DATABASE SQL error\n"); return false;});

    ret = sqlite3_exec(db, sql , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {sqlite_error("SQL_CREATE_USER_SETTING_TABLE SQL error : %s\n", zErrMsg); return false;});

    sqlite3_close(db);
    return true;
}

// call back for setting
int Sqlite::retSettingCallBack(void *data, int argc, char **argv, char **ColName){
    if(argc == CALLBACK_ARGC_2){
        if(argv[1]){
            SettingsCallBackValue.emplace_back(argv[0], argv[1]);
        }else{
            SettingsCallBackValue.emplace_back(argv[0], "");
        }
    }
    return 0;
}

// call back for records table
int Sqlite::retRecordsCallBack(void *data, int argc, char **argv, char **ColName){
    if (CALLBACK_ARGC_5 == argc){
        RecordsCallBackValue.emplace_back(argv[0], argv[1], argv[2][0], argv[3][0], argv[4]);
    } else {
        sqlite_error("%s","return change set callback error");
    }
    return 0;
}

Sqlite::Sqlite() {
    zErrMsg = nullptr;
    db = nullptr;
}

Sqlite::~Sqlite()= default;

// 打开数据库
bool Sqlite::openDataBase(const char *databaseName){
    int ret = sqlite3_open(databaseName, &db);
    CHECK(ret, SQLITE_ERROR, {sqlite_error("OPEN DATABASE SQL error : %s\n", zErrMsg); return false;})
    return true;
}

bool Sqlite::closeDataBase(){
    return sqlite3_close(db);
}

/********************************************* Setting *************************************************/
// 查询setting的值，查询得到返回true，查询不到返回false
bool Sqlite::loadSettingValue(SettingPair &settingPair){
    SettingsCallBackValue.clear();
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_SELECT, settingPair.name), retSettingCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {sqlite_error("SQL_USER_SETTING_TABLE_SELECT SQL error : %s\n", zErrMsg); return false;});

    if(SettingsCallBackValue.size() == 1 && strlen(SettingsCallBackValue[0].value) > 0){
        strcpy(settingPair.value, SettingsCallBackValue[0].value);
        return true;
    }else{
        if(SettingsCallBackValue.size() == 1 && strlen(SettingsCallBackValue[0].value) == 0){
            sqlite_warn("setting : %s is null", settingPair.name);
        }else if(SettingsCallBackValue.size() > 1){
            sqlite_warn("setting : %s is too many, clean all", settingPair.name);
            delSetting(SettingsCallBackValue[0]);
        }else{
            sqlite_warn("no such setting : %s", settingPair.name);
        }
        return false;
    }
}

// 存储setting的值，存储成功返回true，失败返回false
bool Sqlite::saveSettingValue(SettingPair &settingPair){
    int ret;
    if(strlen(settingPair.value) == 0){
        ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_INSERT_SETTING, settingPair.name), nullptr, nullptr, &zErrMsg);
        CHECK(ret, SQLITE_ERROR, {sqlite_error("SQL_USER_SETTING_TABLE_INSERT SQL error : %s\n", zErrMsg); return false;})
    }else{
        ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_INSERT, settingPair.name, settingPair.value), nullptr, nullptr, &zErrMsg);
        CHECK(ret, SQLITE_ERROR, {sqlite_error("SQL_USER_SETTING_TABLE_INSERT SQL error : %s\n", zErrMsg); return false;})
    }
    return true;
}

// 修改setting的值，修改成功返回true，失败返回false
bool Sqlite::alterSettingValue(SettingPair &settingPair){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_UPDATE, settingPair.value, settingPair.name), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {sqlite_error("SQL_USER_SETTING_TABLE_UPDATE SQL error : %s\n", zErrMsg); return false;})
    return true;
}

// 删除指定的Setting，用于去重
bool Sqlite::delSetting(SettingPair &settingPair){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_DELETE, settingPair.name), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {sqlite_error("SQL_USER_SETTING_TABLE_DELETE SQL error : %s\n", zErrMsg); return false;})
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
    CHECK(ret, SQLITE_ERROR, {sqlite_error("table:%s add record SQL error : %s\n", tableName, zErrMsg); return false;})
    return true;
}
bool Sqlite::delRecord(const char tableName[], const struct SocketMsgOpPack &record){
    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_DELETE_ONE,
                               tableName,
                               record.operateTimestamp,
                               record.createTimestamp), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {sqlite_error("table %s delete record SQL error : %s\n", tableName, zErrMsg); return false;})
    return true;
}
bool Sqlite::delAllRecord(const char tableName[]){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_DELETE_ALL, tableName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {sqlite_error("delete all record SQL error : %s\n", zErrMsg); return false;})
    return true;
}
bool Sqlite::findRecord(const char tableName[], struct SocketMsgOpPack &record){
    RecordsCallBackValue.clear();

    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_SELECT_ONE,
                               tableName,
                               record.operateTimestamp,
                               record.createTimestamp), retRecordsCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {sqlite_error("table:%s find record SQL error : %s\n", tableName, zErrMsg); return false;})

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
    CHECK(ret, SQLITE_ERROR, {sqlite_error("table:%s find records SQL error : %s\n", tableName, zErrMsg); return false;})

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
    CHECK(ret, SQLITE_ERROR, {sqlite_error("table:%s find op record SQL error : %s\n", tableName, zErrMsg); return false;})

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
    CHECK(ret, SQLITE_ERROR, {sqlite_error("table:%s update record SQL error : %s\n", tableName, zErrMsg); return false;})
    return true;
}
