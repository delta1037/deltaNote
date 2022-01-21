#ifndef DATACTRL_H
#define DATACTRL_H

#include "sqlite/sqlite.h"

class DataCtrl : public Sqlite {
public:
    static void initDatabse();
public:
    DataCtrl();
    ~DataCtrl();

    // 加载和存储所有的配置
    void getAllSetting();
    void setAllSetting();

    // 存储多条数据记录
    void saveLocalList(std::vector<SocketMsgOpPack> &);
    // 加载多条数据记录
    void loadLocalList(std::vector<SocketMsgOpPack> &);
    // 清除所有数据记录
    void cleanLocalList();

    static char databaseName[DATABASE_NAME_SIZE];   // 数据库名字
    static char userLocalOpListTableName[DATABASE_TABLE_NAME_SIZE];
    static char userLocalListTableName[DATABASE_TABLE_NAME_SIZE];
};
#endif // DATACTRL_H
