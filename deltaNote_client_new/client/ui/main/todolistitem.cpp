#include "todolistitem.h"
#include "ui_todolistitem.h"
#include "log.h"

#include <QAction>
#include <QMenu>

ToDoListItem::ToDoListItem(QWidget *parent, SettingCtrl *setting_ctrl) :
    QWidget(parent),
    ui(new Ui::ToDoListItem),
    setting_ctrl(setting_ctrl){
    ui->setupUi(this);
    item_status = Item_new;
    custom_context_show = false;

    // 连接右键事件
    ui->data->setContextMenuPolicy(Qt::CustomContextMenu);
    action_detail = new QAction(ITEM_DETAIL_INFO, this);
    connect(action_detail, SIGNAL(triggered()), this, SLOT(data_detail_clicked())); // 编辑详细内容

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
        ui->data->setText(QString::fromStdString(item_data.data));
    }else{
        ui->data->setPlaceholderText("TODO");
    }
    ui->data->setFont(QFont("黑体", setting_ctrl->get_int(SETTING_FONT_SIZE)));
}

void ToDoListItem::update_render(){
    // 这里可以统一由外部生成
    QPalette data_palette;
    data_palette.setColor(QPalette::Text, setting_ctrl->get_color(SETTING_FONT_COLOR));
    data_palette.setColor(QPalette::PlaceholderText, setting_ctrl->get_color(SETTING_FONT_COLOR));
    ui->data->setPalette(data_palette);

    if(item_data.is_check == Check_true){
        SvgColor svg_check(":/resource/choose.svg");
        ui->choose->setIcon(svg_check.setColor(setting_ctrl->get_color(SETTING_ICON_COLOR)));
    }else{
        SvgColor svg_check(":/resource/unChoose.svg");
        ui->choose->setIcon(svg_check.setColor(setting_ctrl->get_color(SETTING_ICON_COLOR)));
    }

    // 字体大小
    ui->data->setFont(QFont("黑体", setting_ctrl->get_int(SETTING_FONT_SIZE)));
}

// 修改check的值
void ToDoListItem::on_choose_clicked(){
    emit todo_edit_status(true);
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

// 修改内容
void ToDoListItem::on_data_editingFinished(){
    // 该信号会与右键信号冲突，过滤冲突清空
    if(item_data.data == ui->data->text().toStdString()){
        d_ui_debug("%s", "1_custom_context_show !!!")
        return;
    }
    if(custom_context_show){
        // 如果数据没编辑完就右键了，
        d_ui_debug("%s", "2_custom_context_show !!!")
        // 数据同步一下
        item_data.data = ui->data->text().toStdString();
        d_ui_debug("### data is editing data is %s", item_data.data.c_str())
        return;
    }
    // 发送更新信号，禁止刷新
    emit todo_edit_status(true);
    d_ui_debug("%s", "edit is finish, ready process!!!")
    // 编辑结束，通知准备处理
    if(item_status == Item_new){
        // 如果是新的，编辑结束时还没有数据，相当于无操作
        if(ui->data->text().toStdString().empty()){
            emit todo_edit_status(false);
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

void ToDoListItem::data_detail_clicked(){
    if(item_data.is_check == Check_true){
        // 已经check的不允许修改
        return;
    }

    // 设置条目正在编辑的状态
    emit todo_edit_status(true);
    TodoDetail todo_detail(this, setting_ctrl);
    todo_detail.set_item_data(item_data);
    if(todo_detail.exec() == QDialog::Rejected){
        emit todo_edit_status(false);
        return;
    }

    d_ui_debug("%s", "edit finished, ready update")
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
}

void ToDoListItem::on_data_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos)
    if(item_data.is_check == Check_true){
        // 已经check的数据禁止获取详细信息
        return;
    }
    custom_context_show = true;
    // 右键信息不能被销毁掉
    emit todo_edit_status(true);
    auto* menu = new QMenu(this);                                         //创建菜单
    menu->addAction(action_detail);
    menu->setStyleSheet("background-color:#F0F8FF;selection-background-color: #FFB7DD;border-radius: 0px;");
    menu->exec(QCursor::pos());                                            //在鼠标光标位置显示右键快捷菜单
    delete menu;
    emit todo_edit_status(false);
    custom_context_show = false;
}

