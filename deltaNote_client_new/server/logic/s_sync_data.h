#ifndef SERVER_SYNC_DATA_H
#define SERVER_SYNC_DATA_H

#include <string>
#include "inter_var.h"

// 定义处理接口
class SSyncData{
public:
    // 处理注册
    static int sync_sign_up(const std::string& req, std::string& res, ErrorCode &error_code);

    // 处理登录
    static int sync_sign_in(const std::string& req, std::string& res, ErrorCode &error_code);

    // 处理上传
    static int sync_upload(SyncStatus &net_status, ErrorCode &error_code);
    // 处理下载
    static int sync_download(SyncStatus &net_status, ErrorCode &error_code);

private:

};

#endif //SERVER_SYNC_DATA_H
