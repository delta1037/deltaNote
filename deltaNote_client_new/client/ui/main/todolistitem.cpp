#include "todolistitem.h"
#include "ui_todolistitem.h"

ToDoListItem::ToDoListItem(QWidget *parent, SettingCtrl *setting_ctrl) :
    QWidget(parent),
    ui(new Ui::ToDoListItem),
    setting_ctrl(setting_ctrl){
    ui->setupUi(this);
    item_status = Item_new;

    update_text(Item_new);
    update_render();
}

ToDoListItem::~ToDoListItem(){
    delete ui;
}

void ToDoListItem::set_item_data(const TodoItem &todo_item, ItemStatus status){
    this->item_data = todo_item;
    // 设置是否是新建的
    item_status = status;
    // 刷新该条目
    update_text(item_status);
    update_render();
}

TodoItem ToDoListItem::get_item_data(){
    return this->item_data;
}

ItemStatus ToDoListItem::get_item_status(){
    return item_status;
}

void ToDoListItem::update_text(ItemStatus status){
    if(status == Item_old){
        ui->data_button->setText(QString::fromStdString(item_data.data));
    }else{
        ui->data_button->setText("TODO");
    }
    ui->data_button->setFont(QFont("黑体", setting_ctrl->get_int(SETTING_FONT_SIZE)));
}

void ToDoListItem::update_render(){
    // 这里可以统一由外部生成 TODO 优化的点
    QPalette palette;
    palette.setColor(QPalette::ButtonText, setting_ctrl->get_color(SETTING_FONT_COLOR));
    ui->data_button->setPalette(palette);
    if(item_data.is_check == Check_true){
        SvgColor svg_check(":/resource/choose.svg");
        ui->choose->setIcon(svg_check.setColor(setting_ctrl->get_color(SETTING_ICON_COLOR)));
    }else{
        SvgColor svg_check(":/resource/unChoose.svg");
        ui->choose->setIcon(svg_check.setColor(setting_ctrl->get_color(SETTING_ICON_COLOR)));
    }
}

// 修改check的值
void ToDoListItem::on_choose_clicked(){
    if(item_status == Item_new && item_data.data.empty()){
        return;
    }
    // 更新数据
    if(item_data.is_check == Check_true){
        item_data.is_check = Check_false;
    }else{
        item_data.is_check = Check_true;
    }
    // 发送给主窗口，由主窗口对该条目进行销毁
    emit todo_alt_signal(item_data.create_key, Alt_chk);
}

void ToDoListItem::on_data_button_clicked(){
    if(item_data.is_check == Check_true){
        // 已经check的不允许修改
        return;
    }
    // 设置条目正在编辑的状态
    emit todo_edit_status(true);
    TodoDetail todo_detail(this, setting_ctrl);
    todo_detail.set_item_data(item_data, item_status);
    if(todo_detail.exec() == QDialog::Rejected){
        emit todo_edit_status(false);
        return;
    }

    // 有内容更新，获取内容
    item_data = todo_detail.get_item_data();
    if(!item_data.data.empty()){
        update_text(Item_old); // 更新显示数据
    }

    // 编辑结束，通知准备处理
    if(item_status == Item_new){
        // 如果是新的，编辑结束时还没有数据，相当于无操作
        if(item_data.data.empty()){
            emit todo_edit_status(false);
            return;
        }
        item_status = Item_old;
        // 在发信号之前更新数据
        item_data.op_type = OpType_add;
        emit todo_alt_signal(item_data.create_key, Alt_add);
    }else{
        // 在发信号之前更新数据
        item_data.op_type = OpType_alt;
        emit todo_alt_signal(item_data.create_key, Alt_alt);
    }
    emit todo_edit_status(false);
}

