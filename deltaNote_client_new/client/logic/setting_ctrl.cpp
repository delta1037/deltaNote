#include "log.h"
#include "setting_ctrl.h"
SettingCtrl::SettingCtrl(){
    sql_setting = new SqlKeyValue(DB_NAME, DB_SETTING_TABLE_NAME);
}

SettingCtrl::~SettingCtrl(){
    delete sql_setting;
}

void SettingCtrl::load_all_setting(){
    load_sql_data();
}

int SettingCtrl::str_int(const std::string &str) {
    return atol(str.c_str());
}
std::string SettingCtrl::str_int(int val) {
    return std::to_string(val);
}

bool SettingCtrl::str_bool(const std::string &str) {
    if(str == "true"){
        return true;
    }else{
        return false;
    }
}
std::string SettingCtrl::str_bool(bool val) {
    if(val){
        return "true";
    }else{
        return "false";
    }
}

void SettingCtrl::load_sql_data() {
    ErrorCode error_code;
    KeyValueList setting_list;
    int ret =  sql_setting->sel(setting_list, error_code);
    if(ret != RET_SUCCESS){
        d_logic_error("load data from %s:%s error", DB_NAME, DB_SETTING_TABLE_NAME)
        return;
    }

    for(const auto& item:setting_list){
        update_setting(item.key, item.value);
    }
}

void SettingCtrl::update_setting(const std::string &name, const std::string &value) {
    if(name == SETTING_USERNAME){
        this->note_setting.username = value;
    }else if(name == SETTING_PASSWORD){
        this->note_setting.password = value;
    }else if(name == SETTING_SERVER){
        this->note_setting.server = value;
    }else if(name == SETTING_FONT_COLOR){
        this->note_setting.font_color = value;
    }else if(name == SETTING_ICON_COLOR){
        this->note_setting.icon_color = value;
    }else if(name == SETTING_BG_COLOR){
        this->note_setting.bg_color = value;
    }else if(name == SETTING_FONT_SIZE){
        this->note_setting.font_size = str_int(value);
    }else if(name == SETTING_TRAN_POS){
        this->note_setting.tran_pos = str_int(value);
    }else if(name == SETTING_X_POS){
        this->note_setting.x_pos = str_int(value);
    }else if(name == SETTING_Y_POS){
        this->note_setting.y_pos = str_int(value);
    }else if(name == SETTING_HEIGHT){
        this->note_setting.height = str_int(value);
    }else if(name == SETTING_WIDTH){
        this->note_setting.width = str_int(value);
    }else if(name == SETTING_IS_LOGIN){
        this->note_setting.is_login = str_bool(value);
    }else if(name == SETTING_IS_LOCK){
        this->note_setting.is_lock = str_bool(value);
    }else if(name == SETTING_IS_AUTO_START){
        this->note_setting.is_auto_start = str_bool(value);
    }else{
        d_logic_warn("setting %s:%s cant recognize", name.c_str(), value.c_str())
    }
}

std::string SettingCtrl::get_string(const std::string &name, bool deep) {
    std::string ret_val;
    if(name == SETTING_USERNAME){
        ret_val = this->note_setting.username;
    }else if(name == SETTING_PASSWORD){
        ret_val = this->note_setting.password;
    }else if(name == SETTING_SERVER) {
        ret_val = this->note_setting.server;
    }else{
        d_logic_warn("unknown setting %s", name.c_str())
    }
    if(!deep){
        return ret_val;
    }
    ErrorCode error_code;
    std::string ret_str;
    int ret = sql_setting->sel(name, ret_str, error_code);
    if(ret != RET_SUCCESS){
        d_logic_error("load setting %s from %s:%s error", name.c_str(), DB_NAME, DB_SETTING_TABLE_NAME)
        return ret_val;
    }
    if(ret_str.empty()){
        d_logic_warn("setting %s value is null", name.c_str())
        return ret_val;
    }

    // update
    update_setting(name, ret_str);

    return ret_str;
}

void SettingCtrl::set_string(const std::string &name, const std::string &value) {
    // update
    update_setting(name, value);

    ErrorCode error_code;
    int ret = sql_setting->add(name, value, error_code);
    if(ret != RET_SUCCESS){
        d_logic_error("set setting %s to %s:%s error", name.c_str(), DB_NAME, DB_SETTING_TABLE_NAME)
        return;
    }
}

