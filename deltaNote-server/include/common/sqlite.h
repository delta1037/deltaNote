#ifndef DELTA_NOTE_SQLITE
#define DELTA_NOTE_SQLITE

#include <vector>

#include "sqlite3.h"
#include "untils.h"
#include "../logCtrl/logCtrl.h"

class Sqlite {
public:
    static bool initDatabase(const char []);                         // 初始化表

    static int retUserInfosCallBack(void *data, int argc, char **argv, char **ColName);
    static int retRecordsCallBack(void *data, int argc, char **argv, char **ColName);
    
public:
    Sqlite();
    ~Sqlite();

    // 打开关闭数据库
    bool openDataBase(const char []);
    bool closeDataBase();

    // 用户：增、删、查、改
    bool addUser(const struct UserID &);                                     // 增
    bool delUser(const struct UserID &);                                     // 删
    bool findUser(struct UserID &, char []);                                    // 查
    bool findAllUser(std::vector<struct UserID> &);                         // 查
    bool updateUser(const struct UserID &, const struct UserID &);  // 改

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

    static std::vector<struct UserID> userInfosCallBackValue;
    static std::vector<struct SocketMsgOpPack> RecordsCallBackValue;
};
#endif //DELTA_NOTE_SQLITE
