#include "../../include/deltaNote/packCtrl.h"

bool FixedPackCtrl::msgSend(MsgPack &msgPack) {
    return false;
}

/**
 * 登录 新建用户 接收操作记录
 * @param msgPack
 * @return
 */
bool FixedPackCtrl::msgRecv(MsgPack &msgPack) {
    return false;
}

bool DynamicPackCtrl::msgSend(MsgPack &msgPack) {
    msgPack.setTodoDeviceType(SERVER_DEVICE);
    msgPack.setTodoVersion(VERSION_ID);

    if(msgPack.getMsgType() == ACK_STATUS_TYPE){
        // set head
        msgPack.setMsgBodySize(MSG_BODY_ACK_SIZE);

        // send head
        //LogCtrl::debug("send head addr:%p", &msgPack.getMsgHead());
        if(!sendMsg(&msgPack.getMsgHead(), MSG_HEAD_SIZE)){
            LogCtrl::error("pack control send head error");
            return false;
        }

        // send ack
        //LogCtrl::debug("send ack addr:%p size : %d", &msgPack.getMsgBodyAck(), sizeof(&msgPack.getMsgBodyAck()));
        if(!sendMsg(&msgPack.getMsgBodyAck(), MSG_BODY_ACK_SIZE)){
            LogCtrl::error("pack control send ack error");
            return false;
        }
        //LogCtrl::debug("send ack addr:%p success", &msgPack.getMsgBodyAck());
    }else if(msgPack.getMsgType() == OP_PULL_TYPE){
        // set head
        msgPack.setMsgBodySize(MSG_BODY_OP_SIZE + msgPack.getOpQueueSize() * MSG_OP_PACK_SIZE);

        // send head
        if(!sendMsg(&msgPack.getMsgHead(), MSG_HEAD_SIZE)){
            LogCtrl::error("pack control send head error");
            return false;
        }

        // send body
        if(!sendMsg(&msgPack.getMsgBodyOp(), MSG_BODY_OP_SIZE)){
            LogCtrl::error("pack control send operation body error");
            return false;
        }

        // send queue, after use msgPack.setOperationQueue
        if(!sendMsg(msgPack.getMsgBodyOp().operationQueue, msgPack.getOpQueueSize() * MSG_OP_PACK_SIZE)){
            LogCtrl::error("send operation queue error");
            return false;
        }
    }else if(msgPack.getMsgType() == UPDATE_GET_LINK_TYPE){
        // set head
        msgPack.setMsgBodySize(MSG_BODY_DOWNLOAD_SIZE + msgPack.getDownloadMessageSize());

        // send head
        if(!sendMsg(&msgPack.getMsgHead(), MSG_HEAD_SIZE)){
            LogCtrl::error("pack control send head error");
            return false;
        }

        // send body
        if(!sendMsg(&msgPack.getMsgBodyDownLoad(), MSG_BODY_DOWNLOAD_SIZE)){
            LogCtrl::error("pack control send operation body error");
            return false;
        }
        LogCtrl::debug("update link:%s", msgPack.getDownloadLink());

        if(msgPack.getDownloadMessageSize() == 0){
            LogCtrl::debug("latest version, nothing to send");
            return true;
        }

        LogCtrl::debug("send body message: %s", msgPack.getMsgBodyDownLoad().updateMessage);
        // message
        if(!sendMsg(msgPack.getMsgBodyDownLoad().updateMessage, msgPack.getDownloadMessageSize())){
            LogCtrl::error("send download message error");
            return false;
        }
    }else{
        LogCtrl::error("send msg type not define : %c", msgPack.getMsgType());
        msgPack.setMsgState(OperateNotDefine);
        return false;
    }
    return true;
}

bool DynamicPackCtrl::msgRecv(MsgPack &msgPack) {
    //LogCtrl::debug("recv head addr:%p", &msgPack.getMsgHead());
    //LogCtrl::debug("recv ack addr:%p", &msgPack.getMsgBodyAck());

    /* get pack head */
    if(!recvMsg(&msgPack.getMsgHead(), MSG_HEAD_SIZE)){
        msgPack.setMsgState(RecvSizeError);
        LogCtrl::error("recv head size error");
        return false;
    }
    
    if (msgPack.getMsgType() == UPDATE_GET_LINK_TYPE) {
        return true;
    } 

    /* check pack version */
    if(msgPack.getTodoVersion() != VERSION_ID){
        msgPack.setMsgState(VersionError);
        LogCtrl::error("recv head's version error");
        return false;
    }

    if(msgPack.getMsgType() == OP_LOGIN_TYPE
            || msgPack.getMsgType() == OP_CREATE_USER_TYPE
            || msgPack.getMsgType() == OP_PULL_TYPE){
        /* check pack size */
        if(msgPack.getMsgBodySize() != MSG_BODY_OP_SIZE){
            LogCtrl::error("check pack size %d != %d", msgPack.getMsgHead().msgBodySize, (int)MSG_BODY_OP_SIZE);
            msgPack.setMsgState(RecvSizeError);
            return false;
        }

        /* recv body */
        if(!recvMsg(&msgPack.getMsgBodyOp(), MSG_BODY_OP_SIZE)){
            msgPack.setMsgState(RecvSizeError);
            return false;
        }

        /* check user id null*/
        if(strlen(msgPack.getUserID().userName) == 0 || strlen(msgPack.getUserID().passWord) == 0){
            LogCtrl::error("null username or null passwd");
            msgPack.setMsgState(UserIDNull);
            return false;
        }
    } else if (msgPack.getMsgType() == OP_PUSH_TYPE){
        if(!recvMsg(&msgPack.getMsgBodyOp(), sizeof(struct MsgBodyOp))){
            msgPack.setMsgState(RecvSizeError);
            return false;
        }

        /* check pack size */
        if(msgPack.getMsgBodySize() !=
                MSG_BODY_OP_SIZE + msgPack.getOpQueueSize() * MSG_OP_PACK_SIZE){
            LogCtrl::error("check pack size %d != %d", msgPack.getMsgHead().msgBodySize, MSG_BODY_OP_SIZE + msgPack.getOpQueueSize() * MSG_OP_PACK_SIZE);
            msgPack.setMsgState(RecvSizeError);
            return false;
        }

        msgPack.getMsgBodyOp().operationQueue = nullptr;
        if(msgPack.getOpQueueSize() == 0){
            return true;
        }

        msgPack.getMsgBodyOp().operationQueue = new SocketMsgOpPack[msgPack.getOpQueueSize()];
        if(!recvMsg(msgPack.getMsgBodyOp().operationQueue, msgPack.getOpQueueSize() * MSG_OP_PACK_SIZE)){
            LogCtrl::error("recv queue size error");
            msgPack.setMsgState(RecvSizeError);
            return false;
        }
    } else {
        msgPack.setMsgState(OperateNotDefine);
        return false;
    }

    return true;
}

PackCtrlFactory::~PackCtrlFactory() = default;
