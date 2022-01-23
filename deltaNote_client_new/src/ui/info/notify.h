#ifndef NOTIFY_H
#define NOTIFY_H

#include <QDialog>
#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>

#include "inter_var.h"
#include "setting_ctrl.h"

namespace Ui {
class notify;
}

class notify : public QDialog
{
    Q_OBJECT

public:
    explicit notify(
            QWidget *parent = nullptr,
            QString _message = "未定义错误",
            SettingCtrl *setting_ctrl = nullptr,
            bool parent_main = false);
    ~notify();

private slots:
    void on_OK_clicked();

private:
    Ui::notify *ui;

    int m_mouse_click_x;
    int m_mouse_click_y;

    // 父窗口是主界面
    bool m_parent_main;
    // 配置
    SettingCtrl *m_setting_ctrl;
    // 环境
    QRect    m_desktop_rect;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

class MessagesBox {
public:
    static void info(QWidget *parent, QString message, SettingCtrl *setting_ctrl, bool parent_main=false);
    static void warn(QWidget *parent, QString message, SettingCtrl *setting_ctrl, bool parent_main=false);
    static void error(QWidget *parent, QString message, SettingCtrl *setting_ctrl, bool parent_main=false);
};

#endif // NOTIFY_H
