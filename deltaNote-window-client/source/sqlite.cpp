#include "sqlite.h"

//
// Created by geniusrabbit on 18-2-27.
//

#include <cstdio>
#include "socket.h"

#define CALLBACK_ARGC_1 1
#define CALLBACK_ARGC_2 2
#define CALLBACK_ARGC_4 4
#define CALLBACK_ARGC_5 5

// user setting table
const char *SQL_USER_SETTING_TABLE_CREATE = "CREATE TABLE IF NOT EXISTS settingTable (setting VARCHAR(64) NOT NULL, value VARCHAR(128))";
// insert setting table
const char *SQL_USER_SETTING_TABLE_INSERT = "INSERT INTO settingTable (setting, value) VALUES (%Q, %Q)";
// select setting table
const char *SQL_USER_SETTING_TABLE_SELECT = "SELECT setting, value FROM settingTable WHERE setting == %Q";
// alter setting table
const char *SQL_USER_SETTING_TABLE_UPDATE = "UPDATE settingTable SET value = %Q WHERE setting == %Q";


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

extern char dbPATH[PATH_SIZE];

char ClientSqlite::g_databaseName[G_DATABASE_NAME_SIZE];
char ClientSqlite::g_userLocalOpListTableName[G_DATABASE_TABLE_NAME_SIZE];
char ClientSqlite::g_userLocalListTableName[G_DATABASE_TABLE_NAME_SIZE];

vector<SocketMsgOpPack> ClientSqlite::ListCallBack;

bool ClientSqlite::settingAtTableCallBack;
char ClientSqlite::settingValueCallBack[G_USERDATA_VALUE_SIZE];

char ClientSqlite::settingNameCallBack[32];

// call back for setting
int ClientSqlite::retUserSetting(void *data, int argc, char **argv, char **ColName){
    if(argc == CALLBACK_ARGC_2){
        settingAtTableCallBack = true;
        strcpy(settingNameCallBack, argv[0]);
        if(argv[1]){
            strcpy(settingValueCallBack, argv[1]);
        }
    }
    return 0;
}

// call back for get table content
int ClientSqlite::retListCallBack(void *data, int argc, char **argv, char **ColName){
    if (CALLBACK_ARGC_5 == argc){
        ListCallBack.push_back(SocketMsgOpPack(argv[0], argv[1], argv[2][0], argv[3][0], argv[4]));
    } else {
        LOG_ERROR("return change set callback error")
    }
    return 0;
}


ClientSqlite::ClientSqlite() {
    zErrMsg = nullptr;
    db = nullptr;
}

ClientSqlite::~ClientSqlite(){
    //sqlite3_close(db);
}
// 打开数据库
bool ClientSqlite::openDataBase(){
    int ret = sqlite3_open(g_databaseName, &db);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("OPEN DATABASE SQL error : %s\n", zErrMsg) return false;})
    return true;
}

bool ClientSqlite::closeDataBase(){
    return sqlite3_close(db);
}

// 初始化表
bool ClientSqlite::initDatabase(){
    int ret = sqlite3_exec(db, SQL_USER_SETTING_TABLE_CREATE , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_CREATE_USER_SETTING_TABLE SQL error : %s\n", zErrMsg) return false;})

    // add new user's change table
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_CREATE, g_userLocalOpListTableName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATABASE_TABLE_CREAT SQL error : %s\n", zErrMsg) return false;})

    // add new user's dataset table
    ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_CREATE, g_userLocalListTableName) , nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATABASE_TABLE_CREAT SQL error : %s\n", zErrMsg) return false;})

    return true;
}

/********************************************* Setting *************************************************/
// 查询setting的值，查询得到返回true，查询不到返回false
bool ClientSqlite::loadSettingValue(const char settingName[], char value[]){
    settingAtTableCallBack = false;
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_SELECT, settingName), retUserSetting, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_SETTING_TABLE_SELECT SQL error : %s\n", zErrMsg) return false;})
    if(value != nullptr){
        strcpy(value, settingValueCallBack);
    }
    return settingAtTableCallBack;
}

// 存储setting的值，存储成功返回true，失败返回false
bool ClientSqlite::saveSettingValue(const char settingName[], const char value[]){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_INSERT, settingName, value), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_SETTING_TABLE_INSERT SQL error : %s\n", zErrMsg) return false;})
    return true;
}

