#ifndef NOTIFY_H
#define NOTIFY_H

#include <QDialog>
#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>

#include "untils.h"

namespace Ui {
class notify;
}

class notify : public QDialog
{
    Q_OBJECT

public:
    explicit notify(QWidget *parent = nullptr, QString _message = "未定义错误");
    ~notify();

private slots:
    void on_OK_clicked();

private:
    Ui::notify *ui;

protected:
    void paintEvent(QPaintEvent *);
};

class MessagesBox {
public:
    static void warn(QWidget *parent, QString message);
    static void error(QWidget *parent, QString message);
};

#endif // NOTIFY_H
