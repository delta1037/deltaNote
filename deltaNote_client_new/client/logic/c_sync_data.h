#ifndef CLIENT_SYNC_DATA_H
#define CLIENT_SYNC_DATA_H

#include "http.h"
#include "setting_ctrl.h"
#include "c_data_ctrl.h"
#include "json.h"

// 与服务器进行同步
// 同步逻辑可复用
class CSyncData {
public:
    CSyncData(SettingCtrl *setting_ctrl, CDataCtrl *data_ctrl);

    // 注册
    int sync_sign_up(SyncStatus &net_status, ErrorCode &error_code);
    // 登录
    int sync_sign_in(SyncStatus &net_status, ErrorCode &error_code);
    // 同步数据
    int sync_data(SyncStatus &net_status, ErrorCode &error_code);
private:
    // 上传
    int sync_upload(SyncStatus &net_status, ErrorCode &error_code);
    // 下载
    int sync_download(SyncStatus &net_status, ErrorCode &error_code);

private:
    static Json::Value json_list(const TodoList &todo_list);
    static void json_list(TodoList &todo_list, const Json::Value &json_list);
private:
    HTTP *m_net_handle;
    SettingCtrl *m_setting_ctrl;
    CDataCtrl *m_data_ctrl;
};
#endif //CLIENT_SYNC_DATA_H
