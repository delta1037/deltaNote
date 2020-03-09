//
// Created by geniusrabbit on 18-2-27.
//
#include <cstdio>
#include "../include/sqlite.h"
#include "../include/untils.h"
#include "../include/Log.h"
#include "../include/socket.h"

#define CALLBACK_ARGC_1 1
#define CALLBACK_ARGC_2 2
#define CALLBACK_ARGC_4 4
#define CALLBACK_ARGC_5 5

// users table
const char *SQL_USER_INFO_TABLE_CREATE = "CREATE TABLE IF NOT EXISTS userInfo (userName CHAR(32) NOT NULL, passWord CHAR(256) NOT NULL)";
// add new user
const char *SQL_USER_INFO_TABLE_ADD =    "INSERT INTO userInfo (userName, passWord) VALUES(%Q, %Q)";
// choose user with username and passwd
const char *SQL_USER_INFO_TABLE_SELECT = "SELECT userName, passWord FROM userInfo WHERE userName == %Q";

// create Table %Q
const char *SQL_USER_DATABASE_TABLE_CREATE = "CREATE TABLE IF NOT EXISTS %Q (opTimestamp VARCHAR(32) NOT NULL, createTimestamp VARCHAR(32) NOT NULL, op VARCHAR(1) NOT NULL, isCheck VARCHAR(1) NOT NULL, data VARCHAR(128) NOT NULL)";
// insert one
const char *SQL_USER_DATABASE_TABLE_INSERT = "INSERT INTO %Q (opTimestamp, createTimestamp, op, isCheck, data) VALUES (%Q, %Q, %Q, %Q, %Q)";
// select one
const char *SQL_USER_DATABASE_TABLE_SELECT_ONE = "SELECT opTimestamp, createTimestamp, op, isCheck, data from %Q WHERE opTimestamp == %Q and createTimestamp == %Q";
// select all
const char *SQL_USER_DATABASE_TABLE_SELECT_ALL = "SELECT opTimestamp, createTimestamp, op, isCheck, data from %Q";
// delete one
const char *SQL_USER_DATABASE_TABLE_DELETE_ONE = "DELETE from %Q WHERE opTimestamp == %Q AND createTimestamp == %Q";
// clean table %Q
const char *SQL_USER_DATABASE_TABLE_DELETE_ALL = "DELETE from %Q";


char ServerSqlite::g_databaseName[G_DATABASE_NAME_SIZE];

bool ServerSqlite::userAtTableCallBack;
struct UserInfo ServerSqlite::userInfoCallBack;
vector<struct SocketMsgOpPack> ServerSqlite::ListCallBack;

// call back for user
int ServerSqlite::retUserInfo(void *data, int argc, char **argv, char **ColName){
    if(argc == CALLBACK_ARGC_2){
        userAtTableCallBack = true;
        ServerSqlite::userInfoCallBack = UserInfo(argv[0], argv[1]);
    }else{
        LOG_ERROR("return user info callback error")
    }
    return 0;
}

// call back for get table content
int ServerSqlite::retListCallBack(void *data, int argc, char **argv, char **ColName){
    if (CALLBACK_ARGC_5 == argc){
        ListCallBack.emplace_back(argv[0], argv[1], argv[2][0], argv[3][0], argv[4]);
    } else {
        LOG_ERROR("return change set callback error")
    }
    return 0;
}

ServerSqlite::ServerSqlite() {
    zErrMsg = nullptr;
    db = nullptr;
}

ServerSqlite::~ServerSqlite()= default;

// 打开数据库
bool ServerSqlite::openDataBase(){
    int ret = sqlite3_open(g_databaseName, &db);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("OPEN DATABASE SQL error : %s\n", zErrMsg) return false;})
    return true;
}

bool ServerSqlite::closeDataBase(){
    return sqlite3_close(db);
}

// 初始化表
bool ServerSqlite::initDatabase(){
    sqlite3 *db;
    char *zErrMsg;

    int ret = sqlite3_open(g_databaseName, &db);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("OPEN DATABASE SQL error : %s\n", zErrMsg) return false;})

    ret = sqlite3_exec(db, SQL_USER_INFO_TABLE_CREATE , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_CREATE_USER_SETTING_TABLE SQL error : %s\n", zErrMsg) return false;})

    sqlite3_close(db);
    return true;
}

