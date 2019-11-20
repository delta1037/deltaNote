#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
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

private:
    Ui::login *ui;
    int ret;
};

#endif // LOGIN_H
