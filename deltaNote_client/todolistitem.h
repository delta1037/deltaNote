#ifndef TODOLISTITEM_H
#define TODOLISTITEM_H

#include <QWidget>
#include <QPainter>
#include <QTextCodec>
#include <QMessageBox>

#include <iostream>
#include<cstdlib>

#include "untils.h"
#include "socket.h"
#include "graphicscolorsvgitem.h"

namespace Ui {
class ToDoListItem;
}

class ToDoListItem : public QWidget
{
    Q_OBJECT

public:
    char opTime[32]{};
    char createTime[32]{};
    char op;
    char isCheck;
    char data[128];
    explicit ToDoListItem(QWidget *parent = nullptr, const QString &displayData = "", char *_opTime = nullptr, char *_createTime = nullptr, char _check = '\0', char *_data = nullptr);
    void refreshColor();
    ~ToDoListItem();

private slots:
    void on_choose_clicked();

    void on_dataLine_editingFinished();

private:
    Ui::ToDoListItem *ui;
    QString oldData;
    bool isChoose;
};

#endif // TODOLISTITEM_H
