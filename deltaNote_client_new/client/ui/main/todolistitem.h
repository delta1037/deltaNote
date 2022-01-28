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
#include "todo_detail.h"

namespace Ui {
class ToDoListItem;
}

class ToDoListItem : public QWidget {
    Q_OBJECT
public:
    explicit ToDoListItem(QWidget *parent = nullptr, SettingCtrl *setting_ctrl = nullptr);
    ~ToDoListItem();

    void set_item_data(const TodoItem &data, ItemStatus status=Item_old);
    TodoItem get_item_data();
    ItemStatus get_item_status();

private:
    void update_text(ItemStatus status);
    void update_render();
private slots:
    void on_choose_clicked();        // 修改check的值
    void on_data_button_clicked();   // 显示详细内容对话框

signals:
    void todo_alt_signal(const std::string&, AltType);
    void todo_edit_status(bool);

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
