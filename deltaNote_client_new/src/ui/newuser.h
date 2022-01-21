#ifndef NEWUSER_H
#define NEWUSER_H

#include <QDialog>

#include <QPaintEvent>
#include <QBitmap>

#include "ui/notify.h"
#include "common/graphicscolorsvgitem.h"
#include "deltaNote/connectctrl.h"

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

    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;

private:
    void refreshBackground();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // NEWUSER_H