int SettingCtrl::get_int(const std::string &name, bool deep) {
    uint32_t ret_val = 0;
    if(name == SETTING_FONT_SIZE){
        ret_val = this->note_setting.font_size;
    }else if(name == SETTING_TRAN_POS){
        ret_val = this->note_setting.tran_pos;
    }else if(name == SETTING_X_POS){
        ret_val = this->note_setting.x_pos;
    }else if(name == SETTING_Y_POS){
        ret_val = this->note_setting.y_pos;
    }else if(name == SETTING_HEIGHT){
        ret_val = this->note_setting.height;
    }else if(name == SETTING_WIDTH){
        ret_val = this->note_setting.width;
    }else{
        d_logic_warn("unknown setting %s", name.c_str())
    }
    if(!deep){
        return ret_val;
    }
    ErrorCode error_code;
    std::string ret_str;
    int ret = sql_setting->sel(name, ret_str, error_code);
    if(ret != RET_SUCCESS){
        d_logic_error("load setting %s from %s:%s error", name.c_str(), DB_NAME, DB_SETTING_TABLE_NAME)
        return ret_val;
    }
    if(ret_str.empty()){
        d_logic_warn("setting %s value is null", name.c_str())
        return ret_val;
    }

    // update
    update_setting(name, ret_str);

    return str_int(ret_str);
}

void SettingCtrl::set_int(const std::string &name, int value) {
    // update
    update_setting(name, str_int(value));

    ErrorCode error_code;
    int ret = sql_setting->add(name, str_int(value), error_code);
    if(ret != RET_SUCCESS){
        d_logic_error("set setting %s to %s:%s error", name.c_str(), DB_NAME, DB_SETTING_TABLE_NAME)
        return;
    }
}

QColor SettingCtrl::get_color(const std::string &name, bool deep) {
    QColor color = QColor(252, 233, 79);
    if(name == SETTING_FONT_COLOR){
        color = QColor(QString::fromStdString(this->note_setting.font_color));
    }else if(name == SETTING_ICON_COLOR){
        color = QColor(QString::fromStdString(this->note_setting.icon_color));
    }else if(name == SETTING_BG_COLOR){
        color = QColor(QString::fromStdString(this->note_setting.bg_color));
    }else{
        d_logic_warn("unknown setting %s", name.c_str())
    }
    if(!deep){
        return color;
    }
    ErrorCode error_code;
    std::string ret_str;
    int ret = sql_setting->sel(name, ret_str, error_code);
    if(ret != RET_SUCCESS){
        d_logic_error("load setting %s from %s:%s error", name.c_str(), DB_NAME, DB_SETTING_TABLE_NAME)
        return color;
    }
    if(ret_str.empty()){
        d_logic_warn("setting %s value is null", name.c_str())
        return color;
    }
    color = QColor(QString::fromStdString(ret_str));

    // update
    update_setting(name, ret_str);

    return color;
}

void SettingCtrl::set_color(const std::string &name, const QColor &value) {
    // update
    update_setting(name, value.name().toStdString());

    ErrorCode error_code;
    int ret = sql_setting->add(name, value.name().toStdString(), error_code);
    if(ret != RET_SUCCESS){
        d_logic_error("set setting %s to %s:%s error", name.c_str(), DB_NAME, DB_SETTING_TABLE_NAME)
        return;
    }
}

bool SettingCtrl::get_bool(const std::string &name, bool deep) {
    bool ret_val = false;
    if(name == SETTING_IS_LOGIN){
        ret_val = this->note_setting.is_login;
    }else if(name == SETTING_IS_LOCK){
        ret_val = this->note_setting.is_lock;
    }else if(name == SETTING_IS_AUTO_START){
        ret_val = this->note_setting.is_auto_start;
    }else{
        d_logic_warn("unknown setting %s", name.c_str())
    }
    if(!deep){
        return ret_val;
    }

    ErrorCode error_code;
    std::string ret_str;
    int ret = sql_setting->sel(name, ret_str, error_code);
    if(ret != RET_SUCCESS){
        d_logic_error("load setting %s from %s:%s error", name.c_str(), DB_NAME, DB_SETTING_TABLE_NAME)
        return ret_val;
    }
    if(ret_str.empty()){
        d_logic_warn("setting %s value is null", name.c_str())
        return ret_val;
    }

    // update
    update_setting(name, ret_str);

    return str_bool(ret_str);
}

void SettingCtrl::set_bool(const std::string &name, bool value) {
    // update
    update_setting(name, str_bool(value));

    ErrorCode error_code;
    int ret = sql_setting->add(name, str_bool(value), error_code);
    if(ret != RET_SUCCESS){
        d_logic_error("set setting %s to %s:%s error", name.c_str(), DB_NAME, DB_SETTING_TABLE_NAME)
        return;
    }
}