/********************************************* User Op *************************************************/
bool ServerSqlite::setUserTable(const char *userName, char *value) {
    strcpy(userLocalOpListTableName, userName);
    strcat(userLocalOpListTableName, "OpTable");
    strcpy(userLocalListTableName, userName);
    strcat(userLocalListTableName, "DataTable");
    return false;
}

// 查询用户，查询得到返回true，查询不到返回false
bool ServerSqlite::loadUserValue(const char userName[], char value[]){
    userAtTableCallBack = false;
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_INFO_TABLE_SELECT, userName), retUserInfo, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_SETTING_TABLE_SELECT SQL error : %s\n", zErrMsg) return false;})
    if(value != nullptr && userAtTableCallBack){
        strcpy(value, userInfoCallBack.passWord);
    }
    return userAtTableCallBack;
}

// 存储用户，存储成功返回true，失败返回false
bool ServerSqlite::saveUserValue(const char userName[], char value[]){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_INFO_TABLE_ADD, userName, value), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_SETTING_TABLE_INSERT SQL error : %s\n", zErrMsg) return false;})

    // add new user's change table
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_CREATE, userLocalOpListTableName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATABASE_TABLE_CREAT SQL error : %s\n", zErrMsg) return false;})

    // add new user's dataset table
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_CREATE, userLocalListTableName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATABASE_TABLE_CREAT SQL error : %s\n", zErrMsg) return false;})
    return true;
}
/*
// 修改用户，修改成功返回true，失败返回false
bool ServerSqlite::alterUserValue(const char userName[], char value[]){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_UPDATE, value, userName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_SETTING_TABLE_UPDATE SQL error : %s\n", zErrMsg) return false;})
    return true;
}
*/
/********************************************** Op List************************************************/
// 加载所有的操作记录，成功返回true，失败返回false
bool ServerSqlite::loadOpList(std::vector<SocketMsgOpPack> &msg_pack){
    // clean _retChange
    ListCallBack.erase(ListCallBack.begin(), ListCallBack.end());

    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_SELECT_ALL, userLocalOpListTableName), retListCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_SELECT : SQL error : %s\n", zErrMsg) return false;})

    // _retChange mem need to free
    for(const auto & i : ListCallBack){
        msg_pack.push_back(i);
    }
    return true;
}

// 查询操作记录，成功返回true，失败返回false
bool ServerSqlite::loadOpListSingle(struct SocketMsgOpPack &msg_pack){
    // clean _retChange
    ListCallBack.erase(ListCallBack.begin(), ListCallBack.end());

    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_SELECT_ONE,
                               userLocalOpListTableName,
                               msg_pack.operateTimestamp,
                               msg_pack.createTimestamp), retListCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_SELECT : SQL error : %s\n", zErrMsg) return false;})

    // _retChange mem need to free
    if(ListCallBack.size() > 0){
        return true;
    }else{
        return false;
    }
}

// 存储操作记录，成功返回true，失败返回false
bool ServerSqlite::saveOpList(struct SocketMsgOpPack &msg_pack){
    char *operation = new char[2]();
    char *isCheck = new char[2]();
    operation[0] = msg_pack.operation;
    isCheck[0] = msg_pack.isCheck;
    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_INSERT,
                               userLocalOpListTableName,
                               msg_pack.operateTimestamp,
                               msg_pack.createTimestamp,
                               operation,
                               isCheck,
                               msg_pack.data), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_CHANGE_TABLE_INSERT SQL error : %s\n", zErrMsg) return false;})
    return true;
}

// 清除所有的操作记录
bool ServerSqlite::cleanOpList(){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_DELETE_ALL, userLocalOpListTableName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATABASE_TABLE_CLEAN : SQL error : %s\n", zErrMsg) return false;})
    return true;
}

// 清除单条操作记录
bool ServerSqlite::cleanOpListSingle(struct SocketMsgOpPack &msg_pack){
    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_DELETE_ONE,
                               userLocalOpListTableName,
                               msg_pack.operateTimestamp,
                               msg_pack.createTimestamp), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATABASE_TABLE_CLEAN : SQL error : %s\n", zErrMsg) return false;})
    return true;
}