// 修改setting的值，修改成功返回true，失败返回false
bool ClientSqlite::alterSettingValue(const char settingName[], const char value[]){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_SETTING_TABLE_UPDATE, value, settingName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_SETTING_TABLE_UPDATE SQL error : %s\n", zErrMsg) return false;})
    return true;
}

/********************************************** Op List************************************************/
// 加载所有的操作记录，成功返回true，失败返回false
bool ClientSqlite::loadOpList(std::vector<SocketMsgOpPack> &msg_pack){
    // clean _retChange
    ListCallBack.erase(ListCallBack.begin(), ListCallBack.end());

    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_SELECT_ALL, g_userLocalOpListTableName), retListCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_SELECT : SQL error : %s\n", zErrMsg) return false;})

    // _retChange mem need to free
    for(auto it : ListCallBack){
        msg_pack.push_back(it);
    }
    return true;
}

// 查询操作记录，成功返回true，失败返回false
bool ClientSqlite::loadOpListSingle(SocketMsgOpPack &msg_pack){
    // clean _retChange
    ListCallBack.erase(ListCallBack.begin(), ListCallBack.end());

    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_SELECT_ONE,
                               g_userLocalOpListTableName,
                               msg_pack.operateTimestamp,
                               msg_pack.createTimestamp), retListCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_SELECT : SQL error : %s\n", zErrMsg) return false;})

    // _retChange mem need to free
    if(ListCallBack.size() > 0){
        msg_pack = ListCallBack[0];
    }
    return true;
}

// 存储操作记录，成功返回true，失败返回false
bool ClientSqlite::saveOpList(SocketMsgOpPack &msg_pack){
    char *operation = new char[2]();
    char *isCheck = new char[2]();
    operation[0] = msg_pack.operation;
    isCheck[0] = msg_pack.isCheck;
    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_INSERT,
                               g_userLocalOpListTableName,
                               msg_pack.operateTimestamp,
                               msg_pack.createTimestamp,
                               operation,
                               isCheck,
                               msg_pack.data), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_CHANGE_TABLE_INSERT SQL error : %s\n", zErrMsg) return false;})
    return true;
}

// 清除所有的操作记录
bool ClientSqlite::cleanOpList(){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_DELETE_ALL, g_userLocalOpListTableName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATABASE_TABLE_CLEAN : SQL error : %s\n", zErrMsg) return false;})
    return true;
}

// 清除单条操作记录
bool ClientSqlite::cleanOpListSingle(SocketMsgOpPack &msg_pack){
    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_DELETE_ONE,
                               g_userLocalOpListTableName,
                               msg_pack.operateTimestamp,
                               msg_pack.createTimestamp), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATABASE_TABLE_CLEAN : SQL error : %s\n", zErrMsg) return false;})
    return true;
}

/********************************************* List *************************************************/
// 加载所有的数据记录，成功返回true，失败返回false
bool ClientSqlite::loadList(std::vector<SocketMsgOpPack> &msg_pack){
    // clean _retChange
    ListCallBack.erase(ListCallBack.begin(), ListCallBack.end());

    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_SELECT_ALL, g_userLocalListTableName), retListCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_SELECT : SQL error : %s\n", zErrMsg) return false;})

    // _retChange mem need to free
    for(auto it : ListCallBack){
        msg_pack.push_back(it);
    }
    return true;
}
// 查询数据记录，成功返回true，失败返回false
bool ClientSqlite::loadListSingle(SocketMsgOpPack &msg_pack){
    // clean _retChange
    ListCallBack.erase(ListCallBack.begin(), ListCallBack.end());

    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_SELECT_ONE,
                               g_userLocalListTableName,
                               msg_pack.operateTimestamp,
                               msg_pack.createTimestamp), retListCallBack, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATASET_TABLE_SELECT : SQL error : %s\n", zErrMsg) return false;})

    // _retChange mem need to free
    if(ListCallBack.size() > 0){
        msg_pack = ListCallBack[0];
    }
    return true;
}

// 存储数据记录，成功返回true，失败返回false
bool ClientSqlite::saveList(SocketMsgOpPack &msg_pack){
    char *operation = new char[2]();
    char *isCheck = new char[2]();
    operation[0] = msg_pack.operation;
    isCheck[0] = msg_pack.isCheck;
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_INSERT,
                                           g_userLocalListTableName,
                                           msg_pack.operateTimestamp,
                                           msg_pack.createTimestamp,
                                           operation,
                                           isCheck,
                                           msg_pack.data), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_CHANGE_TABLE_INSERT SQL error : %s\n", zErrMsg) return false;})
    return true;
}

