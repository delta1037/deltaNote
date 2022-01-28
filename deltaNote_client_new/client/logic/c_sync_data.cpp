#include "c_sync_data.h"
#include "base64.h"
#include "log.h"

CSyncData::CSyncData(SettingCtrl *setting_ctrl, CDataCtrl *data_ctrl){
    this->m_setting_ctrl = setting_ctrl;
    this->m_data_ctrl = data_ctrl;

    this->m_net_handle = new HTTP(HTTP_client, "http://192.168.0.106:1234");
}

int CSyncData::sync_sign_up(SyncStatus &net_status, ErrorCode &error_code) {
    // TODO 从配置获取数据
    // 封装需要发送的数据
    Json::Value t_register;
    t_register["username"] = m_setting_ctrl->get_string(SETTING_USERNAME);
    t_register["password"] = m_setting_ctrl->get_string(SETTING_PASSWORD);
    std::string t_req = t_register.asString();
    d_logic_debug("sync_sign_up send data:%s", t_req.c_str())

    std::string t_res;
    int ret = m_net_handle->c_get(SYNC_SIGN_UP, t_req, t_res, error_code);
    if(ret == RET_FAILED){
        net_status = Sync_sign_up_undefined_error;
        d_logic_error("sync_sign_up send %s to get data error", t_req.c_str())
        return RET_FAILED;
    }
    d_logic_debug("sync_sign_up recv data:%s", t_res.c_str())

    Json::Value t_json_res;
    Json::Reader t_reader;
    t_reader.parse(t_res, t_json_res);
    net_status = (SyncStatus)t_json_res.get("status", Json::Value(int(Sync_sign_up_undefined_error))).asInt();
    return RET_SUCCESS;
}

int CSyncData::sync_sign_in(SyncStatus &net_status, ErrorCode &error_code) {
    // TODO 从配置获取数据
    Json::Value t_register;
    t_register["username"] = m_setting_ctrl->get_string(SETTING_USERNAME);
    t_register["password"] = m_setting_ctrl->get_string(SETTING_PASSWORD);
    std::string t_req = t_register.toStyledString();
    d_logic_debug("sync_sign_in send data:%s", t_req.c_str())

    std::string t_res;
    int ret = m_net_handle->c_get(SYNC_SIGN_IN, t_req, t_res, error_code);
    if(ret == RET_FAILED){
        net_status = Sync_login_undefined_error;
        d_logic_error("sync_sign_in send %s to get data error", t_req.c_str())
        return RET_FAILED;
    }
    d_logic_debug("sync_sign_in recv data:%s", t_res.c_str())

    Json::Value t_json_res;
    Json::Reader t_reader;
    t_reader.parse(t_res, t_json_res);
    net_status = (SyncStatus)t_json_res.get("status", Json::Value(int(Sync_login_undefined_error))).asInt();
    return RET_SUCCESS;
}

int CSyncData::sync_data(SyncStatus &net_status, ErrorCode &error_code) {
    // 同步操作先上传再下载
    if(RET_FAILED == sync_upload(net_status, error_code)){
        return RET_FAILED;
    }
    if(RET_FAILED == sync_download(net_status, error_code)){
        return RET_FAILED;
    }
    return RET_SUCCESS;
}

int CSyncData::sync_upload(SyncStatus &net_status, ErrorCode &error_code) {
    // TODO 从配置获取数据
    // 封装需要发送的数据
    Json::Value t_register;
    t_register["username"] = m_setting_ctrl->get_string(SETTING_USERNAME);
    t_register["password"] = m_setting_ctrl->get_string(SETTING_PASSWORD);
    TodoList todo_list;
    m_data_ctrl->sel_todo(ListType_OP, todo_list, error_code);
    t_register["todo_list"] = json_list(todo_list);
    std::string t_req = t_register.asString();
    d_logic_debug("sync_sign_up send data:%s", t_req.c_str())

    std::string t_res;
    int ret = m_net_handle->c_get(SYNC_UPLOAD, t_req, t_res, error_code);
    if(ret == RET_FAILED){
        net_status = Sync_login_undefined_error;
        d_logic_error("sync_upload send %s to get data error", t_req.c_str())
        return RET_FAILED;
    }
    d_logic_debug("sync_upload recv data:%s", t_res.c_str())

    Json::Value t_json_res;
    Json::Reader t_reader;
    t_reader.parse(t_res, t_json_res);
    net_status = (SyncStatus)t_json_res.get("status", Json::Value(int(Sync_login_undefined_error))).asInt();
    return RET_SUCCESS;
}

