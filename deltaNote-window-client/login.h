#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QColorDialog>
#include <QMessageBox>
#include "newuser.h"

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

private:
    Ui::login *ui;
    int ret;
};

#endif // LOGIN_H
