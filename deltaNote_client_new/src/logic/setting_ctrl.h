//
// Created by delta1037 on 2022/1/21.
//

#ifndef SQLITE_TEST_SETTING_CTRL_H
#define SQLITE_TEST_SETTING_CTRL_H

#include <QColor>
#include "inter_var.h"
#include "sql_setting.h"

class SettingCtrl {
public:
    SettingCtrl();
    ~SettingCtrl();

    void load_all_setting();

    std::string get_string(const std::string &name, bool deep=false);
    void set_string(const std::string &name, const std::string &value);

    int get_int(const std::string &name, bool deep= false);
    void set_int(const std::string &name, int value);

    QColor get_color(const std::string &name, bool deep=false);
    void set_color(const std::string &name, const QColor &);

    bool get_bool(const std::string &name, bool deep=false);
    void set_bool(const std::string &name, bool);
private:
    // 从数据库加载数据
    void load_sql_data();
    // 更新数据
    void update_setting(const std::string &name, const std::string &value);

    // 转换函数
    static int str_int(const std::string &str);
    static std::string str_int(int val);
    static bool str_bool(const std::string &str);
    static std::string str_bool(bool val);

private:
    NoteSetting note_setting;
    SqlSetting *sql_setting;
};


#endif //SQLITE_TEST_SETTING_CTRL_H