int CSyncData::sync_download(SyncStatus &net_status, ErrorCode &error_code) {
    // TODO 从配置获取数据
    // 封装需要发送的数据
    Json::Value t_register;
    t_register["username"] = m_setting_ctrl->get_string(SETTING_USERNAME);
    t_register["password"] = m_setting_ctrl->get_string(SETTING_PASSWORD);
    std::string t_req = t_register.asString();
    d_logic_debug("sync_sign_up send data:%s", t_req.c_str())

    std::string t_res;
    int ret = m_net_handle->c_get(SYNC_DOWNLOAD, t_req, t_res, error_code);
    if(ret == RET_FAILED){
        net_status = Sync_login_undefined_error;
        d_logic_error("sync_upload send %s to get data error", t_req.c_str())
        return RET_FAILED;
    }
    d_logic_debug("sync_upload recv data:%s", t_res.c_str())

    Json::Value t_json_res;
    Json::Reader t_reader;
    t_reader.parse(t_res, t_json_res);
    net_status = (SyncStatus)t_json_res.get("status", Json::Value(int(Sync_login_undefined_error))).asInt();

    // TODO 解析接收到的数据,填充到数据库中
    return RET_SUCCESS;
}

Json::Value CSyncData::json_list(const TodoList &todo_list) {
    // 将列表封装为json的格式[{},{},{}]
    Json::Value ret_json;
    for(const auto &it : todo_list){
        Json::Value one_todo;
        one_todo[TODO_ITEM_CREATE_KEY] = it.create_key;
        one_todo[TODO_ITEM_EDIT_KEY] = it.edit_key;
        one_todo[TODO_ITEM_OP_TYPE] = op_type_str(it.op_type);
        one_todo[TODO_ITEM_IS_CHECK] = is_check_str(it.is_check);
        one_todo[TODO_ITEM_DATA] = it.data;

        ret_json.append(one_todo);
    }
    d_logic_debug("list to json, size:%d", ret_json.size())
    // 格式：[{},{},{}]
    return ret_json;
}

void CSyncData::json_list(TodoList &todo_list, const Json::Value &json_list) {
    // 将json解析为list的格式
    for(int idx = 0; idx < json_list.size(); idx++){
        Json::Value one_todo = json_list[0];
        TodoItem one_item;
        one_item.create_key = one_todo.get(TODO_ITEM_CREATE_KEY, "").asString();
        one_item.edit_key = one_todo.get(TODO_ITEM_EDIT_KEY, "").asString();
        one_item.op_type = op_type_enum(one_todo.get(TODO_ITEM_OP_TYPE, "").asString());
        one_item.is_check = is_check_enum(one_todo.get(TODO_ITEM_IS_CHECK, "").asString());
        one_item.data = one_todo.get(TODO_ITEM_DATA, "").asString();

        if(check_item_valid(one_item)){
            todo_list.emplace_back(one_item);
            d_logic_debug("todo valid, create_key:%s,edit_key:%s,op_type:%s,check_type:%s,data:%s",
                          one_item.create_key.c_str(),
                          one_item.edit_key.c_str(),
                          op_type_str(one_item.op_type).c_str(),
                          is_check_str(one_item.is_check).c_str(),
                          one_item.data.c_str())
        }else{
            d_logic_error("todo is invalid, create_key:%s,edit_key:%s,op_type:%s,check_type:%s,data:%s",
                          one_item.create_key.c_str(),
                          one_item.edit_key.c_str(),
                          op_type_str(one_item.op_type).c_str(),
                          is_check_str(one_item.is_check).c_str(),
                          one_item.data.c_str())
        }
    }
    d_logic_debug("json to list, size:%d", todo_list.size())
}
