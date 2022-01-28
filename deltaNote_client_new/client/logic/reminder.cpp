#include <QSystemTrayIcon>
#include "reminder.h"
#include "log.h"

Reminder::Reminder(CDataCtrl *data_ctrl, SettingCtrl *setting_ctrl, QSystemTrayIcon *tray_icon) {
    m_data_ctrl = data_ctrl;
    m_setting_ctrl = setting_ctrl;
    m_tray_icon = tray_icon;
}

void Reminder::register_notify(const std::string &create_key, const std::string &reminder){
    // 时间戳转化为数值
    m_register_map_lock.lock();
    auto it = m_register_map.find(create_key);
    if(it != m_register_map.end()){
        d_logic_warn("create key %s has been add to map", create_key.c_str())
        m_register_map_lock.unlock();
        return;
    }
    int time_s = time_int_s(reminder);
    m_register_map[create_key] = time_s;
    m_register_map_lock.unlock();
    d_logic_debug("create key %s:%d add to map, map size:%d", create_key.c_str(), time_s, m_register_map.size())
}

void Reminder::clear_registered() {
    m_register_map_lock.lock();
    m_register_map.clear();
    m_register_map_lock.unlock();
}

bool Reminder::check_and_notify() {
    bool ret_val = false;
    d_logic_debug("%s", "check and send notify to user")
    int now_time_s = int(get_time_of_s());
    int now_time_idx = 60 - (now_time_s % 60);
    // TODO 这里要不要加个锁？按理说程序运行是单线程的，不需要考虑锁的问题（这里流程需要再确认一下）
    m_register_map_lock.lock();
    for(const auto &it : m_register_map){
        // 发过通知了就把发过的时间清空
        if(now_time_s <= it.second - now_time_idx ){ // 在同一分钟内的全都通知
            //时间还没到
            d_logic_debug("time is under remind,%d < %d", now_time_s, it.second)
            continue;
        }
        // 时间到了，获取指定的数据，向系统发通知
        ErrorCode error_code;
        TodoList ret_list;
        int ret = m_data_ctrl->sel_todo(ListType_UI, it.first, ret_list, error_code);
        if(ret == SQLITE_ERROR){
            d_logic_error("%s", "Reminder sel_todo error")
            continue;
        }
        d_logic_debug("%s", "Reminder sel_todo success")

        if(ret_list.size() != 1){
            d_logic_warn("%s", "ret_list size != 1")
        }
        TodoItem item_data = *(ret_list.begin());
        // 校验是否需要发送通知
        if(item_data.reminder.empty()){
            d_logic_warn("create key %s reminder is null!!!", item_data.create_key.c_str())
            continue;
        }

        // 通知内容
        m_tray_icon->showMessage("TODO", QString::fromStdString(item_data.data), QSystemTrayIcon::Warning);
        ret_val = true;
        // 修改指定的数据，把reminder清理掉
        item_data.reminder  = ""; // 偷偷改了
        ret = m_data_ctrl->alt_todo(item_data, error_code);
        if(ret == SQLITE_ERROR){
            d_logic_error("%s", "Reminder alt_todo error")
            continue;
        }
        d_logic_debug("%s", "Reminder alt_todo success")
    }
    m_register_map_lock.unlock();
    return ret_val;
}
