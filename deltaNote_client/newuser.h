#ifndef NEWUSER_H
#define NEWUSER_H

#include <QDialog>

#include "untils.h"


namespace Ui {
class newUser;
}

class newUser : public QDialog
{
    Q_OBJECT

public:
    explicit newUser(QWidget *parent = nullptr);
    ~newUser();

private slots:
    void on_ok_clicked();

    void on_cancel_clicked();

private:
    Ui::newUser *ui;
    int ret;
};

#endif // NEWUSER_H
