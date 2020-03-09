
#ifndef SQLITE_H
#define SQLITE_H

#include <string>
#include <vector>
#include <cstring>

#include "log.h"
#include "sqlite/sqlite3.h"

struct SocketMsgOpPack;

extern char dbPATH[PATH_SIZE];
using namespace std;

class ClientSqlite {
public:
    ClientSqlite();
    ~ClientSqlite();

    // 初始化表
    bool initDatabase();

    // 打开关闭数据库
    bool openDataBase();
    bool closeDataBase();

    // 查询setting的值，查询得到返回true，查询不到返回false
    bool loadSettingValue(const char settingName[], char value[]);
    // 存储setting的值，存储成功返回true，失败返回false
    bool saveSettingValue(const char settingName[], const char value[]);
    // 修改setting的值，修改成功返回true，失败返回false
    bool alterSettingValue(const char settingName[], const char value[]);

    // 加载所有的操作记录，成功返回true，失败返回false
    bool loadOpList(std::vector<SocketMsgOpPack> &msg_pack);
    // 查询操作记录，成功返回true，失败返回false
    bool loadOpListSingle(SocketMsgOpPack &msg_pack);
    // 存储操作记录，成功返回true，失败返回false
    bool saveOpList(SocketMsgOpPack &msg_pack);
    // 清除所有的操作记录
    bool cleanOpList();
    // 清除单条操作记录
    bool cleanOpListSingle(SocketMsgOpPack &msg_pack);

    // 加载所有的数据记录，成功返回true，失败返回false
    bool loadList(std::vector<SocketMsgOpPack> &msg_pack);
    // 查询数据记录，成功返回true，失败返回false
    bool loadListSingle(SocketMsgOpPack &msg_pack);
    // 存储数据记录，成功返回true，失败返回false
    bool saveList(SocketMsgOpPack &msg_pack);
    // 清除所有的数据记录
    bool cleanList();
    // 清除单条数据记录
    bool cleanListSingle(SocketMsgOpPack &msg_pack);

    static int retUserSetting(void *data, int argc, char **argv, char **ColName);
    static int retListCallBack(void *data, int argc, char **argv, char **ColName);
public:
    static char g_databaseName[G_DATABASE_NAME_SIZE];
    static char g_userLocalOpListTableName[G_DATABASE_TABLE_NAME_SIZE];
    static char g_userLocalListTableName[G_DATABASE_TABLE_NAME_SIZE];
private:
    sqlite3 *db;
    char *zErrMsg;

    static bool settingAtTableCallBack;
    static char settingNameCallBack[G_USERDATA_SETTING_SIZE];
    static char settingValueCallBack[G_USERDATA_VALUE_SIZE];
    static vector<SocketMsgOpPack> ListCallBack;
};

class UserDataControl {
public:
    UserDataControl();
    ~UserDataControl();
    // 加载和存储所有的配置
    void getAllSetting();
    void setAllSetting();

    // 存储单条操作记录
    void saveLocalOpListSingle(SocketMsgOpPack &msg_pack);
    // 存储多条操作记录
    void saveLocalOpList(std::vector<SocketMsgOpPack> &msg_pack_list);
    // 加载多条操作记录
    void loadLocalOpList(std::vector<SocketMsgOpPack> &msg_pack_list);
    // 查询操作记录，成功返回true，失败返回false
    bool findLocalOpList(SocketMsgOpPack &msg_pack);
    // 清除所有操作记录
    void cleanLocalOpList();

    // 存储单条数据记录
    void saveLocalListSingle(SocketMsgOpPack &msg_pack);

    // 存储多条数据记录
    void saveLocalList(std::vector<SocketMsgOpPack> &msg_pack_list);
    // 加载多条数据记录
    void loadLocalList(std::vector<SocketMsgOpPack> &msg_pack_list);
    // 查询操作数据记录，成功返回true，失败返回false
    bool findLocalList(SocketMsgOpPack &msg_pack);
    // 清除所有数据记录
    void cleanLocalList();
};

#endif // SQLITE_H
