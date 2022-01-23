#include "todolistitem.h"
#include "ui_todolistitem.h"

ToDoListItem::ToDoListItem(QWidget *parent, SettingCtrl *setting_ctrl) :
    QWidget(parent),
    ui(new Ui::ToDoListItem),
    setting_ctrl(setting_ctrl)
{
    ui->setupUi(this);
    item_status = Item_new;

    ui->data->setFont(QFont("黑体", setting_ctrl->get_int(SETTING_FONT_SIZE)));
    ui->data->setPlaceholderText("TODO");

    update_render();
}

ToDoListItem::~ToDoListItem()
{
    delete ui;
}

void ToDoListItem::set_item_data(const TodoItem &todo_item, ItemStatus status){
    this->item_data = todo_item;
    // 设置是否是新建的
    item_status = status;
    // 刷新该条目
    update_text();
    update_render();
}

TodoItem ToDoListItem::get_item_data(){
    return this->item_data;
}

ItemStatus ToDoListItem::get_item_status(){
    return item_status;
}

void ToDoListItem::update_text(){
    ui->data->setFont(QFont("黑体", setting_ctrl->get_int(SETTING_FONT_SIZE)));
    ui->data->setText(QString::fromStdString(item_data.data));
}

void ToDoListItem::update_render(){
    // 这里可以统一由外部生成 TODO 优化的点
    QPalette palette;
    palette.setColor(QPalette::Text, setting_ctrl->get_color(SETTING_FONT_COLOR));
    palette.setColor(QPalette::PlaceholderText, setting_ctrl->get_color(SETTING_FONT_COLOR));
    ui->data->setPalette(palette);

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
    if(item_status == Item_new && ui->data->text().toStdString().empty()){
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

// 修改内容
void ToDoListItem::on_data_editingFinished(){
    // 编辑结束，通知准备处理
    if(item_status == Item_new){
        // 如果是新的，编辑结束时还没有数据，相当于无操作
        if(ui->data->text().toStdString().empty()){
            return;
        }
        item_status = Item_old;
        // 在发信号之前更新数据
        item_data.op_type = OpType_add;
        item_data.data = ui->data->text().toStdString();
        emit todo_alt_signal(item_data.create_key, Alt_add);
    }else{
        // 在发信号之前更新数据
        item_data.op_type = OpType_alt;
        item_data.data = ui->data->text().toStdString();
        emit todo_alt_signal(item_data.create_key, Alt_alt);
    }
}

