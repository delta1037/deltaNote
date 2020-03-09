#ifndef NEWUSER_H
#define NEWUSER_H

#include <QDialog>

#include <QPaintEvent>
#include <QBitmap>

#include "log.h"
#include "socket.h"
#include "notify.h"
#include "graphicscolorsvgitem.h"

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

private:
    void refreshBackground();

protected:
    void paintEvent(QPaintEvent *);
};

#endif // NEWUSER_H
