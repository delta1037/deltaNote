#ifndef TODOLISTITEM_H
#define TODOLISTITEM_H

#include <QWidget>
#include <QPainter>
#include <QTextCodec>
#include <QMessageBox>

#include <iostream>
#include<cstdlib>

#include "svg_color.h"

#include "inter_var.h"
#include "setting_ctrl.h"

namespace Ui {
class ToDoListItem;
}


enum AltType{
    Alt_nul = 0,
    Alt_add = 1,
    Alt_alt = 2,
    Alt_chk = 3,
};

enum ItemStatus{
    Item_old = 0,
    Item_new = 1,
};


class ToDoListItem : public QWidget {
    Q_OBJECT
public:
    explicit ToDoListItem(QWidget *parent = nullptr, SettingCtrl *setting_ctrl = nullptr);
    ~ToDoListItem();

    void set_item_data(const TodoItem &data, ItemStatus status=Item_old);
    TodoItem get_item_data();
    ItemStatus get_item_status();

private:
    void update_text();
    void update_render();
private slots:
    void on_choose_clicked();           // 修改check的值
    void on_data_editingFinished(); // 编辑内容

signals:
    void todo_alt_signal(const std::string&, AltType);

private:
    Ui::ToDoListItem *ui;
    // 数据
    TodoItem item_data;
    // 配置
    SettingCtrl *setting_ctrl;

    // 记录条目状态
    ItemStatus item_status;
};

#endif // TODOLISTITEM_H
