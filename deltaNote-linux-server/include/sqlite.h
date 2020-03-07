//
// Created by geniusrabbit on 18-2-27.
//

#ifndef GENIUSNOTE1_0_SQLITESERVER_H
#define GENIUSNOTE1_0_SQLITESERVER_H


#include <string>
#include <vector>
#include <cstring>

#include "untils.h"
#include "sqlite3.h"

struct SocketMsgOpPack;
struct SocketMsgPack;

using namespace std;

class ServerSqlite {
public:
    ServerSqlite();
    ~ServerSqlite();

    // 初始化表
    static bool initDatabase();

    // 打开关闭数据库
    bool openDataBase();
    bool closeDataBase();

    // 设置用户的操作表和数据名称
    bool setUserTable(const char userName[], char value[]);
    // 查询用户，查询得到返回true，查询不到返回false
    bool loadUserValue(const char userName[], char value[]);
    // 存储用户，存储成功返回true，失败返回false
    bool saveUserValue(const char userName[], char value[]);
    // 修改用户，修改成功返回true，失败返回false
    //bool alterUserValue(const char settingName[], char value[]);

    // 加载所有的操作记录，成功返回true，失败返回false
    bool loadOpList(std::vector<struct  SocketMsgOpPack> &msg_pack);
    // 查询操作记录，成功返回true，失败返回false
    bool loadOpListSingle(struct SocketMsgOpPack &msg_pack);
    // 存储操作记录，成功返回true，失败返回false
    bool saveOpList(struct SocketMsgOpPack &msg_pack);
    // 清除所有的操作记录
    bool cleanOpList();
    // 清除单条操作记录
    bool cleanOpListSingle(struct SocketMsgOpPack &msg_pack);

    // 加载所有的数据记录，成功返回true，失败返回false
    bool loadList(std::vector<struct SocketMsgOpPack> &msg_pack);
    // 查询数据记录，成功返回true，失败返回false
    bool loadListSingle(struct SocketMsgOpPack &msg_pack);
    // 存储数据记录，成功返回true，失败返回false
    bool saveList(struct SocketMsgOpPack &msg_pack);
    // 清除所有的数据记录
    bool cleanList();
    // 清除单条数据记录
    bool cleanListSingle(struct SocketMsgOpPack &msg_pack);

    static int retUserInfo(void *data, int argc, char **argv, char **ColName);
    static int retListCallBack(void *data, int argc, char **argv, char **ColName);
public:
    static char g_databaseName[G_DATABASE_NAME_SIZE];
private:
    sqlite3 *db;
    char *zErrMsg;

    char userLocalOpListTableName[G_DATABASE_TABLE_NAME_SIZE]{};
    char userLocalListTableName[G_DATABASE_TABLE_NAME_SIZE]{};

    static bool userAtTableCallBack;
    static UserInfo userInfoCallBack;
    static vector<struct SocketMsgOpPack> ListCallBack;
};

class ServerDataControl {
public:
    ServerDataControl(struct SocketMsgPack &_msg_pack);
    ~ServerDataControl();

    MSG_State getSqliteState();

    // 添加新用户，添加成功返回true，添加失败返回false
    bool addNewUser();
    // 用户登录，登录成功返回true，登录失败返回false
    bool userDoLogin();

    // 存储单条操作记录
    void saveLocalOpListSingle(struct SocketMsgOpPack &msg_pack);
    // 存储多条操作记录
    void saveLocalOpList(std::vector<struct SocketMsgOpPack> &msg_pack_list);
    // 加载多条操作记录
    void loadLocalOpList(std::vector<struct SocketMsgOpPack> &msg_pack_list);
    // 查询操作记录，成功返回true，失败返回false
    bool findLocalOpList(struct SocketMsgOpPack &msg_pack);
    // 清除所有操作记录
    void cleanLocalOpList();

    // 存储单条数据记录
    void saveLocalListSingle(struct SocketMsgOpPack &msg_pack);
    // 存储多条数据记录
    void saveLocalList(std::vector<struct SocketMsgOpPack> &msg_pack_list);
    // 加载多条数据记录
    void loadLocalList(std::vector<struct SocketMsgOpPack> &msg_pack_list);
    // 查询操作数据记录，成功返回true，失败返回false
    bool findLocalList(struct SocketMsgOpPack &msg_pack);
    // 清除所有数据记录
    void cleanLocalList();

    static bool initControlEnv(const char dbPath[]);
private:
    struct SocketMsgPack msg_pack;
    ServerSqlite *serverSqlite;
    MSG_State sqliteState;
};
#endif //GENIUSNOTE1_0_SQLITESERVER_H