// 清除所有的数据记录
bool ClientSqlite::cleanList(){
    int ret = sqlite3_exec(db, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_DELETE_ALL, g_userLocalListTableName), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATABASE_TABLE_CLEAN : SQL error : %s\n", zErrMsg) return false;})
    return true;
}

// 清除数据操作记录
bool ClientSqlite::cleanListSingle(SocketMsgOpPack &msg_pack){
    int ret = sqlite3_exec(db, sqlite3_mprintf(
                               SQL_USER_DATABASE_TABLE_DELETE_ONE,
                               g_userLocalListTableName,
                               msg_pack.operateTimestamp,
                               msg_pack.createTimestamp), nullptr, nullptr, &zErrMsg);
    CHECK(ret, SQLITE_ERROR, {LOG_ERROR("SQL_USER_DATABASE_TABLE_CLEAN : SQL error : %s\n", zErrMsg) return false;})
    return true;
}

/*
 * UserDataControl
 *
 */

UserDataControl::UserDataControl(){

}
UserDataControl::~UserDataControl(){

}
void UserDataControl::getAllSetting(){
    strcpy(ClientSqlite::g_databaseName, dbPATH);
    strcpy(ClientSqlite::g_userLocalOpListTableName, "localOpListTable");
    strcpy(ClientSqlite::g_userLocalListTableName, "localListTable");

    ClientSqlite clientSqlite;
    clientSqlite.openDataBase();
    clientSqlite.initDatabase();

    // init setting
    char value[128];
    if(clientSqlite.loadSettingValue("username", value)){
        strcpy(g_username, value);
    }else{
        clientSqlite.saveSettingValue("username", "");
    }

    if(clientSqlite.loadSettingValue("passwd", value)){
        strcpy(g_passwd, value);
    }else{
        clientSqlite.saveSettingValue("passwd", "");
    }

    if(clientSqlite.loadSettingValue("server", value)){
        strcpy(g_server, value);
    }else{
        clientSqlite.saveSettingValue("server", "39.96.162.190");
        strcpy(g_server, "39.96.162.190");
    }

    if(clientSqlite.loadSettingValue("port", value)){
        g_port = atoi(value);
    }else{
        clientSqlite.saveSettingValue("port", "6666");
        g_port = 6666;
    }

    if(clientSqlite.loadSettingValue("fontColor", value)){
        fontColor = QColor(value);
    }else{
        fontColor = QColor(252, 233, 79);
        clientSqlite.saveSettingValue("fontColor", fontColor.name().toUtf8().data());
    }

    if(clientSqlite.loadSettingValue("fontSize", value)){
        fontSize = atoi(value);
    }else{
        clientSqlite.saveSettingValue("fontSize", "11");
        fontSize = 11;
    }

    if(clientSqlite.loadSettingValue("iconColor", value)){
        iconColor = QColor(value);
    }else{
        iconColor = QColor(252, 175, 62);
        clientSqlite.saveSettingValue("iconColor", iconColor.name().toUtf8().data());
    }

    if(clientSqlite.loadSettingValue("transparentPos", value)){
        transparentPos = atoi(value);
    }else{
        clientSqlite.saveSettingValue("transparentPos", "200");
        transparentPos = 200;
    }

    if(clientSqlite.loadSettingValue("xPos", value)){
        xPos = atoi(value);
    }else{
        clientSqlite.saveSettingValue("xPos", "600");
        xPos = 600;
    }

    if(clientSqlite.loadSettingValue("yPos", value)){
        yPos = atoi(value);
    }else{
        clientSqlite.saveSettingValue("yPos", "300");
        yPos = 300;
    }

    if(clientSqlite.loadSettingValue("frameHeight", value)){
        frameHeight = atoi(value);
    }else{
        clientSqlite.saveSettingValue("frameHeight", "560");
        frameHeight = 560;
    }

    if(clientSqlite.loadSettingValue("frameWidth", value)){
        frameWidth = atoi(value);
    }else{
        clientSqlite.saveSettingValue("frameWidth", "300");
        frameWidth = 300;
    }

    if(clientSqlite.loadSettingValue("isLocked", value)){
        isLocked = bool(value[0] - '0');
    }else{
        clientSqlite.saveSettingValue("isLocked", "0");
        isLocked = false;
    }

    if(clientSqlite.loadSettingValue("isAutoStart", value)){
        isAutoStart = bool(value[0] - '0');
    }else{
        clientSqlite.saveSettingValue("isAutoStart", "0");
        isAutoStart = false;
    }
    clientSqlite.closeDataBase();
}
void UserDataControl::setAllSetting(){
    ClientSqlite clientSqlite;
    clientSqlite.openDataBase();

    clientSqlite.alterSettingValue("username", g_username);
    clientSqlite.alterSettingValue("passwd", g_passwd);

    clientSqlite.alterSettingValue("server", g_server);
    clientSqlite.alterSettingValue("port", to_string(g_port).data());

    clientSqlite.alterSettingValue("fontColor", fontColor.name().toUtf8().data());
    clientSqlite.alterSettingValue("fontSize", to_string(fontSize).data());
    clientSqlite.alterSettingValue("iconColor", iconColor.name().toUtf8().data());

    clientSqlite.alterSettingValue("transparentPos", to_string(transparentPos).data());

    clientSqlite.alterSettingValue("xPos", to_string(xPos).data());
    clientSqlite.alterSettingValue("yPos", to_string(yPos).data());
    clientSqlite.alterSettingValue("frameWidth", to_string(frameWidth).data());
    clientSqlite.alterSettingValue("frameHeight", to_string(frameHeight).data());

    clientSqlite.alterSettingValue("isLocked", isLocked == true? "1" : "0");
    clientSqlite.alterSettingValue("isAutoStart", isAutoStart == true? "1" : "0");

    clientSqlite.closeDataBase();
}

