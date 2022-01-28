#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QPaintEvent>
#include <QBitmap>
#include <QColorDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QPainter>

#include "newuser.h"
#include "choosecolor.h"
#include "../utils/svg_color.h"
#include "setting_ctrl.h"
#include "c_sync_data.h"

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(
            QWidget *parent = nullptr,
            SettingCtrl *setting_ctrl = nullptr,
            CSyncData *sync_data = nullptr);
    ~login();

private slots:
    void on_cancel_clicked();

    void on_Login_clicked();

    void on_creteNewUser_clicked();

    void on_chooseFontColor_clicked();

    void on_chooseIconColor_clicked();

    void on_choose_bg_color_clicked();

    void on_transparent_sliderMoved(int position);

    void on_radioButton_clicked(bool checked);

    void on_exit_clicked();

    void on_fontSizeSlider_sliderMoved(int position);

    void on_mainWinWidthSlider_sliderMoved(int position);

private:
    Ui::login *ui;
    int ret;

    double m_mouse_click_x;
    double m_mouse_click_y;

    // 配置
    SettingCtrl *m_setting_ctrl;
    // 同步控制
    CSyncData *m_sync_data;
    // 环境
    QRect    m_desktop_rect;

private:
    void set_value();
    void refresh_icon();
    void refresh_text();
    void refresh_background();
private:
    void doLogin();
    void doLogout();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // LOGIN_H
