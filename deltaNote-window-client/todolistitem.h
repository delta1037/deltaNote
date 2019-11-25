#ifndef TODOLISTITEM_H
#define TODOLISTITEM_H

#include <QWidget>
#include <QPainter>
#include <QTextCodec>
#include <QMessageBox>

#include <iostream>
#include<cstdlib>

#include "untils.h"
#include "graphicscolorsvgitem.h"

namespace Ui {
class ToDoListItem;
}

class ToDoListItem : public QWidget
{
    Q_OBJECT

public:
    char opTime[G_TIMESTAMP_SIZE]{};
    char createTime[G_TIMESTAMP_SIZE]{};
    char op;
    char isCheck;
    char data[G_DATA_TRANS_SIZE];
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
