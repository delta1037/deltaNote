#include "inter_var.h"

NoteSetting::NoteSetting() {
    username = "";
    password = "";

    server = "127.0.0.1:8888"; // ip:port

    is_login = false;
    is_lock = false;
    is_auto_start = false;

    font_size = 11; // 字体大小配置
    tran_pos = 200;  // 透明度记录

    // 颜色名称记录
    font_color = "#ffff00";
    bg_color = "#ffff00";
    icon_color = "#ffff00";

    // 窗体定位信息
    x_pos = 600;
    y_pos = 300;
    height = 560;
    width = 300;
}

std::string is_check_str(IsCheck is_check){
    if(is_check == Check_true){
        return "true";
    } else if(is_check == Check_false){
        return "false";
    }
    return "null";
}

IsCheck is_check_enum(const std::string &is_check){
    if(is_check == "true"){
        return Check_true;
    }else if(is_check == "false"){
        return Check_false;
    }
    return Check_null;
}

std::string op_type_str(OpType op_type){
    if(op_type == OpType_add){
        return "add";
    }else if(op_type == OpType_del){
        return "del";
    }else if(op_type == OpType_alt){
        return "alt";
    }
    return "nul";
}

OpType op_type_enum(const std::string &op_type){
    if(op_type == "add"){
        return OpType_add;
    }else if(op_type == "del"){
        return OpType_del;
    }else if(op_type == "alt"){
        return OpType_alt;
    }
    return OpType_nul;
}

std::string tag_type_str(TagType tag_type){
    if(tag_type == TagType_low){
        return "3-low";
    }else if(tag_type == TagType_mid){
        return "2-mid";
    }else if(tag_type == TagType_high){
        return "1-high";
    }
    return "4-nul";
}

TagType tag_type_enum(const std::string &tag_type){
    if(tag_type == "3-low"){
        return TagType_low;
    }else if(tag_type == "2-mid"){
        return TagType_mid;
    }else if(tag_type == "1-high"){
        return TagType_high;
    }
    return TagType_nul;
}

TodoItem::TodoItem(){
    this->create_key = "";
    this->edit_key = "";
    this->op_type = OpType_nul;
    this->is_check = Check_false;
    this->tag_type = TagType_nul;
    this->reminder = "";
    this->data = "";
}

TodoItem::TodoItem(
        const std::string &create_key,
        const std::string &edit_key,
        OpType op_type,
        IsCheck is_check,
        TagType tag_type,
        const std::string &reminder,
        const std::string &data
        ){
    this->create_key = create_key;
    this->edit_key = edit_key;
    this->op_type = op_type;
    this->is_check = is_check;
    this->tag_type = tag_type;
    this->reminder = reminder;
    this->data = data;
}

bool check_item_valid(const TodoItem &item){
    if(item.create_key.empty()){
        return false;
    }
    if(item.edit_key.empty()){
        return false;
    }
    if(item.is_check == Check_null){
        return false;
    }
    if(item.op_type == OpType_nul){
        return false;
    }
    return true;
}
