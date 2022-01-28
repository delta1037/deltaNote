#ifndef C_INTER_VAR_H
#define C_INTER_VAR_H
#include <ctime>
#include <sys/time.h>
#include <string>

#include "c_language.h"
#include "inter_var.h"

/*************************客户端相关变量*********************************/
/* 定时器 */
// 自动同步时间间隔 5min
#define TIMER_REFRESH   (1000*60*5)
// 提醒时间间隔 1min
#define TIMER_REMINDER  (1000*60)
/* 客户端版本： 3.1 */
#define MAIN_VERSION 3
#define NEXT_VERSION 1
#define VERSION_ID ((MAIN_VERSION * 10) + NEXT_VERSION)

/* 客户端类型 */
#define DEVICE_TYPE_MOD 1000

/* 客户端注册表信息，在Linux客户端中被隐藏 */
#define REGEDIT_AUTO_START_PATH "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define REGEDIT_KEY "deltaNote"

/* 更新程序名字 */
#ifdef LINUX_CLIENT
#define UPDATE_PROGRAM "updateDeltaNote"
#endif
#ifdef WINDOW_CLIENT
#define UPDATE_PROGRAM "updateDeltaNote.exe"
#endif

#define RET_SUCCESS (0)
#define RET_FAILED (-1)

// 一些表的名字，数据库的名称定义
#define DB_NAME "delta_note_db"
#define DB_SETTING_TABLE_NAME "setting_table"
#define DB_TODO_UI_TABLE_NAME "todo_ui_table"
#define DB_TODO_OP_TABLE_NAME "todo_op_table"


#define SETTING_USERNAME        "username"
#define SETTING_PASSWORD        "password"
#define SETTING_SERVER          "server"
#define SETTING_IS_LOGIN        "is_login"
#define SETTING_IS_LOCK         "is_lock"
#define SETTING_IS_AUTO_START   "is_auto_start"
#define SETTING_FONT_SIZE       "font_size"
#define SETTING_TRAN_POS        "tran_pos"
#define SETTING_FONT_COLOR      "font_color"
#define SETTING_BG_COLOR        "bg_color"
#define SETTING_ICON_COLOR      "icon_color"
#define SETTING_X_POS           "x_pos"
#define SETTING_Y_POS           "y_pos"
#define SETTING_HEIGHT          "height"
#define SETTING_WIDTH           "width"
struct NoteSetting{
    std::string username;
    std::string password;

    std::string server; // ip:port

    bool is_login;
    bool is_lock;
    bool is_auto_start;

    int font_size; // 字体大小配置
    int tran_pos;  // 透明度记录

    // 颜色名称记录
    std::string font_color;
    std::string bg_color;
    std::string icon_color;

    // 窗体定位信息
    int    x_pos;
    int    y_pos;
    int    height;
    int    width;

    NoteSetting();
};



#endif //C_INTER_VAR_H
