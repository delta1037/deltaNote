#include "c_inter_var.h"

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
