#ifndef CLIENT_REMINDER_H
#define CLIENT_REMINDER_H
#include <QMutex>
#include <string>
#include <map>
#include <qsystemtrayicon.h>
#include "c_data_ctrl.h"
#include "setting_ctrl.h"

class Reminder {
public:
    // data_ctrl 是为了获取到需要通知的数据，setting_ctrl是为了获取到提前通知时间，目前无用
    Reminder(CDataCtrl *data_ctrl, SettingCtrl *setting_ctrl, QSystemTrayIcon *tray_icon);

    // 注册提醒时间
    void register_notify(const std::string &create_key, const std::string &reminder);
    void clear_registered();

    // 检查过期时间并发起通知,发了通知返回true，否则返回false
    bool check_and_notify();

private:
    CDataCtrl *m_data_ctrl;
    SettingCtrl *m_setting_ctrl;
    QSystemTrayIcon *m_tray_icon;
    std::map<std::string, int> m_register_map; // 创建时间key值和时间戳
    QMutex m_register_map_lock;
};

#endif //CLIENT_REMINDER_H
