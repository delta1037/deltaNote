#include "../../include/deltaNote/sqliteCtrl.h"

char SqliteControl::serverDatabaseName[DATABASE_NAME_SIZE]{};

bool SqliteControl::initControlEnv(const char dbPath[]) {
    strcpy(serverDatabaseName, dbPath);
    return Sqlite::initDatabase(serverDatabaseName);
}

SqliteControl::SqliteControl(struct UserID &userId){
    strcpy(userLocalOpListTableName, userId.userName);
    strcat(userLocalOpListTableName, "OpTable");
    strcpy(userLocalListTableName, userId.userName);
    strcat(userLocalListTableName, "DataTable");

    sqlite = new Sqlite;
    sqlite->openDataBase(serverDatabaseName);
}

SqliteControl::~SqliteControl(){
    sqlite->closeDataBase();
    delete sqlite;
}

MsgStatus SqliteControl::getSqliteState() {
    return sqliteState;
}

bool SqliteControl::addNewUser(struct UserID &userId) {
    //struct UserID userId(this->userInfoMsg.userName, this->userInfoMsg.passwd);
    char password[PACK_PASSWORD_SIZE];
    if(!sqlite->findUser(userId, password)){
        if(sqlite->addUser(userId)){
            sqliteState = CreateUserSuccess;
            return true;
        }else{
            sqliteState = CreateUserUndefinedError;
            return false;
        }
    }else{
        sqliteState = CreateUserUserExists;
        return false;
    }
}

bool SqliteControl::userDoLogin(struct UserID &userId) {
    char password[PACK_PASSWORD_SIZE];
    if(sqlite->findUser(userId, password)) {
        if (strcmp(userId.passWord, password) == 0) {
            sqliteState = LoginSuccess;
            return true;
        }else{
            sqliteState = LoginPasswdError;
            return false;
        }
    }else{
        sqliteState = LoginUserNotExits;
        return false;
    }
}

void SqliteControl::addTodoOpRecords(std::vector<struct SocketMsgOpPack> &todoOpRecords){
    std::vector<struct SocketMsgOpPack> todoRecords;
    sqlite->findAllRecord(userLocalListTableName, todoRecords);
    // 收到的包为按照操作的时间顺序到达
    for(auto it : todoOpRecords){
        if(!sqlite->findRecord(userLocalOpListTableName, it)){
            sqlite->addRecord(userLocalOpListTableName, it);

            // 按照操作的顺序对服务端数据库数据进行更新
            addRecordToTodoRecords(todoRecords, it);
        }else{
            LogCtrl::warn("muti trans op pack");
        }
    }

    // save user data
    sqlite->delAllRecord(userLocalListTableName);
    for(const auto &it : todoRecords){
        sqlite->addRecord(userLocalListTableName, it);
    }
}

void SqliteControl::loadTodoRecords(std::vector<struct SocketMsgOpPack> &todoRecords){
    sqlite->findAllRecord(userLocalListTableName, todoRecords);
}

void SqliteControl::addRecordToTodoRecords(std::vector<struct SocketMsgOpPack> &todoRecords, struct SocketMsgOpPack &record){
    if(TODO_ADD == record.operation){
        // 新增操作是唯一的，可以直接插入
        todoRecords.push_back(record);
    } else {
        for(int dataIndex = 0; dataIndex < todoRecords.size(); ++dataIndex){
            // 当前时间点存在这条记录
            if(0 == strcmp(todoRecords[dataIndex].createTimestamp, record.createTimestamp)){
                // 较新操作记录才能对当前记录进行修改，旧操作记录则忽略，认为被新操作覆盖
                if(strcmp(todoRecords[dataIndex].operateTimestamp, record.operateTimestamp) < 0){
                    if (TODO_DEL == record.operation) {
                        todoRecords.erase(todoRecords.begin() + dataIndex);
                    } else if (TODO_ALTER == record.operation || TODO_CHECK == record.operation) {
                        todoRecords[dataIndex] = record;
                    } else {
                        // 之前有这条数据，但是这次的操作不是常规操作，忽略本次操作
                        LogCtrl::error("record exist, unknown op ： %c", record.operation);
                    }
                }
                return;
            }
        }

        /**
         * 当前时间点不存在这条记录，可能是该条目已经删除，或者ADD操作在本地被覆盖
         */
        // 当前要操作的条目有删除记录，这条操作已经过时了，直接返回
        if(sqlite->findOpRecord(userLocalOpListTableName, record, TODO_DEL)){
            return;
        }

        // 缺失ADD的，新的记录
        if(TODO_ALTER == record.operation || TODO_CHECK == record.operation){
            todoRecords.push_back(record);
        }else if (TODO_DEL == record.operation) {
            LogCtrl::info("record already delete");
        } else{
            // 之前没有这条数据，而且这次的操作不是常规操作
            LogCtrl::error("record not exist, unknown op ： %c", record.operation);
        }
    }
}
