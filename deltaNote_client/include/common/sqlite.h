
#ifndef SQLITE_H
#define SQLITE_H

#include <string>
#include <vector>
#include <cstring>

#include "log/logCtrl.h"
#include "common/untils.h"
#include "sqlite/sqlite3.h"

using namespace std;

class Sqlite {
public:
    static bool createTable(const char *, const char *);                  // 初始化设置表

    static int retSettingCallBack(void *data, int argc, char **argv, char **ColName);
    static int retRecordsCallBack(void *data, int argc, char **argv, char **ColName);

public:
    Sqlite();
    ~Sqlite();

    // 打开关闭数据库
    bool openDataBase(const char *);
    bool closeDataBase();

    // 查询setting的值，查询得到返回true，查询不到返回false
    bool loadSettingValue(SettingPair &);
    // 存储setting的值，存储成功返回true，失败返回false
    bool saveSettingValue(SettingPair &);
    // 修改setting的值，修改成功返回true，失败返回false
    bool alterSettingValue(SettingPair &);
    // 删除指定的Setting，用于去重
    bool delSetting(SettingPair &);

    // 记录：增、删、查、改
    bool addRecord(const char tableName[], const struct SocketMsgOpPack &record);     // 增
    bool delRecord(const char tableName[], const struct SocketMsgOpPack &record);     // 删
    bool delAllRecord(const char tableName[]);                                  // 删
    bool findRecord(const char tableName[], struct SocketMsgOpPack &record);    // 查
    bool findAllRecord(const char tableName[],
        std::vector<struct  SocketMsgOpPack> &records);                         // 查
    bool findOpRecord(const char tableName[], struct SocketMsgOpPack &record, char op);    // 查
    bool updateRecord(const char tableName[], const struct SocketMsgOpPack &oldRecord,
        const struct SocketMsgOpPack &newRecord);                                     // 改

private:
    sqlite3 *db;
    char *zErrMsg;

    static std::vector<struct SocketMsgOpPack> RecordsCallBackValue;
    static std::vector<struct SettingPair> SettingsCallBackValue;
};
#endif // SQLITE_H
