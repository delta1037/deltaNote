#include "deltaNote/datactrl.h"

char DataCtrl::databaseName[DATABASE_NAME_SIZE];
char DataCtrl::userLocalListTableName[DATABASE_TABLE_NAME_SIZE]{};
char DataCtrl::userLocalOpListTableName[DATABASE_TABLE_NAME_SIZE]{};

void DataCtrl::initDatabse(){
    // create table
    Sqlite::createTable(databaseName, SQL_USER_SETTING_TABLE_CREATE);
    Sqlite::createTable(databaseName, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_CREATE, userLocalListTableName));
    Sqlite::createTable(databaseName, sqlite3_mprintf(SQL_USER_DATABASE_TABLE_CREATE, userLocalOpListTableName));
}

DataCtrl::DataCtrl(){
    openDataBase(databaseName);
}

DataCtrl::~DataCtrl(){
    closeDataBase();
}
void DataCtrl::getAllSetting(){
    // init setting
    SettingPair usernamePair("username", "");
    if(!loadSettingValue(usernamePair)){
        saveSettingValue(usernamePair);
        LogCtrl::debug("save setting username");
    }else{
        strcpy(g_username, usernamePair.value);
    }

    SettingPair passwordPair("passwd", "");
    if(!loadSettingValue(passwordPair)){
        saveSettingValue(passwordPair);
        LogCtrl::debug("save setting password");
    }else{
        strcpy(g_passwd, passwordPair.value);
    }

    SettingPair serverIPPair("server", "39.96.162.190");
    if(!loadSettingValue(serverIPPair)){
        saveSettingValue(serverIPPair);
    }
    strcpy(g_server, serverIPPair.value);

    SettingPair serverPortPair("port", "8888");
    if(!loadSettingValue(serverPortPair)){
        saveSettingValue(serverPortPair);
    }
    g_port = atoi(serverPortPair.value);

    SettingPair fontColorPair("fontColor", QColor(252, 233, 79).name().toUtf8().data());
    if(!loadSettingValue(fontColorPair)){
        saveSettingValue(fontColorPair);
    }
    fontColor = QColor(fontColorPair.value);

    SettingPair fontSizePair("fontSize", "11");
    if(!loadSettingValue(fontSizePair)){
        saveSettingValue(fontSizePair);
    }
    fontSize = atoi(fontSizePair.value);

    SettingPair iconColorPair("iconColor", QColor(252, 175, 62).name().toUtf8().data());
    if(!loadSettingValue(iconColorPair)){
        saveSettingValue(iconColorPair);
    }
    iconColor = QColor(iconColorPair.value);

    SettingPair transparentPair("transparentPos", "200");
    if(!loadSettingValue(transparentPair)){
        saveSettingValue(transparentPair);
    }
    transparentPos = atoi(transparentPair.value);

    SettingPair xPosPair("xPos", "600");
    if(!loadSettingValue(xPosPair)){
        saveSettingValue(xPosPair);
    }
    xPos = atoi(xPosPair.value);

    SettingPair yPosPair("yPos", "300");
    if(!loadSettingValue(yPosPair)){
        saveSettingValue(yPosPair);
    }
    yPos = atoi(yPosPair.value);

    SettingPair frameHightPair("frameHeight", "560");
    if(!loadSettingValue(frameHightPair)){
        saveSettingValue(frameHightPair);
    }
    frameHeight = atoi(frameHightPair.value);

    SettingPair frameWidthPair("frameWidth", "300");
    if(!loadSettingValue(frameWidthPair)){
        saveSettingValue(frameWidthPair);
    }
    frameWidth = atoi(frameWidthPair.value);

    SettingPair isLockedPair("isLocked", "0");
    if(!loadSettingValue(isLockedPair)){
        saveSettingValue(isLockedPair);
    }
    isLocked = bool(isLockedPair.value[0] - '0');

    SettingPair isAutoStartPair("isAutoStart", "0");
    if(!loadSettingValue(isAutoStartPair)){
        saveSettingValue(isAutoStartPair);
    }
    isAutoStart = bool(isAutoStartPair.value[0] - '0');
}

void DataCtrl::setAllSetting(){
    SettingPair usernamePair("username", g_username);
    alterSettingValue(usernamePair);
    SettingPair passwordPair("passwd", g_passwd);
    alterSettingValue(passwordPair);

    SettingPair serverIPPair("server", g_server);
    alterSettingValue(serverIPPair);
    SettingPair serverPortPair("port", to_string(g_port).data());
    alterSettingValue(serverPortPair);

    SettingPair fontColorPair("fontColor", fontColor.name().toUtf8().data());
    alterSettingValue(fontColorPair);
    SettingPair fontSizePair("fontSize", to_string(fontSize).data());
    alterSettingValue(fontSizePair);
    SettingPair iconColorPair("iconColor", iconColor.name().toUtf8().data());
    alterSettingValue(iconColorPair);

    SettingPair transparentPair("transparentPos", to_string(transparentPos).data());
    alterSettingValue(transparentPair);

    SettingPair xPosPair("xPos", to_string(xPos).data());
    alterSettingValue(xPosPair);
    SettingPair yPosPair("yPos", to_string(yPos).data());
    alterSettingValue(yPosPair);
    SettingPair frameWidthPair("frameWidth", to_string(frameWidth).data());
    alterSettingValue(frameWidthPair);
    SettingPair frameHightPair("frameHeight", to_string(frameHeight).data());
    alterSettingValue(frameHightPair);

    SettingPair isLockedPair("isLocked", isLocked == true? "1" : "0");
    alterSettingValue(isLockedPair);
    SettingPair isAutoStartPair("isAutoStart", isAutoStart == true? "1" : "0");
    alterSettingValue(isAutoStartPair);
}

void DataCtrl::saveLocalList(std::vector<SocketMsgOpPack> &packList){
    for(const auto &it : packList){
        if(!addRecord(userLocalListTableName, it)){
            LogCtrl::error("load local db list error");
        }
    }
}
void DataCtrl::loadLocalList(std::vector<SocketMsgOpPack> &packList){
    if(!findAllRecord(userLocalListTableName, packList)){
        LogCtrl::error("load local db list error");
    }
}

void DataCtrl::cleanLocalList(){
    if(!delAllRecord(userLocalListTableName)){
        LogCtrl::error("delete local db list error");
    }
}
