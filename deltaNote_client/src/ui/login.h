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

#include "ui/newuser.h"
#include "ui/choosecolor.h"
#include "common/graphicscolorsvgitem.h"

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_cancel_clicked();

    void on_Login_clicked();

    void on_creteNewUser_clicked();

    void on_chooseFontColor_clicked();

    void on_chooseIconColor_clicked();

    void on_transparent_sliderMoved(int position);

    void on_radioButton_clicked(bool checked);

    void on_exit_clicked();

    void on_fontSizeSlider_sliderMoved(int position);

    void on_mainWinWidthSlider_sliderMoved(int position);

private:
    Ui::login *ui;
    int ret;

    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;

public:
    void refreshBackground();
private:
    void doLogin();
    void doLogout();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // LOGIN_H
