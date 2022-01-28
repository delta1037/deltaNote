#include "s_data_ctrl.h"
#include "log.h"

using namespace std;

SDataCtrl::SDataCtrl(){

}

SDataCtrl::~SDataCtrl(){

}

int SDataCtrl::add_todo(const string &create_key, const string &data, ErrorCode &error_code) {

    return RET_SUCCESS;
}

int SDataCtrl::alt_todo(const std::string &create_key, IsCheck is_check, const std::string &data, ErrorCode &error_code) {

    return RET_SUCCESS;
}

int SDataCtrl::del_todo(const std::string &create_key, ErrorCode &error_code) {
    return RET_SUCCESS;
}

int SDataCtrl::sel_todo(TodoList &ret_list, ErrorCode &error_code) {

    return RET_SUCCESS;
}
