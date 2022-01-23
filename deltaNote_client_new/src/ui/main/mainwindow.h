#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QSystemTrayIcon>
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QUrl>
#include <QMenu>
#include <QListWidget>
#include <QCloseEvent>
#include <QTimer>
#include <QProcess>

#include "login.h"
#include "todolistitem.h"
//#include "info/checkupdate.h"
#include "data_ctrl.h"
#include "sync_data.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 编辑结束处理
    void ui_alt_todo(const std::string&, AltType);

private slots:
    void on_setting_clicked();

    void on_refresh_clicked();

    void on_lock_clicked();

    void on_history_clicked();

    void do_add_clicked();

    void do_open_official();

    void do_check_update();

    void do_clear_done();

    void on_ToDoListWin_customContextMenuRequested(const QPoint &pos);

#ifdef LINUX_CLIENT
    void dataAudoSave();
#endif
protected:
    // 关闭
    void closeEvent(QCloseEvent *e) override;
    // 界面绘制
    void paintEvent(QPaintEvent *event = nullptr);
    // 鼠标监听事件
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent*event);

private:
    // 初始化一些设置
    void event_connect();
    void set_background();
    void set_menu();
    void set_mouse();

    // 鼠标监测相关
    void detect_edge();
    void mouse_reset();

    // 界面新增一条
    void add_new_todo_item(TodoItem *item_data = nullptr);
    // 数据库删除相关内容
    void del_todo_item(TodoItem *item_data);

    // 刷新界面列表
    int refresh_list_from_db();
    // 刷新背景
    void refresh_background();

    Ui::MainWindow *ui;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *action_official;
    QAction *action_setting;
    QAction *action_quit;
    QAction *action_clear;
    QAction *action_update;

    QTimer *refreshTimer;
    QTimer *uploadTimer;

private:
    // 数据控制
    DataCtrl m_data_ctrl;

    // 列表切换控制
    bool m_history_switch;
    // 列表控制（方便检索，用map类型）
    std::map<std::string, ToDoListItem*> m_list_map;

    // 配置控制
    SettingCtrl m_setting_ctrl;
    // 同步控制
    SyncData *m_sync_data;

    // 界面控制
    bool m_location_change;
    bool m_is_show_history;

    // 环境
    QRect    m_desktop_rect;

    // 鼠标相关
    int m_edge_margin;     //鼠标检测的边缘距离
    double m_mouse_click_x{};
    double m_mouse_click_y{};
    enum {
        Dir_None = 0x01,
        Dir_Down = 0x02
    }m_dir_resize; //更改尺寸的方向

    // 界面刷新控制
    long long last_refresh_time;
#ifdef LINUX_CLIENT
    QTimer *dataSaveTimer;
    bool settingChange;
#endif
};
#endif // MAINWINDOW_H
