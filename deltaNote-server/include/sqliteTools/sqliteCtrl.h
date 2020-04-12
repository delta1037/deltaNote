#ifndef SQLITE_TOOLS_SQLITE_CONTROL
#define SQLITE_TOOLS_SQLITE_CONTROL

#include <vector>

#include "sqlite.h"

class SqliteControl {
public:
    // init when server start
    static bool initControlEnv(const char dbPath[]);

public:
    SqliteControl();
    ~SqliteControl();

    // 用户是否存在
    bool findUser(struct UserID &userInfos);
    
    // 返回所有用户
    void findAllUser(std::vector<struct UserID> &userInfos);

    // 加载指定用户的记录
    void loadTodoRecords(const char userName[], std::vector<struct SocketMsgOpPack> &todoRecords, bool isOpRecord);

private:
    static char serverDatabaseName[DATABASE_NAME_SIZE];
    char userLocalOpListTableName[DATABASE_TABLE_NAME_SIZE]{};
    char userLocalListTableName[DATABASE_TABLE_NAME_SIZE]{};

    Sqlite *sqlite;
};
#endif //SQLITE_TOOLS_SQLITE_CONTROL