#ifndef TODOLISTITEM_H
#define TODOLISTITEM_H

#include <QWidget>
#include <QPainter>
#include <QTextCodec>
#include <QMessageBox>

#include <iostream>
#include<cstdlib>

#include "todolistcontrol.h"
#include "graphicscolorsvgitem.h"

namespace Ui {
class ToDoListItem;
}

class ToDoListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ToDoListItem(QWidget *parent = nullptr);
    ~ToDoListItem();

    void refreshItem();
    void setItemData(MsgOpPack &newData);
    MsgOpPack getItemData();
    QString getTextData();

private slots:
    void on_choose_clicked();           // 修改check的值
    void on_dataLine_editingFinished(); // 编辑内容

signals:
    void deleteTodoSignal();

private:
    Ui::ToDoListItem *ui;
    MsgOpPack itemData;
};

#endif // TODOLISTITEM_H
