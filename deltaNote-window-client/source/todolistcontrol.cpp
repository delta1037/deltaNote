#include "todolistcontrol.h"
#include "socket.h"

MsgOpPack::MsgOpPack(const SocketMsgOpPack &msg){
    this->operation = msg.operation;
    this->isCheck = msg.isCheck;
    this->createTime = atoll(msg.createTimestamp);
    this->operateTime = atoll(msg.operateTimestamp);
    this->data = msg.data; // char -> QString
}

int TodoListControl::opListSize = 0;
std::vector<MsgOpPack> TodoListControl::m_undoList;
std::vector<MsgOpPack> TodoListControl::m_doneList;
std::vector<MsgOpPack> TodoListControl::m_opList;
TodoListControl::TodoListControl()
{

}

void TodoListControl::opListAdd(MsgOpPack &msg_pack){
    m_opList.push_back(msg_pack);

    // upload 完成之前是不会再push_back的，所以是安全的
    uploadToServer();
}

void TodoListControl::undoListAdd(MsgOpPack &msg_pack){
    m_undoList.push_back(msg_pack);
}

void TodoListControl::undoListAddNew(MsgOpPack &msg_pack){
    msg_pack.createTime = std::time(nullptr);
    msg_pack.operateTime = msg_pack.createTime;
    msg_pack.operation = TODO_ADD;
    if(msg_pack.data.size() > 0){
        undoListAdd(msg_pack);
        opListAdd(msg_pack);
    }
}

void TodoListControl::doneListAdd(MsgOpPack &msg_pack){
    m_doneList.push_back(msg_pack);
}

void TodoListControl::todoListModify(MsgOpPack & msg_pack){
    for(auto it : (msg_pack.isCheck == Checked ? m_doneList : m_undoList)){
        if( it.createTime == msg_pack.createTime ){
            it.operateTime = std::time(nullptr);
            it.operation = msg_pack.operation;
            it.isCheck = msg_pack.isCheck;
            it.operation = TODO_ALTER;
            it.data = msg_pack.data;
            opListAdd(it);
            break;
        }
    }
}

void TodoListControl::undoMoveToDone(MsgOpPack & msg_pack){
    msg_pack.isCheck = Checked;
    msg_pack.operation = TODO_CHECK;
    msg_pack.operateTime = std::time(nullptr);
    doneListAdd(msg_pack);

    for(auto it = m_undoList.begin(); it != m_undoList.end(); ++it){
        if(it->createTime == msg_pack.createTime){
            m_undoList.erase(it);
            break;
        }
    }

    opListAdd(msg_pack);
}

void TodoListControl::doneMoveToUndo(MsgOpPack & msg_pack){
    msg_pack.isCheck = UnCheck;
    msg_pack.operation = TODO_CHECK;
    msg_pack.operateTime = std::time(nullptr);
    undoListAdd(msg_pack);

    for(auto it = m_doneList.begin(); it != m_doneList.end(); ++it){
        if(it->createTime == msg_pack.createTime){
            m_doneList.erase(it);
            break;
        }
    }

    opListAdd(msg_pack);
}

void TodoListControl::clearDoneList(){
    for(auto it : m_doneList){
        it.operation = TODO_DEL;
        it.operateTime = std::time(nullptr);
        m_opList.push_back(it);
        LOG_INFO("add to op list")
    }
    m_doneList.clear();
    uploadToServer();
}

MsgOpPack TodoListControl::getLastOfDone(){
    if(0 == m_doneList.size()){
        MsgOpPack nullPack;
        return nullPack;
    }else{
        return m_doneList[m_doneList.size() - 1];
    }
}
MsgOpPack TodoListControl::getLastOfUndo(){
    if(0 == m_undoList.size()){
        MsgOpPack nullPack;
        return nullPack;
    }else{
        return m_undoList[m_undoList.size() - 1];
    }
}
std::vector<MsgOpPack> &TodoListControl::getUndoList(){
    return m_undoList;
}

std::vector<MsgOpPack> &TodoListControl::getDoneList(){
    return m_doneList;
}

void TodoListControl::loadFromServer(){
    LOG_INFO("load from server")
    if(!isLogin){
        LOG_INFO("not login, load from local")
        return;
    }
    if(m_opList.size() > 0){
        // 首先上传本地的记录
        if(!uploadToServer()){
            return;
        }
    }

    ServerConnectControl control;
    if(!control.loadFromServer(m_opList)){
        m_opList.clear();
        return;
    }

    LOG_INFO("load from server success : %d", m_opList.size())

    m_undoList.clear();
    m_doneList.clear();

    for(auto it : m_opList){
        if(it.isCheck == Checked){
            m_doneList.push_back(it);
        }else{
            m_undoList.push_back(it);
        }
    }
    m_opList.clear();
}

bool TodoListControl::uploadToServer(){
    LOG_INFO("syn with server")
    if(!isLogin){
        // 没有登录就只存储在本地的oplist中
        LOG_INFO("not login, ready save to local")
        return false;
    }
    ServerConnectControl control;
    if(control.uploadToServer(m_opList)){
        LOG_INFO("upload to server success : %d", m_opList.size())
        m_opList.clear();
        return true;
    }else{
        LOG_ERROR("upload to server fail")
        return false;
    }
}
