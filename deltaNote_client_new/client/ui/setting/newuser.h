#ifndef NEWUSER_H
#define NEWUSER_H

#include <QDialog>

#include <QPaintEvent>
#include <QBitmap>

#include "notify.h"
#include "../utils/svg_color.h"
#include "setting_ctrl.h"
#include "c_sync_data.h"

namespace Ui {
class newUser;
}

class newUser : public QDialog
{
    Q_OBJECT

public:
    explicit newUser(QWidget *parent = nullptr,
                     SettingCtrl *setting_ctrl = nullptr,
                     CSyncData *sync_data = nullptr);
    ~newUser();

private slots:
    void on_ok_clicked();

    void on_cancel_clicked();

private:
    Ui::newUser *ui;
    int ret;

    // 配置
    SettingCtrl *m_setting_ctrl;
    // 同步控制
    CSyncData *m_sync_data;

    int m_mouse_click_x;
    int m_mouse_click_y;

private:
    void refreshBackground();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // NEWUSER_H