void UserDataControl::saveLocalOpListSingle(SocketMsgOpPack &msg_pack){
    ClientSqlite clientSqlite;
    clientSqlite.openDataBase();

    clientSqlite.saveOpList(msg_pack);

    clientSqlite.closeDataBase();
}
void UserDataControl::saveLocalOpList(std::vector<SocketMsgOpPack> &msg_pack_list){
    ClientSqlite clientSqlite;
    clientSqlite.openDataBase();

    for(auto it : msg_pack_list){
        clientSqlite.saveOpList(it);
    }

    clientSqlite.closeDataBase();
}
void UserDataControl::loadLocalOpList(std::vector<SocketMsgOpPack> &msg_pack_list){
    ClientSqlite clientSqlite;
    clientSqlite.openDataBase();

    clientSqlite.loadOpList(msg_pack_list);

    clientSqlite.closeDataBase();
}

bool UserDataControl::findLocalOpList(SocketMsgOpPack &msg_pack){
    ClientSqlite clientSqlite;
    clientSqlite.openDataBase();

    bool ret = clientSqlite.loadOpListSingle(msg_pack);

    clientSqlite.closeDataBase();
    return ret;
}

void UserDataControl::cleanLocalOpList(){
    ClientSqlite clientSqlite;
    clientSqlite.openDataBase();

    clientSqlite.cleanOpList();

    clientSqlite.closeDataBase();
}

void UserDataControl::saveLocalListSingle(SocketMsgOpPack &msg_pack){
    ClientSqlite clientSqlite;
    clientSqlite.openDataBase();

    clientSqlite.saveList(msg_pack);

    clientSqlite.closeDataBase();
}
void UserDataControl::saveLocalList(std::vector<SocketMsgOpPack> &msg_pack_list){
    ClientSqlite clientSqlite;
    clientSqlite.openDataBase();

    for(auto it : msg_pack_list){
        clientSqlite.saveList(it);
    }

    clientSqlite.closeDataBase();
}
void UserDataControl::loadLocalList(std::vector<SocketMsgOpPack> &msg_pack_list){
    ClientSqlite clientSqlite;
    clientSqlite.openDataBase();

    clientSqlite.loadList(msg_pack_list);

    clientSqlite.closeDataBase();
}

bool UserDataControl::findLocalList(SocketMsgOpPack &msg_pack){
    ClientSqlite clientSqlite;
    clientSqlite.openDataBase();

    bool ret = clientSqlite.loadListSingle(msg_pack);

    clientSqlite.closeDataBase();
    return ret;
}

void UserDataControl::cleanLocalList(){
    ClientSqlite clientSqlite;
    clientSqlite.openDataBase();

    clientSqlite.cleanList();

    clientSqlite.closeDataBase();
}
