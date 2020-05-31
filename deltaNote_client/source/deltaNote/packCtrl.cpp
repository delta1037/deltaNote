#include "deltaNote/packCtrl.h"

bool DynamicPackCtrl::msgSend(MsgPack &msgPack) {
    msgPack.setTodoVersion(VERSION_ID);
#ifdef LINUX_CLIENT
    msgPack.setTodoDeviceType(LINUX_DEVICE);
#endif
#ifdef WINDOW_CLIENT
    msgPack.setTodoDeviceType(WINDOW_DEVICE);
#endif

    if(msgPack.getMsgType() == OP_LOGIN_TYPE
            || msgPack.getMsgType() == OP_CREATE_USER_TYPE
            || msgPack.getMsgType() == OP_PULL_TYPE){
        // set head
        msgPack.setMsgBodySize(MSG_BODY_OP_SIZE);

        // send head
        if(!sendMsg(&msgPack.getMsgHead(), MSG_HEAD_SIZE)){
            LogCtrl::debug("pack control send head error");
            return false;
        }

        // set body
        struct UserID userID(g_username, g_passwd);
        msgPack.setUserID(userID);

        // send body
        if(!sendMsg(&msgPack.getMsgBodyOp(), MSG_BODY_OP_SIZE)){
            LogCtrl::debug("pack control send operation body error");
            return false;
        }
    } else if (msgPack.getMsgType() == OP_PUSH_TYPE){
        // set head
        msgPack.setMsgBodySize(MSG_BODY_OP_SIZE + msgPack.getOpQueueSize() * MSG_OP_PACK_SIZE);

        // send head
        if(!sendMsg(&msgPack.getMsgHead(), MSG_HEAD_SIZE)){
            LogCtrl::debug("pack control send head error");
            return false;
        }

        // set body
        struct UserID userID(g_username, g_passwd);
        msgPack.setUserID(userID);

        // send body
        if(!sendMsg(&msgPack.getMsgBodyOp(), MSG_BODY_OP_SIZE)){
            LogCtrl::debug("pack control send operation body error");
            return false;
        }

        if(msgPack.getOpQueueSize() == 0){
            LogCtrl::debug("nothing to send");
            return true;
        }

        // send queue, after use msgPack.setOperationQueue
        if(!sendMsg(msgPack.getMsgBodyOp().operationQueue, msgPack.getOpQueueSize() * MSG_OP_PACK_SIZE)){
            LogCtrl::debug("send operation queue error");
            return false;
        }
    } else if (msgPack.getMsgType() == UPDATE_GET_LINK_TYPE) {
        msgPack.setMsgBodySize(0);
        if(!sendMsg(&msgPack.getMsgHead(), MSG_HEAD_SIZE)){
            LogCtrl::debug("pack control get update link error");
            return false;
        }
    } else {
        LogCtrl::error("unknown pack operation : %c", msgPack.getMsgType());
        return false;
    }

    return true;
}

bool DynamicPackCtrl::msgRecv(MsgPack &msgPack) {
    /* get pack head */
    if(!recvMsg(&msgPack.getMsgHead(), MSG_HEAD_SIZE)){
        LogCtrl::debug("pack control recv pack head error");
        msgPack.setMsgState(RecvSizeError);
        return false;
    }

    /* check pack version */
    if(msgPack.getMsgType() != UPDATE_GET_LINK_TYPE
            && msgPack.getTodoVersion() != VERSION_ID){
        LogCtrl::debug("pack control recv pack version check error, %d != %d",
                       msgPack.getTodoVersion(),
                       VERSION_ID);
        msgPack.setMsgState(VersionError);
        return false;
    }

    if(msgPack.getMsgType() == ACK_STATUS_TYPE){
        /* check pack size */
        if(msgPack.getMsgBodySize() != MSG_BODY_ACK_SIZE){
            LogCtrl::debug("get ack, check pack size %d != %d",
                           msgPack.getMsgHead().msgBodySize, MSG_BODY_ACK_SIZE);
            msgPack.setMsgState(RecvSizeError);
            return false;
        }

        if(!recvMsg(&msgPack.getMsgBodyAck(), MSG_BODY_ACK_SIZE)){
            LogCtrl::debug("get ack, receive ack size error");
            msgPack.setMsgState(RecvSizeError);
            return false;
        }
    }else if(msgPack.getMsgType() == OP_PULL_TYPE){
        if(!recvMsg(&msgPack.getMsgBodyOp(), MSG_BODY_OP_SIZE)){
            msgPack.setMsgState(RecvSizeError);
            return false;
        }

        /* check pack size */
        if(msgPack.getMsgBodySize() !=
                MSG_BODY_OP_SIZE + msgPack.getOpQueueSize() * MSG_OP_PACK_SIZE){
            LogCtrl::debug("pull todo, check recv pack size error %d != %d",
                           msgPack.getMsgHead().msgBodySize,
                           MSG_BODY_OP_SIZE + msgPack.getOpQueueSize() * MSG_OP_PACK_SIZE);
            msgPack.setMsgState(RecvSizeError);
            return false;
        }

        msgPack.getMsgBodyOp().operationQueue = nullptr;
        if(msgPack.getOpQueueSize() == 0){
            LogCtrl::debug("pack control, nothing to pull");
            return true;
        }

        msgPack.getMsgBodyOp().operationQueue = new SocketMsgOpPack[msgPack.getOpQueueSize()];
        if(!recvMsg(msgPack.getMsgBodyOp().operationQueue, msgPack.getOpQueueSize() * MSG_OP_PACK_SIZE)){
            LogCtrl::debug("pull todo, recv operation queue size error");
            msgPack.setMsgState(RecvSizeError);
            return false;
        }
    }else if(msgPack.getMsgType() == UPDATE_GET_LINK_TYPE){
        /* get download pack */
        if(!recvMsg(&msgPack.getMsgBodyDownLoad(), MSG_BODY_DOWNLOAD_SIZE)){
            LogCtrl::debug("get download pack, size error");
            msgPack.setMsgState(RecvSizeError);
            return false;
        }

        /* check pack size */
        if(msgPack.getMsgBodySize() != MSG_BODY_DOWNLOAD_SIZE + msgPack.getDownloadMessageSize()){
            LogCtrl::debug("get download, check pack size %d != %d",
                           msgPack.getMsgHead().msgBodySize,
                           MSG_BODY_DOWNLOAD_SIZE + msgPack.getDownloadMessageSize());
            msgPack.setMsgState(RecvSizeError);
            return false;
        }

        if(msgPack.getDownloadMessageSize() == 0
                && msgPack.getTodoVersion() % DEVICE_TYPE_MOD == VERSION_ID){
            msgPack.getMsgBodyDownLoad().updateMessage = nullptr;
            msgPack.setMsgState(AlreadyLatest);
            return true;
        }

        /* get download message */
        LogCtrl::debug("log message size:%d", msgPack.getDownloadMessageSize());
        msgPack.getMsgBodyDownLoad().updateMessage = new char[msgPack.getDownloadMessageSize()+1];
        if(!recvMsg(msgPack.getMsgBodyDownLoad().updateMessage , msgPack.getDownloadMessageSize())){
            LogCtrl::debug("get download, receive ack size error");
            msgPack.setMsgState(RecvSizeError);
            return false;
        }
    }else{
        LogCtrl::error("get msg type error");
    }
    return true;
}

PackCtrlFactory::~PackCtrlFactory() = default;
