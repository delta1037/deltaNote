#ifndef TODOLISTCONTROL_H
#define TODOLISTCONTROL_H

#include <QString>
#include <vector>
#include <ctime>
#include <unordered_map>

#include "deltaNote/connectctrl.h"

class ListControl
{
private:
    static int opListSize;
    static std::vector<MsgOpPack> m_opList;
    static std::vector<MsgOpPack> m_undoList;
    static std::vector<MsgOpPack> m_doneList;

    static std::unordered_map<TIME_STAMP, unsigned int> m_undoListMap;
    static std::unordered_map<TIME_STAMP, unsigned int> m_doneListMap;
public:
    static bool todoListChange;
    ListControl();

    static void opListAdd(MsgOpPack & msg_pack);
    static void undoListAdd(MsgOpPack & msg_pack);
    static void undoListAddNew(MsgOpPack & msg_pack);      // 新建并填写内容
    static void doneListAdd(MsgOpPack & msg_pack);      // 新建并填写内容

    static void todoListModify(MsgOpPack & msg_pack);    // 修改

    static void undoMoveToDone(MsgOpPack & msg_pack);    // 选择
    static void doneMoveToUndo(MsgOpPack & msg_pack);

    static void clearDoneList();

    static MsgOpPack getLastOfDone();
    static MsgOpPack getLastOfUndo();

    static std::vector<MsgOpPack> &getUndoList();
    static std::vector<MsgOpPack> &getDoneList();
    static std::vector<MsgOpPack> &getOpList();

    // 从服务端拉取所有数据，分类放到undo和done里面
    static void loadFromServer();

    // 向服务端推送数据，把操作队列的数据全推上去
    static bool uploadToServer();
};

#endif // TODOLISTCONTROL_H
