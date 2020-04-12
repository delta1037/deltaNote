#include "common/untils.h"

/************************************服务端相关函数********************************/
MsgPack::~MsgPack() {
    delete msgBodyDownload.updateMessage;
    delete msgBodyOp.operationQueue;
}

struct MsgHead &MsgPack::getMsgHead() {
    return msgHead;
}

struct MsgBodyOp &MsgPack::getMsgBodyOp() {
    return msgBodyOp;
}

struct MsgBodyAck &MsgPack::getMsgBodyAck() {
    return msgBodyAck;
}

struct MsgBodyDownload &MsgPack::getMsgBodyDownLoad(){
    return msgBodyDownload;
}

void MsgPack::setTodoVersion(uint16_t version) {
    msgHead.todoVersion = msgHead.todoVersion - (msgHead.todoVersion % DEVICE_TYPE_MOD) + version;
}

uint16_t MsgPack::getTodoVersion() {
    return (msgHead.todoVersion % DEVICE_TYPE_MOD);
}

void MsgPack::setTodoDeviceType(DeviceType type) {
    msgHead.todoVersion = type * DEVICE_TYPE_MOD + msgHead.todoVersion % DEVICE_TYPE_MOD;
}

DeviceType MsgPack::getTodoDeviceType() {
    return DeviceType(msgHead.todoVersion / DEVICE_TYPE_MOD);
}

void MsgPack::setMSgType(MsgType msgType) {
    msgHead.msgType = msgType;
}

MsgType MsgPack::getMsgType() {
    return msgHead.msgType;
}

void MsgPack::setMsgBodySize(uint32_t size) {
    msgHead.msgBodySize = size;
}

uint32_t MsgPack::getMsgBodySize() {
    return msgHead.msgBodySize;
}

void MsgPack::setUserID(struct UserID &t_userID){
    msgBodyOp.userID = t_userID;
}

struct UserID &MsgPack::getUserID() {
    return msgBodyOp.userID;
}

void MsgPack::setOpQueueSize(uint32_t size) {
    msgBodyOp.msgOpQueueSize = size;
}

uint32_t MsgPack::getOpQueueSize() {
    return msgBodyOp.msgOpQueueSize;
}

void MsgPack::setOperationQueue(std::vector<struct SocketMsgOpPack> &packList) {
    msgBodyOp.operationQueue = new SocketMsgOpPack[packList.size()];
    msgBodyOp.msgOpQueueSize = uint32_t(packList.size());
    for(unsigned long i = 0; i < packList.size(); ++i){
        msgBodyOp.operationQueue[i] = packList[i];
    }
}

void MsgPack::getOperationQueue(std::vector<struct SocketMsgOpPack> &packList) {
    packList.clear();
    for(unsigned long  i = 0; i < msgBodyOp.msgOpQueueSize; ++i){
        packList.emplace_back(msgBodyOp.operationQueue[i]);
    }
}

void MsgPack::setMsgState(MsgStatus state) {
    msgBodyAck.msgStatus = state;
}

MsgStatus MsgPack::getMsgState() {
    return msgBodyAck.msgStatus;
}

void MsgPack::setOpQueueAckSize(uint32_t size) {
    msgBodyAck.msgQueueAckSize = size;
}

uint32_t MsgPack::getOpQueueAckSize() {
    return msgBodyAck.msgQueueAckSize;
}

void MsgPack::setDownloadLink(std::string &link) {
    strcpy(msgBodyDownload.downloadLink, link.c_str());
}

void MsgPack::setDownloadLink(const char link[]) {
    strcpy(msgBodyDownload.downloadLink, link);
}

char *MsgPack::getDownloadLink() {
    return msgBodyDownload.downloadLink;
}

void  MsgPack::setDownloadMessageSize(uint32_t size){
    msgBodyDownload.updateMessageSize = size;
}
uint32_t  MsgPack::getDownloadMessageSize(){
    return msgBodyDownload.updateMessageSize;
}

void  MsgPack::setDownloadMessage(const char *msg){
    msgBodyDownload.updateMessage = new char[msgBodyDownload.updateMessageSize];
    strncpy(msgBodyDownload.updateMessage, msg, msgBodyDownload.updateMessageSize);
}
char * MsgPack::getDownloadMessage(){
    return msgBodyDownload.updateMessage;
}

