#include "log.h"
#include "json.h"
#include "s_sync_data.h"

int SSyncData::sync_sign_up(const std::string &req, std::string &res, ErrorCode &error_code) {
    return RET_SUCCESS;
}

int SSyncData::sync_sign_in(const std::string &req, std::string &res, ErrorCode &error_code) {
    // 登录，校验用户名和密码
    return RET_SUCCESS;
}

int SSyncData::sync_upload(SyncStatus &net_status, ErrorCode &error_code) {
    return RET_SUCCESS;
}

int SSyncData::sync_download(SyncStatus &net_status, ErrorCode &error_code) {
    return RET_SUCCESS;
}
