#include "data_ctrl.h"
#include "log.h"

using namespace std;

DataCtrl::DataCtrl(){
    op_list = new SqlTodoList(DB_NAME, DB_TODO_OP_TABLE_NAME);
    ui_list = new SqlTodoList(DB_NAME, DB_TODO_UI_TABLE_NAME);
}

DataCtrl::~DataCtrl(){
    delete op_list;
    delete ui_list;
}

int DataCtrl::add_todo(const string &create_key, const string &data, ErrorCode &error_code) {
    // 操作流中新增该记录（创建时间key值保证该便签key不会与之前的key重复）
    string edit_key = get_time_key();
    int ret = op_list->add(create_key, edit_key, OpType_add, Check_false, data, error_code);
    if(ret == SQLITE_ERROR){
        d_logic_error("DataCtrl key %s add op_list error", create_key.c_str())
        return RET_FAILED;
    }
    d_logic_debug("DataCtrl key %s add op_list success", create_key.c_str())

    // UI表中新增数据
    ret = ui_list->add(create_key, edit_key, OpType_add, Check_false, data, error_code);
    if(ret == SQLITE_ERROR){
        d_logic_error("DataCtrl key %s add ui_list error", create_key.c_str())
        return RET_FAILED;
    }
    d_logic_debug("DataCtrl key %s add ui_list success", create_key.c_str())
    return RET_SUCCESS;
}

int DataCtrl::alt_todo(const std::string &create_key, IsCheck is_check, const std::string &data, ErrorCode &error_code) {
    /*
     * 1、若操作流中没有该key值，说明是第一次操作，将修改记录附加到操作流中；
     * 2、若操作流中已经存在该key值，说明已经不是第一次修改，将之前的修改记录覆盖为现在的修改记录
     * （即修改记录只会存在一条，并且是最新的操作）
     */
    // 操作流表
    string edit_key = get_time_key();
    TodoList ret_list;
    int ret = op_list->sel(create_key, OpType_alt, ret_list, error_code);
    if(ret == SQLITE_ERROR){
        d_logic_error("DataCtrl key %s sel op_list error", create_key.c_str())
        return RET_FAILED;
    }
    d_logic_debug("DataCtrl key %s sel op_list success", create_key.c_str())
    if(ret_list.empty()){
        // 如果还没有则插入一条新的修改记录
        ret = op_list->add(create_key, edit_key, OpType_alt, is_check, data, error_code);
        if(ret == SQLITE_ERROR){
            d_logic_error("DataCtrl key %s add op_list error", create_key.c_str())
            return RET_FAILED;
        }
        d_logic_debug("DataCtrl key %s add op_list success", create_key.c_str())
    }else{
        // 如果已存在则修改现有记录(根据创建时间和操作类型修改，别把add类型覆盖了)
        ret = op_list->alt(create_key, OpType_alt, edit_key, is_check, data, error_code);
        if(ret == SQLITE_ERROR){
            d_logic_error("DataCtrl key %s alt op_list error", create_key.c_str())
            return RET_FAILED;
        }
        d_logic_debug("DataCtrl key %s alt op_list success", create_key.c_str())
    }

    // UI界面表
    ret_list.clear(); // 注意清理环境
    ret = ui_list->sel(create_key, ret_list, error_code);
    if(ret == SQLITE_ERROR){
        d_logic_error("DataCtrl key %s sel ui_list error", create_key.c_str())
        return RET_FAILED;
    }
    d_logic_debug("DataCtrl key %s sel ui_list success", create_key.c_str())
    if(ret_list.empty()){
        // 如果还没有则插入一条新的记录
        ret = ui_list->add(create_key, edit_key, OpType_alt, is_check, data, error_code);
        if(ret == SQLITE_ERROR){
            d_logic_error("DataCtrl key %s add ui_list error", create_key.c_str())
            return RET_FAILED;
        }
        d_logic_debug("DataCtrl key %s add ui_list success", create_key.c_str())
    }else{
        // 如果已存在则修改现有记录(根据创建时间修改)
        ret = ui_list->alt(create_key, edit_key, OpType_alt, is_check, data, error_code);
        if(ret == SQLITE_ERROR){
            d_logic_error("DataCtrl key %s alt ui_list error", create_key.c_str())
            return RET_FAILED;
        }
        d_logic_debug("DataCtrl key %s alt ui_list success", create_key.c_str())
    }
    return RET_SUCCESS;
}

int DataCtrl::del_todo(const std::string &create_key, ErrorCode &error_code) {
    /**
     * 1、如果操作流中有该key值的新增操作，则直接将所有相同的key值便签删除（新增记录和修改记录）；
     * 2、如果操作流中不存在该key值的新增操作，说明该key值的新增操作已经上传到服务端了，则先将该操作保存到操作流中，等待同步
     * （如果服务端永久失联则会称为一条僵尸记录）
     */
    // 操作流表
    string edit_key = get_time_key();
    TodoList ret_list;
    int ret = op_list->sel(create_key, OpType_add, ret_list, error_code);
    if(ret == SQLITE_ERROR){
        d_logic_error("DataCtrl key %s sel op_list error", create_key.c_str())
        return RET_FAILED;
    }
    d_logic_debug("DataCtrl key %s sel op_list success", create_key.c_str())
    if(ret_list.empty()){
        // 不存在新增操作
        ret = op_list->add(create_key, edit_key, OpType_del, Check_true, "", error_code);
        if(ret == SQLITE_ERROR){
            d_logic_error("DataCtrl key %s add op_list error", create_key.c_str())
            return RET_FAILED;
        }
        d_logic_debug("DataCtrl key %s add op_list success", create_key.c_str())
    }else{
        // 如果存在新增操作，删除所有内容
        ret = op_list->del(create_key, error_code);
        if(ret == SQLITE_ERROR){
            d_logic_error("DataCtrl key %s alt op_list error", create_key.c_str())
            return RET_FAILED;
        }
        d_logic_debug("DataCtrl key %s alt op_list success", create_key.c_str())
    }

    // UI表操作，删除相关内容即可
    ret = ui_list->del(create_key, error_code);
    if(ret == SQLITE_ERROR){
        d_logic_error("DataCtrl key %s alt ui_list error", create_key.c_str())
        return RET_FAILED;
    }
    d_logic_debug("DataCtrl key %s alt ui_list success", create_key.c_str())
    return RET_SUCCESS;
}

int DataCtrl::sel_todo(ListType list_type, TodoList &ret_list, ErrorCode &error_code) {
    if(list_type == ListType_OP){
        int ret = op_list->sel(ret_list, error_code);
        if(ret == SQLITE_ERROR){
            d_logic_error("%s", "DataCtrl sel op_list error")
            return RET_FAILED;
        }
        d_logic_debug("%s", "DataCtrl sel op_list success")
    }else if(list_type == ListType_UI){
        int ret = ui_list->sel(ret_list, error_code);
        if(ret == SQLITE_ERROR){
            d_logic_error("%s", "DataCtrl sel ui_list error")
            return RET_FAILED;
        }
        d_logic_debug("%s", "DataCtrl sel ui_list success")
    }else{
        d_logic_error("DataCtrl sel type %d error", list_type)
        return RET_FAILED;
    }
    return RET_SUCCESS;
}
