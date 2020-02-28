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
#include "graphicscolorsvgitem.h"
#include "choosecolor.h"

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

    void on_clearData_clicked();

    void on_radioButton_clicked(bool checked);

    void on_exit_clicked();

private:
    Ui::login *ui;
    int ret;

public:
    void refreshBackground();
private:
    void doLogin();
    void doLogout();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // LOGIN_H
