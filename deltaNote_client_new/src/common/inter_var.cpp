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


