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
    if(ListCallBack.size() > 1){
        msg_pack = ListCallBack[0];
    }
    return true;
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
    char passwd[G_ARR_SIZE_PASSWD];
    if(!serverSqlite->loadUserValue(this->msg_pack.userName, passwd)){
        if(serverSqlite->saveUserValue(msg_pack.userName, msg_pack.passwd)){
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

bool ServerDataControl::userDoLogin() {
    char passwd[G_ARR_SIZE_PASSWD];
    if(serverSqlite->loadUserValue(this->msg_pack.userName, passwd)) {
        if (strcmp(passwd, this->msg_pack.passwd) == 0) {
            sqliteState = LoginSuccess;
            return true;
        }
        sqliteState = LoginPasswdError;
        return false;
    }
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

void ServerDataControl::saveLocalOpList(std::vector<struct SocketMsgOpPack> &msg_op_pack_list){
    serverSqlite->openDataBase();

    for(auto it : msg_op_pack_list){
        if(!serverSqlite->loadOpListSingle(it)){
            serverSqlite->saveOpList(it);
        }
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
    ServerSqlite::initDatabase();
}

MSG_State ServerDataControl::getSqliteState() {
    return sqliteState;
}


/*

vector<pair<string, string>> ServerSqlite::_resUserPasswd;
vector<MSG_OP_PACK> ServerSqlite::_retDataSet;
int ServerSqlite::atChangeTableSize;

int ServerSqlite::retUserDatasetCallback(void *data, int argc, char **argv, char **ColName){
    if (CALLBACK_ARGC_4 == argc){
        MSG_OP_PACK pack{};
        makeDataPack(pack, argv[0], argv[1], NULL_OP, argv[2][0], argv[3]);

        ServerSqlite::_retDataSet.push_back(pack);
    } else {
        LOG_ERROR("change table callback error")
    }

    return 0;
}

int ServerSqlite::newChangeAtChangeTableCallback(void *data, int argc, char **argv, char **ColName){
    atChangeTableSize++;
    return 0;
}

int ServerSqlite::userPasswdTableCallback(void *data, int argc, char **argv, char **ColName){
    if (CALLBACK_ARGC_2 == argc) {
        string username(argv[0]);
        string passwd(argv[1]);
        ServerSqlite::_resUserPasswd.emplace_back(username, passwd);
    } else {
        LOG_ERROR("user_passwd table callback error")
    }

    return 0;
}

ServerSqlite::ServerSqlite(const char *databaseName, char *userName, const char *passwd) {
    strcpy(g_databaseName, databaseName);
    strcpy(g_usersChangeTableName, userName);
    strcat(g_usersChangeTableName, "_changeTable");
    strcpy(g_usersDatasetTableName, userName);
    strcat(g_usersDatasetTableName, "_datasetTable");

    strcpy(_userName, userName);
    _passwd = passwd;
    sqliteState = SqliteStopped;
    sqliteOpState = UndefinedError;

    zErrMsg = nullptr;
    ret = 0;
    db = nullptr;

    ret = sqlite3_open(g_databaseName, &db);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("OPEN DATABASE SQL error : %s\n", zErrMsg)})

    ret = sqlite3_exec(db, SQL_CREATE_TABLE_USER_PASSWD , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("CREATE USERS TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError; exit(-1);})
}
ServerSqlite::~ServerSqlite() {
    sqlite3_close(db);
}

SqliteState ServerSqlite::cleanSqlite() {
    cleanDataSet();
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_CHANGE_TABLE_DELETE, g_usersChangeTableName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_CHANGE_TABLE_DELETE : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})
    return sqliteState;
}

SqliteState ServerSqlite::addUser() {
    // check new user
    _resUserPasswd.erase(_resUserPasswd.begin(), _resUserPasswd.end());
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_FIND_ONE_USER, _userName), userPasswdTableCallback, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SELECT USER : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    if (_resUserPasswd.empty()) {
        sqliteState = SqliteRunning;
        sqliteOpState = CreateUserSuccess;
    } else if (1 == _resUserPasswd.size()){
        sqliteState = SqliteStopped;
        sqliteOpState = CreateUserUserExists;
        LOG_ERROR("user %s exits", _userName)
        return sqliteState;
    } else {
        sqliteState = SqliteError;
        sqliteOpState = CreateUserUndefinedError;
        LOG_ERROR("create user error")
        return sqliteState;
    }

    // add new user
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_ADD_NEW_USER, _userName, _passwd.data()), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("ADD USER SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    // add new user's change table
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_CREATE_USER_CHANGE_TABLE, g_usersChangeTableName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_CREATE_USER_CHANGE_TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    // add new user's dataset table
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_CREATE_USER_DATASET_TABLE, g_usersDatasetTableName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_CREATE_USER_DATASET_TABLE SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    LOG_INFO("create user success")

    return sqliteState;
}

SqliteState ServerSqlite::loginRes() {
    _resUserPasswd.erase(_resUserPasswd.begin(), _resUserPasswd.end());
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_FIND_ONE_USER, _userName), userPasswdTableCallback, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("LOGIN SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    if (_resUserPasswd.empty()) {
        sqliteState = SqliteStopped;
        sqliteOpState = LoginUserNotExits;
        LOG_ERROR("user %s not exits", _userName)
        return sqliteState;
    } else if (1 == _resUserPasswd.size()){
        if (_resUserPasswd[0].second != _passwd){
            sqliteState = SqliteStopped;
            sqliteOpState = LoginPasswdError;
            LOG_ERROR("user %s's passwd %s is error", _userName, _resUserPasswd[0].second.data())
            return sqliteState;
        }else{
            sqliteState = SqliteRunning;
            sqliteOpState = LoginSuccess;
            LOG_INFO("login success")
        }
    } else {
        sqliteState = SqliteError;
        sqliteOpState = LoginUndefinedError;
        LOG_ERROR("login undefined error")
        return sqliteState;
    }

    return sqliteState;
}

SqliteState ServerSqlite::addChange(std::vector<MSG_OP_PACK> &changeArr) {
    char *op = new char[2]();
    char *isCheck = new char[2]();
    vector<MSG_OP_PACK> userDataset;
    returnDataSet(userDataset);

    for(auto & i : changeArr){
        if(newChangeAtChangeTable(i)){
            // change is exists
            continue;
        }
        if(ADD == i.op){
            MSG_OP_PACK newData{};
            makeDataPack(newData, i.opTimestamp, i.createTimestamp, i.op, i.isCheck, i.data);
            userDataset.push_back(newData);
        } else {
            for(int dataIndex = 0; dataIndex < userDataset.size(); ++dataIndex){
                if(0 == strcmp(userDataset[dataIndex].createTimestamp, i.createTimestamp)){
                    if (DEL == i.op) {
                        userDataset.erase(userDataset.begin() + dataIndex);
                    } else if (ALTER == i.op) {
                        makeDataPack(userDataset[dataIndex], i.opTimestamp, i.createTimestamp, i.op, i.isCheck, i.data);
                    } else if (CHECK == i.op) {
                        makeDataPack(userDataset[dataIndex], i.opTimestamp, i.createTimestamp, i.op, i.isCheck, i.data);
                    } else {
                        LOG_ERROR("unknown op")
                    }
                    break;
                }
            }
        }

        // insert into change table
        op[0] = i.op;
        isCheck[0] = i.isCheck;
        ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_CHANGE_TABLE_INSERT, g_usersChangeTableName, i.opTimestamp, i.createTimestamp, op, isCheck, i.data), nullptr, nullptr, &zErrMsg);
        CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_CHANGE_TABLE_INSERT SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})
    }

    // push new data set to dataset table
    cleanDataSet();
    pushDataSet(userDataset);
    return sqliteState;
}

bool ServerSqlite::newChangeAtChangeTable(MSG_OP_PACK &msg){
    atChangeTableSize = 0;

    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_CHANGE_TABLE_SELECT, g_usersChangeTableName, msg.opTimestamp, msg.createTimestamp), newChangeAtChangeTableCallback, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_CHANGE_TABLE_SELECT : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    return atChangeTableSize >= 1;
}

SqliteState ServerSqlite::returnDataSet(std::vector<MSG_OP_PACK> &userDataset){
    // clean _retDataSet
    _retDataSet.erase(_retDataSet.begin(), _retDataSet.end());

    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATASET_TABLE_SELECT, g_usersDatasetTableName), retUserDatasetCallback, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_SELECT : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})

    // _retDataSet mem need to free
    for(auto i : _retDataSet){
        userDataset.push_back(i);
    }

    return sqliteState;
}

SqliteState ServerSqlite::cleanDataSet(){
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATASET_TABLE_DELETE, g_usersDatasetTableName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_DELETE : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})
    return sqliteState;
}

SqliteState ServerSqlite::pushDataSet(std::vector<MSG_OP_PACK> &userDataset){
    char *op = new char[2]();
    char *isCheck = new char[2]();

    for(auto & i : userDataset){
        op[0] = i.op;
        isCheck[0] = i.isCheck;
        ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATASET_TABLE_INSERT, g_usersDatasetTableName, i.opTimestamp, i.createTimestamp, isCheck, i.data), nullptr, nullptr, &zErrMsg);
        CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_INSERT : SQL error : %s\n", zErrMsg) sqliteState = SqliteError; return sqliteState;})
    }
    return sqliteState;
}
*/