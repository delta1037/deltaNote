#include "sync_data.h"
#include "base64.h"
#include "log.h"

SyncData::SyncData(SettingCtrl *setting_ctrl, DataCtrl *data_ctrl){
    this->m_setting_ctrl = setting_ctrl;
    this->m_data_ctrl = data_ctrl;
}

int SyncData::sync_register(NetStatus net_status, ErrorCode error_code) {
    // 从配置获取数据
    return 0;
}

int SyncData::sync_login(NetStatus net_status, ErrorCode error_code) {
    return 0;
}

int SyncData::sync_data(NetStatus net_status, ErrorCode error_code) {
    return 0;
}

int SyncData::sync_upload(NetStatus net_status, ErrorCode error_code) {
    return 0;
}

int SyncData::sync_download(NetStatus net_status, ErrorCode error_code) {
    return 0;
}

Json::Value SyncData::json_list(const TodoList &todo_list) {
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

void SyncData::json_list(TodoList &todo_list, const Json::Value &json_list) {
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
