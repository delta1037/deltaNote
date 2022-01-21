#include "deltaNote/listCtrl.h"
#include "common/socket.h"

bool ListControl::todoListChange = false;
int ListControl::opListSize = 0;
std::vector<MsgOpPack> ListControl::m_undoList;
std::vector<MsgOpPack> ListControl::m_doneList;
std::vector<MsgOpPack> ListControl::m_opList;

std::unordered_map<TIME_STAMP, unsigned int> ListControl::m_undoListMap;
std::unordered_map<TIME_STAMP, unsigned int> ListControl::m_doneListMap;

ListControl::ListControl()
{

}

void ListControl::opListAdd(MsgOpPack &msg_pack){
    m_opList.push_back(msg_pack);
}

void ListControl::undoListAdd(MsgOpPack &msg_pack){
    todoListChange = true;
    m_undoList.push_back(msg_pack);
    m_undoListMap.insert(std::make_pair(msg_pack.createTime, m_undoList.size()-1));
}

void ListControl::undoListAddNew(MsgOpPack &msg_pack){
    msg_pack.createTime = std::time(nullptr);
    msg_pack.operateTime = msg_pack.createTime;
    msg_pack.operation = TODO_ADD;
    if(msg_pack.data.size() > 0){
        undoListAdd(msg_pack);
        opListAdd(msg_pack);
    }
}

void ListControl::doneListAdd(MsgOpPack &msg_pack){
    todoListChange = true;
    m_doneList.push_back(msg_pack);
    m_doneListMap.insert(std::make_pair(msg_pack.createTime, m_doneList.size()-1));
}

void ListControl::todoListModify(MsgOpPack & msg_pack){
    if(msg_pack.isCheck != Checked){
        auto it = m_undoListMap.find(msg_pack.createTime);
        m_undoList[it->second].operateTime = std::time(nullptr);
        m_undoList[it->second].operation = msg_pack.operation;
        m_undoList[it->second].isCheck = msg_pack.isCheck;
        m_undoList[it->second].operation = TODO_ALTER;
        m_undoList[it->second].data = msg_pack.data;
        opListAdd(m_undoList[it->second]);
    }
}

void ListControl::undoMoveToDone(MsgOpPack & msg_pack){
    msg_pack.isCheck = Checked;
    msg_pack.operation = TODO_CHECK;
    msg_pack.operateTime = std::time(nullptr);
    doneListAdd(msg_pack);

    auto it = m_undoListMap.find(msg_pack.createTime);
    m_undoList.erase(m_undoList.begin() + it->second);
    opListAdd(msg_pack);
}

void ListControl::doneMoveToUndo(MsgOpPack & msg_pack){
    msg_pack.isCheck = UnCheck;
    msg_pack.operation = TODO_CHECK;
    msg_pack.operateTime = std::time(nullptr);
    undoListAdd(msg_pack);

    auto it = m_doneListMap.find(msg_pack.createTime);
    m_doneList.erase(m_doneList.begin() + it->second);
    opListAdd(msg_pack);
}

void ListControl::clearDoneList(){
    delta_note_debug("%s", "list control clear done list, ready to upload to server");

    for(auto it : m_doneList){
        it.operation = TODO_DEL;
        it.operateTime = std::time(nullptr);
        opListAdd(it);
    }
    todoListChange = true;
    m_doneList.clear();
    m_doneListMap.clear();
}

MsgOpPack ListControl::getLastOfDone(){
    if(0 == m_doneList.size()){
        MsgOpPack nullPack;
        return nullPack;
    }else{
        return m_doneList[m_doneList.size() - 1];
    }
}
MsgOpPack ListControl::getLastOfUndo(){
    if(0 == m_undoList.size()){
        MsgOpPack nullPack;
        return nullPack;
    }else{
        return m_undoList[m_undoList.size() - 1];
    }
}
std::vector<MsgOpPack> &ListControl::getUndoList(){
    return m_undoList;
}

std::vector<MsgOpPack> &ListControl::getDoneList(){
    return m_doneList;
}

std::vector<MsgOpPack> &ListControl::getOpList(){
    return m_opList;
}

void ListControl::loadFromServer(){
    delta_note_debug("%s", "list control start load from server");
    if(!isLogin){
        delta_note_debug("%s", "list control load from server, not login and load from local");
        return;
    }
    if(m_opList.size() > 0){
        // 首先上传本地的记录
        if(!uploadToServer()){
            return;
        }
    }

    ConnectCtrl loadControl;
    std::vector<SocketMsgOpPack> loadPacks;
    MsgStatus loadStatus = loadControl.loadFromServer(loadPacks);
    if(loadStatus != PullSuccess){
        delta_note_error("list control load from server fail, status code : %d", (int)loadStatus);
        return;
    }

    for(const auto &it : loadPacks){
        m_opList.emplace_back(it);
    }

    delta_note_debug("list control load from server success : %d", m_opList.size());

    m_undoList.clear();
    m_undoListMap.clear();
    m_doneList.clear();
    m_doneListMap.clear();

    todoListChange = true;

    for(auto it : m_opList){
        if(it.isCheck == Checked){
            doneListAdd(it);
        }else{
            undoListAdd(it);
        }
    }
    m_opList.clear();
}

bool ListControl::uploadToServer(){
    delta_note_debug("%s", "list control upload todo to server");
    if(!isLogin){
        // 没有登录就只存储在本地的oplist中
        delta_note_debug("%s", "list control not login, ready save to local");
        return false;
    }

    if(m_opList.size() == 0){
        delta_note_debug("%s", "list control upload todo to server size = 0");
        return false;
    }

    std::vector<SocketMsgOpPack> uploadPacks;
    for(const auto &it: m_opList){
        uploadPacks.emplace_back(it);
    }

    ConnectCtrl uploadControl;
    MsgStatus uploadStatus = uploadControl.uploadToServer(uploadPacks);
    if(uploadStatus == PushSuccess){
        delta_note_debug("list control upload to server success : %d", m_opList.size());
        m_opList.clear();
        return true;
    }else{
        delta_note_error("list control upload to server fail, status code : %d", (int)uploadStatus);
        return false;
    }
}