/***********************************客户端相关函数********************************/
SocketMsgOpPack::SocketMsgOpPack(const MsgOpPack &msg){
    this->operation = msg.operation;
    this->isCheck = msg.isCheck;

    sprintf(this->createTimestamp, "%lld", msg.createTime);
    sprintf(this->operateTimestamp, "%lld", msg.operateTime);
    strcpy(this->data, msg.data.toUtf8().data());
}

MsgOpPack::MsgOpPack(const SocketMsgOpPack &msg){
    this->operation = msg.operation;
    this->isCheck = msg.isCheck;
    this->createTime = atoll(msg.createTimestamp);
    this->operateTime = atoll(msg.operateTimestamp);
    this->data = msg.data; // char -> QString
}

/*******************************客户端相关变量**********************************/
char    g_server[SETTING_SERVER_IP_SIZE];
int     g_port;
char    g_username[PACK_USERNAME_SIZE];
char    g_passwd[PACK_PASSWORD_SIZE];

bool    isLogin;
bool    isLocked;
bool    isAutoStart;

int     fontSize;
QColor  fontColor;
QColor  iconColor;
int     transparentPos;

int     xPos;
int     yPos;
int     frameHeight;
int     frameWidth;

QRect   desktopRect;
QWidget *mainWindowWidget;

// user setting table
const char *SQL_USER_SETTING_TABLE_CREATE = "CREATE TABLE IF NOT EXISTS settingTable (setting VARCHAR(64) NOT NULL, value VARCHAR(128))";
// insert setting table
const char *SQL_USER_SETTING_TABLE_INSERT =         "INSERT INTO settingTable (setting, value) VALUES (%Q, %Q)";
const char *SQL_USER_SETTING_TABLE_INSERT_SETTING = "INSERT INTO settingTable (setting) VALUES (%Q)";
// select setting table
const char *SQL_USER_SETTING_TABLE_SELECT = "SELECT setting, value FROM settingTable WHERE setting == %Q";
// alter setting table
const char *SQL_USER_SETTING_TABLE_UPDATE = "UPDATE settingTable SET value = %Q WHERE setting == %Q";
// delete setting
const char *SQL_USER_SETTING_TABLE_DELETE = "DELETE from settingTable WHERE setting == %Q";

// create Table %Q
const char *SQL_USER_DATABASE_TABLE_CREATE = "CREATE TABLE IF NOT EXISTS %Q (opTimestamp VARCHAR(32) NOT NULL, createTimestamp VARCHAR(32) NOT NULL, op VARCHAR(1) NOT NULL, isCheck VARCHAR(1) NOT NULL, data VARCHAR(128) NOT NULL)";
// add
const char *SQL_USER_DATABASE_TABLE_INSERT = "INSERT INTO %Q (opTimestamp, createTimestamp, op, isCheck, data) VALUES (%Q, %Q, %Q, %Q, %Q)";
// delete one
const char *SQL_USER_DATABASE_TABLE_DELETE_ONE = "DELETE from %Q WHERE opTimestamp == %Q AND createTimestamp == %Q";
// clean table %Q
const char *SQL_USER_DATABASE_TABLE_DELETE_ALL = "DELETE from %Q";
// select one
const char *SQL_USER_DATABASE_TABLE_SELECT_ONE = "SELECT opTimestamp, createTimestamp, op, isCheck, data from %Q WHERE opTimestamp == %Q and createTimestamp == %Q";
// select all
const char *SQL_USER_DATABASE_TABLE_SELECT_ALL = "SELECT opTimestamp, createTimestamp, op, isCheck, data from %Q";
// select op
const char *SQL_USER_DATABASE_TABLE_SELECT_OP = "SELECT opTimestamp, createTimestamp, op, isCheck, data from %Q WHERE createTimestamp == %Q and op == %Q";
// update
const char *SQL_USER_NOTE_RECORDS_TABLE_UPDATE = "UPDATE %Q SET opTimestamp = %Q, createTimestamp = %Q, op = %Q, isCheck = %Q, data = %Q WHERE opTimestamp == %Q and createTimestamp == %Q";

