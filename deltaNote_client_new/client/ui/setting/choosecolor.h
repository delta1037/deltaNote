#ifndef CHOOSECOLOR_H
#define CHOOSECOLOR_H

#include <QDialog>
#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>

#include "setting_ctrl.h"

namespace Ui {
class chooseColor;
}

class chooseColor : public QDialog
{
    Q_OBJECT

public:
    explicit chooseColor(QWidget *parent = nullptr, SettingCtrl *setting_ctrl = nullptr);
    ~chooseColor();
    QColor get_color();

private slots:
    void on_color_1_1_clicked();

    void on_color_1_2_clicked();

    void on_color_1_3_clicked();

    void on_color_2_1_clicked();

    void on_color_2_2_clicked();

    void on_color_2_3_clicked();

    void on_color_1_4_clicked();

    void on_color_2_4_clicked();

private:
    Ui::chooseColor *ui;
    QColor choosed_color;

    // 配置
    SettingCtrl *setting_ctrl;
    // 环境
    QRect    m_desktop_rect;

    int m_mouse_click_x;
    int m_mouse_click_y;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // CHOOSECOLOR_H
