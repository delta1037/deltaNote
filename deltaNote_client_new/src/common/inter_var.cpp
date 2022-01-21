#include "inter_var.h"

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
    }else if(op_type == OpType_chk){
        return "chk";
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
    }else if(op_type == "chk"){
        return OpType_chk;
    }
    return OpType_nul;
}

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
    font_color = "#00ffff00";
    icon_color = "#00ffff00";;

    // 窗体定位信息
    x_pos = 600;
    y_pos = 300;
    height = 560;
    width = 300;
}
