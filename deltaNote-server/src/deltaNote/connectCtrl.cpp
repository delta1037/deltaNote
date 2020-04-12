#include "../../include/deltaNote/connectCtrl.h"

ConnectControl::ConnectControl() {
    socketServer = new DynamicPackCtrl();
}

ConnectControl::~ConnectControl() {
    delete socketServer;
}

bool ConnectControl::initServerConnect(const char *initServerIP, int initServerPort) {
    return Socket::initSocketServer(initServerIP,initServerPort);
}

bool ConnectControl::acceptNewConnect() {
    return socketServer->acceptConn();
}

void ConnectControl::processingClientRequest() {
    socketServer->getClientIPAddr(this->clientIPAddr);

    if(!socketServer->msgRecv(msgPack)){
        // 记录黑名单
        LogCtrl::error("receive first operation pack error, ip: %s", clientIPAddr);
        ServerBlackListControl::instance()->addBlackListItem(clientIPAddr);

        // return ack pack
        msgPack.setMSgType(ACK_STATUS_TYPE);
        socketServer->msgSend(msgPack);
        return;
    }

    // init database control
    if(msgPack.getMsgType() != UPDATE_GET_LINK_TYPE){
        dataControl = new SqliteControl(msgPack.getUserID());
    }else{
        dataControl = nullptr;
    }
    
    // process msg
    switch (msgPack.getMsgType()){
        case OP_CREATE_USER_TYPE:
            LogCtrl::info("Create new user:%s", msgPack.getUserID().userName);
            createNewUser();
            break;
        case OP_LOGIN_TYPE:
            LogCtrl::info("user:%s Login to server", msgPack.getUserID().userName);
            loginToServer();
            break;
        case OP_PULL_TYPE:
            LogCtrl::info("user:%s Pull from server", msgPack.getUserID().userName);
            loadFromServer();
            break;
        case OP_PUSH_TYPE:
            LogCtrl::info("user:%s Push to server", msgPack.getUserID().userName);
            uploadToServer();
            break;
        case UPDATE_GET_LINK_TYPE:
            LogCtrl::info("some one get update link");
            getUpdateLink();
            break;
        default:
            LogCtrl::error("unknown op: %c, IP:%s", msgPack.getMsgType(), this->clientIPAddr);

            // return ack pack
            msgPack.setMSgType(ACK_STATUS_TYPE);
            socketServer->msgSend(msgPack);
    }

    delete dataControl;
}

void ConnectControl::getUpdateLink(){
    msgPack.setMSgType(UPDATE_GET_LINK_TYPE);
    if(msgPack.getTodoVersion() != VERSION_ID){
        if(msgPack.getTodoDeviceType() == WINDOW_DEVICE){
            msgPack.setDownloadLink(WINDOW_CLIENT_DOWNLOAD_LINK);

            msgPack.setDownloadMessageSize(strlen(WINDOW_CLIENT_MESSAGE));
            msgPack.setDownloadMessage(WINDOW_CLIENT_MESSAGE);

            LogCtrl::debug("set message:%s", msgPack.getDownloadMessage());
        }else if(msgPack.getTodoDeviceType() == LINUX_DEVICE){
            msgPack.setDownloadLink(LINUX_CLIENT_DOWNLOAD_LINK);

            msgPack.setDownloadMessageSize(strlen(LINUX_CLIENT_MESSAGE));
            msgPack.setDownloadMessage(LINUX_CLIENT_MESSAGE);

            LogCtrl::debug("set message:%s", msgPack.getDownloadMessage());
        }else{
            msgPack.setDownloadMessageSize(0);

            LogCtrl::debug("send null");
        }
    }else{
        // send null
        msgPack.setDownloadMessageSize(0);
        LogCtrl::debug("send null");
    }
    socketServer->msgSend(msgPack);
}

void ConnectControl::loginToServer() {
    // check user
    if(!dataControl->userDoLogin(msgPack.getUserID())){
        LogCtrl::error("user:%s passwd:****** login error, IP: %s", msgPack.getUserID().userName, this->clientIPAddr);
    }

    msgPack.setMSgType(ACK_STATUS_TYPE);
    msgPack.setMsgState(dataControl->getSqliteState());
    socketServer->msgSend(msgPack);
}

void ConnectControl::createNewUser() {
    // check user
    if(!dataControl->addNewUser(msgPack.getUserID())){
        LogCtrl::error("user:%s passwd:****** create error, IP: %s", msgPack.getUserID().userName, this->clientIPAddr);
    }

    msgPack.setMSgType(ACK_STATUS_TYPE);
    msgPack.setMsgState(dataControl->getSqliteState());
    socketServer->msgSend(msgPack);
}

void ConnectControl::uploadToServer() {
    // check user
    if(!dataControl->userDoLogin(msgPack.getUserID())){
        LogCtrl::error("user:%s passwd:****** upload user error, IP: %s", msgPack.getUserID().userName, this->clientIPAddr);

        msgPack.setMSgType(ACK_STATUS_TYPE);
        msgPack.setMsgState(LoginPasswdError);
        socketServer->msgSend(msgPack);
        return;
    }

    // push data to database
    std::vector<SocketMsgOpPack> socketMsgOpList;
    msgPack.getOperationQueue(socketMsgOpList);
    if(!socketMsgOpList.empty()){
        dataControl->addTodoOpRecords(socketMsgOpList);
    }

    // send ack
    msgPack.setMSgType(ACK_STATUS_TYPE);
    msgPack.setMsgState(PushSuccess);
    msgPack.setOpQueueAckSize(socketMsgOpList.size());
    socketServer->msgSend(msgPack);

    LogCtrl::info("push to server success, size : %d", msgPack.getOpQueueAckSize());
}

void ConnectControl::loadFromServer() {
    // check user
    if(!dataControl->userDoLogin(msgPack.getUserID())){
        LogCtrl::error("user:%s passwd:******  load from server error, IP:%s", msgPack.getUserID().userName, this->clientIPAddr);

        msgPack.setMSgType(ACK_STATUS_TYPE);
        msgPack.setMsgState(LoginPasswdError);
        msgPack.setOpQueueAckSize(0);
        socketServer->msgSend(msgPack);
        return;
    }

    // load data from database
    std::vector<SocketMsgOpPack> socketMsgList;
    dataControl->loadTodoRecords(socketMsgList);

    // send data to client
    msgPack.setMSgType(OP_PULL_TYPE);
    msgPack.setOperationQueue(socketMsgList);
    socketServer->msgSend(msgPack);

    // send ack
    msgPack.setMSgType(ACK_STATUS_TYPE);
    msgPack.setMsgState(PullSuccess);
    msgPack.setOpQueueAckSize(socketMsgList.size());
    socketServer->msgSend(msgPack);

    LogCtrl::info("pull from server success, size : %d", msgPack.getOpQueueAckSize());
}

std::string ConnectControl::getClientIPAddr() {
    std::string ret(clientIPAddr);
    return ret;
}