/********************************************* List *************************************************/
// 加载所有的数据记录，成功返回true，失败返回false
bool ServerSqlite::loadList(std::vector<struct SocketMsgOpPack> &msg_pack){
    // clean _retChange
    ListCallBack.erase(ListCallBack.begin(), ListCallBack.end());

    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_SELECT_ALL, userLocalListTableName), retListCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_SELECT : SQL error : %s\n", zErrMsg) return false;})

    // _retChange mem need to free
    for(const auto& it : ListCallBack){
        msg_pack.push_back(it);
    }
    return true;
}
// 查询数据记录，成功返回true，失败返回false
bool ServerSqlite::loadListSingle(struct SocketMsgOpPack &msg_pack){
    // clean _retChange
    ListCallBack.erase(ListCallBack.begin(), ListCallBack.end());
    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_SELECT_ONE,
                               userLocalListTableName,
                               msg_pack.operateTimestamp,
                               msg_pack.createTimestamp), retListCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_SELECT : SQL error : %s\n", zErrMsg) return false;})

    // _retChange mem need to free
    if(ListCallBack.size() > 1){
        msg_pack = ListCallBack[0];
    }
    return true;
}

// 存储数据记录，成功返回true，失败返回false
bool ServerSqlite::saveList(struct SocketMsgOpPack &msg_pack){
    char *operation = new char[2]();
    char *isCheck = new char[2]();
    operation[0] = msg_pack.operation;
    isCheck[0] = msg_pack.isCheck;
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_INSERT,
                                           userLocalListTableName,
                                           msg_pack.operateTimestamp,
                                           msg_pack.createTimestamp,
                                           operation,
                                           isCheck,
                                           msg_pack.data), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_CHANGE_TABLE_INSERT SQL error : %s\n", zErrMsg) return false;})
    return true;
}

// 清除所有的数据记录
bool ServerSqlite::cleanList(){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_DELETE_ALL, userLocalListTableName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATABASE_TABLE_CLEAN : SQL error : %s\n", zErrMsg) return false;})
    return true;
}

// 清除数据操作记录
bool ServerSqlite::cleanListSingle(struct SocketMsgOpPack &msg_pack){
    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_DELETE_ONE,
                               userLocalListTableName,
                               msg_pack.operateTimestamp,
                               msg_pack.createTimestamp), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATABASE_TABLE_CLEAN : SQL error : %s\n", zErrMsg) return false;})
    return true;
}

/*
 * UserDataControl
 *
 */
ServerDataControl::ServerDataControl(struct SocketMsgPack &_msg_pack){
    serverSqlite = new ServerSqlite;
    this->msg_pack = _msg_pack;
    serverSqlite->setUserTable(_msg_pack.userName, _msg_pack.passwd);
}

ServerDataControl::~ServerDataControl(){
    delete serverSqlite;
}

bool ServerDataControl::addNewUser() {
    serverSqlite->openDataBase();
    char passwd[G_ARR_SIZE_PASSWD];
    if(!serverSqlite->loadUserValue(this->msg_pack.userName, passwd)){
        if(serverSqlite->saveUserValue(msg_pack.userName, msg_pack.passwd)){
            sqliteState = CreateUserSuccess;
            serverSqlite->closeDataBase();
            return true;
        }else{
            sqliteState = CreateUserUndefinedError;
            serverSqlite->closeDataBase();
            return false;
        }
    }else{
        sqliteState = CreateUserUserExists;
        serverSqlite->closeDataBase();
        return false;
    }
}

bool ServerDataControl::userDoLogin() {
    serverSqlite->openDataBase();
    char passwd[G_ARR_SIZE_PASSWD];
    if(serverSqlite->loadUserValue(this->msg_pack.userName, passwd)) {
        serverSqlite->closeDataBase();
        if (strcmp(passwd, this->msg_pack.passwd) == 0) {
            sqliteState = LoginSuccess;
            return true;
        }
        sqliteState = LoginPasswdError;
        return false;
    }
    serverSqlite->closeDataBase();
    sqliteState = LoginUserNotExits;
    return false;
}

