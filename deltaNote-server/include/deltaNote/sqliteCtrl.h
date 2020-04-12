#ifndef DELTA_NOTE_SOCKET_CONTROL
#define DELTA_NOTE_SOCKET_CONTROL

#include "../common/sqlite.h"

class SqliteControl {
public:
    // init when server start
    static bool initControlEnv(const char dbPath[]);

public:
    SqliteControl(struct UserID &userId);
    ~SqliteControl();

    // 获取操作状态
    MsgStatus getSqliteState();
    
    // 添加新用户，添加成功返回true，添加失败返回false
    bool addNewUser(struct UserID &);
    // 用户登录，登录成功返回true，登录失败返回false
    bool userDoLogin(struct UserID &);

    // 存储多条操作记录
    void addTodoOpRecords(std::vector<struct SocketMsgOpPack> &todoOpRecords);
    // 加载多条数据记录
    void loadTodoRecords(std::vector<struct SocketMsgOpPack> &todoRecords);

private:
    // 更新单条数据记录
    void addRecordToTodoRecords(std::vector<struct SocketMsgOpPack> &todoRecords, struct SocketMsgOpPack &record);

private:
    static char serverDatabaseName[DATABASE_NAME_SIZE];
    char userLocalOpListTableName[DATABASE_TABLE_NAME_SIZE]{};
    char userLocalListTableName[DATABASE_TABLE_NAME_SIZE]{};

    Sqlite *sqlite;
    MsgStatus sqliteState;
};
#endif //DELTA_NOTE_SOCKET_CONTROL