#include "deltaNote/connectctrl.h"

using namespace std;

ConnectCtrl::ConnectCtrl() {
    socketClient = new DynamicPackCtrl();
    inited =false;
}

ConnectCtrl::~ConnectCtrl() {
    delete socketClient;
}

bool ConnectCtrl::initSocket(){
    if(!inited){
        inited = socketClient->initSocket(g_server, g_port);
    }
    return inited;
}

MsgStatus ConnectCtrl::checkUpdate(string& downloadLink, string& downLoadMessage){
    // init socket
    if(!initSocket()){
        delta_note_error("%s", "connect control do update, socket init error")
        return ConnectError;
    }

    // set msgPack
    msgPack.setMSgType(UPDATE_GET_LINK_TYPE);

    // send request
    if(!socketClient->msgSend(msgPack)){
        return ConnectError;
    }

    // get download
    if(!socketClient->msgRecv(msgPack)){
        return ConnectError;
    }

    // set return value
    if(msgPack.getMsgState() != AlreadyLatest){
        downloadLink = string(msgPack.getDownloadLink());
        downLoadMessage = string(msgPack.getDownloadMessage());
        delta_note_debug("get download link:%s", downloadLink.c_str())
        delta_note_debug("get download message:%s", downLoadMessage.c_str())
    }

    return msgPack.getMsgState();
}

MsgStatus ConnectCtrl::loginToServer() {
    if(!checkUserIDValied()){
        return UserIDNull;
    }

    // init socket
    isLogin = false;
    if(!initSocket()){
        delta_note_error("%s", "connect control do login, socket init error")
        return ConnectError;
    }

    delta_note_debug("%s", "connect server success")

    // set msgPack
    msgPack.setMSgType(OP_LOGIN_TYPE);

    // send request
    if(!socketClient->msgSend(msgPack)){
        delta_note_error("%s", "login send request error")
        return msgPack.getMsgState();
    }

    // get ack
    if(!socketClient->msgRecv(msgPack)){
        delta_note_error("%s", "login get ack error")
        return msgPack.getMsgState();
    }

    if(msgPack.getMsgState() == LoginSuccess){
        delta_note_debug("%s", "login success")
        isLogin = true;
    }

    return msgPack.getMsgState();
}

MsgStatus ConnectCtrl::createNewUser() {
    if(!checkUserIDValied()){
        return UserIDNull;
    }
    // init socket
    if(!initSocket()){
        delta_note_error("%s", "connect control create user, socket init error")
        return ConnectError;
    }

    msgPack.setMSgType(OP_CREATE_USER_TYPE);
    // send request
    if(!socketClient->msgSend(msgPack)){
        delta_note_debug("%s", "connect control send request error")
        return msgPack.getMsgState();
    }

    // get ack
    if(!socketClient->msgRecv(msgPack)){
        delta_note_debug("%s", "connect control get ack error")
        return msgPack.getMsgState();
    }

    if(msgPack.getMsgState() == CreateUserSuccess){
        delta_note_debug("%s", "connect control create user success")
        isLogin = true;
    }

    return msgPack.getMsgState();
}

MsgStatus ConnectCtrl::uploadToServer(std::vector<struct SocketMsgOpPack>& packs) {
    if(!checkUserIDValied()){
        return UserIDNull;
    }

    // init socket
    if(!initSocket()){
        isLogin = false;
        delta_note_error("%s", "connect control upload to server, socket init error")
        return ConnectError;
    }

    delta_note_debug("%s", "connect control upload to server")

    msgPack.setMSgType(OP_PUSH_TYPE);
    msgPack.setOperationQueue(packs);

    // send request
    if(!socketClient->msgSend(msgPack)){
        return msgPack.getMsgState();
    }

    // get ack
    if(!socketClient->msgRecv(msgPack)){
        return msgPack.getMsgState();
    }

    if(msgPack.getOpQueueAckSize() != packs.size()){
        return PushError;
    }

    return msgPack.getMsgState();
}

MsgStatus ConnectCtrl::loadFromServer(std::vector<struct SocketMsgOpPack>& packs) {
    if(!checkUserIDValied()){
        return UserIDNull;
    }
    // init socket
    if(!initSocket()){
        isLogin = false;
        delta_note_error("%s", "connect control load from server, socket init error")
        return ConnectError;
    }

    msgPack.setMSgType(OP_PULL_TYPE);
    // send request
    if(!socketClient->msgSend(msgPack)){
        return msgPack.getMsgState();
    }

    // get result
    if(!socketClient->msgRecv(msgPack)){
        return msgPack.getMsgState();
    }

    // get ack
    if(!socketClient->msgRecv(msgPack)){
        return msgPack.getMsgState();
    }

    // set result
    packs.clear();
    if(msgPack.getMsgState() == PullSuccess){
        msgPack.getOperationQueue(packs);
    }else{
        delta_note_debug("%s", "connect control pull todo from server fail")
    }
    return msgPack.getMsgState();
}

bool ConnectCtrl::isIPAddr(const char *pStr){
    bool bRet = true;
    if (nullptr == pStr) return -1;
        const char* p = pStr;
    for (; *p != '\0'; p++)
    {
        if ((isalpha(*p)) && (*p != '.'))
        {
            bRet = false;
            break;
        }
    }
    return bRet;
}

void ConnectCtrl::parserServerPort(char *serverPort){
    bool isServer = true;
    char server[SETTING_SERVER_IP_SIZE] = {0};
    int port = 0;
    for(int i = 0; serverPort[i] != '\0'; ++i){
        if(serverPort[i] == ':'){
            isServer = false;
            server[i] = '\0';
            continue;
        }

        if(isServer){
            server[i] = serverPort[i];
        } else {
            port = port * 10 + serverPort[i] - '0';
        }
    }

    //check
    if(server[0] == '\0' || port == 0){
        MessagesBox::warn(nullptr, SOCKET_SERVER_FORMAT_ERROR);
        delta_note_error("%s", "server and port parser error!")
        return;
    }

    if(!isIPAddr(server)){
        MessagesBox::warn(nullptr, SOCKET_SERVER_ADDR_ERROR);
    }

    // store log info
    strcpy(g_server, server);
    g_port = port;
}

bool ConnectCtrl::checkUserIDValied(){
    if(strlen(g_username) == 0 || strlen(g_passwd) == 0){
        delta_note_warn("%s", "username or password is null")
        return false;
    }
    return true;
}