void ServerDataControl::saveLocalOpListSingle(struct SocketMsgOpPack &msg_op_pack){
    serverSqlite->openDataBase();

    if(!serverSqlite->loadOpListSingle(msg_op_pack)){
        serverSqlite->saveOpList(msg_op_pack);
    }
    serverSqlite->closeDataBase();
}

void ServerDataControl::addPackToDataset(std::vector<struct SocketMsgOpPack> &userDataset, struct SocketMsgOpPack &pack){
    if(TODO_ADD == pack.operation){
        userDataset.push_back(pack);
        // LOG_INFO("TODO ADD")
    } else {
        int dataIndex = 0;
        for(dataIndex = 0; dataIndex < userDataset.size(); ++dataIndex){
            if(0 == strcmp(userDataset[dataIndex].createTimestamp, pack.createTimestamp)){
                if (TODO_DEL == pack.operation) {
                    userDataset.erase(userDataset.begin() + dataIndex);
                    // LOG_INFO("TODO DEL")
                } else if (TODO_ALTER == pack.operation || TODO_CHECK == pack.operation) {
                    userDataset[dataIndex] = pack;
                    // LOG_INFO("TODO ALTER || CHECK")
                } else {
                    // LOG_ERROR("unknown op")
                }
                break;
            }
        }
        if(dataIndex == userDataset.size()){
            LOG_ERROR("cant find this pack in userDataset")
        }
    }
}

void ServerDataControl::saveLocalOpList(std::vector<struct SocketMsgOpPack> &msg_op_pack_list){
    serverSqlite->openDataBase();

    vector<struct SocketMsgOpPack> userDataset;
    serverSqlite->loadList(userDataset);

    for(auto it : msg_op_pack_list){
        if(!serverSqlite->loadOpListSingle(it)){
            // LOG_INFO("add new op to oplist")
            serverSqlite->saveOpList(it);
            addPackToDataset(userDataset, it);
        }else{
            LOG_ERROR("muti trans op pack")
        }
    }

    // save user data
    serverSqlite->cleanList();
    for(auto it : userDataset){
        serverSqlite->saveList(it);
    }

    serverSqlite->closeDataBase();
}
void ServerDataControl::loadLocalOpList(std::vector<struct SocketMsgOpPack> &msg_op_pack_list){
    serverSqlite->openDataBase();

    serverSqlite->loadOpList(msg_op_pack_list);

    serverSqlite->closeDataBase();
}

bool ServerDataControl::findLocalOpList(struct SocketMsgOpPack &msg_op_pack){
    serverSqlite->openDataBase();
    bool ret = serverSqlite->loadOpListSingle(msg_op_pack);
    serverSqlite->closeDataBase();
    return ret;
}

void ServerDataControl::cleanLocalOpList(){
    serverSqlite->openDataBase();

    serverSqlite->cleanOpList();

    serverSqlite->closeDataBase();
}

void ServerDataControl::saveLocalListSingle(struct SocketMsgOpPack &msg_data_pack){
    serverSqlite->openDataBase();

    serverSqlite->saveList(msg_data_pack);

    serverSqlite->closeDataBase();
}
void ServerDataControl::saveLocalList(std::vector<struct SocketMsgOpPack> &msg_data_pack_list){
    serverSqlite->openDataBase();

    for(auto it : msg_data_pack_list){
        serverSqlite->saveList(it);
    }

    serverSqlite->closeDataBase();
}
void ServerDataControl::loadLocalList(std::vector<struct SocketMsgOpPack> &msg_data_pack_list){
    serverSqlite->openDataBase();

    serverSqlite->loadList(msg_data_pack_list);

    serverSqlite->closeDataBase();
}

bool ServerDataControl::findLocalList(struct SocketMsgOpPack &msg_data_pack){
    serverSqlite->openDataBase();

    bool ret = serverSqlite->loadListSingle(msg_data_pack);

    serverSqlite->closeDataBase();
    return ret;
}

void ServerDataControl::cleanLocalList(){
    serverSqlite->openDataBase();

    serverSqlite->cleanList();

    serverSqlite->closeDataBase();
}

bool ServerDataControl::initControlEnv(const char dbPath[]) {
    strcpy(ServerSqlite::g_databaseName, dbPath);
    return ServerSqlite::initDatabase();
}

MSG_State ServerDataControl::getSqliteState() {
    return sqliteState;
}