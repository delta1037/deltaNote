#ifndef SQLITE_TEST_SYNC_DATA_H
#define SQLITE_TEST_SYNC_DATA_H

#include "network.h"
#include "setting_ctrl.h"
#include "data_ctrl.h"
#include "json.h"

// 与服务器进行同步
// 同步逻辑可复用
class SyncData {
public:
    SyncData(SettingCtrl *setting_ctrl, DataCtrl *data_ctrl);

    // json格式封装
    // 注册
    int sync_register(NetStatus net_status, ErrorCode error_code);
    // 登录
    int sync_login(NetStatus net_status, ErrorCode error_code);
    // 同步数据
    int sync_data(NetStatus net_status, ErrorCode error_code);
private:
    // 上传
    int sync_upload(NetStatus net_status, ErrorCode error_code);
    // 下载
    int sync_download(NetStatus net_status, ErrorCode error_code);

private:
    static Json::Value json_list(const TodoList &todo_list);
    static void json_list(TodoList &todo_list, const Json::Value &json_list);
private:
    Network *m_net_handle;
    SettingCtrl *m_setting_ctrl;
    DataCtrl *m_data_ctrl;
};
#endif //SQLITE_TEST_SYNC_DATA_H
