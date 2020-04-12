#include "ui/todolistitem.h"
#include "ui_todolistitem.h"

ToDoListItem::ToDoListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToDoListItem)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setColor(QPalette::Text, fontColor);
    ui->dataLine->setPalette(palette);
    ui->dataLine->setFont(QFont("黑体",fontSize));
    ui->dataLine->setPlaceholderText("TODO");
    //ui->dataLine->setStyleSheet("QToolTip{background-color:transparent}");

    GraphicsColorSvgItem svg_check(":/resource/unChoose.svg");
    ui->choose->setIcon(svg_check.setColor(iconColor));
}

MsgOpPack ToDoListItem::getItemData(){
    return itemData;
}
void ToDoListItem::setItemData(MsgOpPack &newData){
    itemData = newData;
    refreshItem();
}
QString ToDoListItem::getTextData(){
    return itemData.data;
}

void ToDoListItem::refreshItem(){
    QPalette palette;
    palette.setColor(QPalette::Text, fontColor);
    ui->dataLine->setPalette(palette);
    ui->dataLine->setFont(QFont("黑体",fontSize));
    ui->dataLine->setText(itemData.data);
    //std::time_t op_time = itemData.createTime;
    //ui->dataLine->setToolTip(std::asctime(std::localtime(&op_time)));

    if(itemData.isCheck == Checked){
        GraphicsColorSvgItem svg_check(":/resource/choose.svg");
        ui->choose->setIcon(svg_check.setColor(iconColor));
    }else{
        GraphicsColorSvgItem svg_check(":/resource/unChoose.svg");
        ui->choose->setIcon(svg_check.setColor(iconColor));
    }
}

ToDoListItem::~ToDoListItem()
{
    delete ui;
}

// 修改check的值
void ToDoListItem::on_choose_clicked()
{
    if(itemData.data.size() == 0){
        return;
    }

    if(itemData.isCheck == Checked){
        // 将此item从done队列转移到undo队列
        ListControl::doneMoveToUndo(itemData);
    }else{
        // 将此item从undo队列转移到done队列
        ListControl::undoMoveToDone(itemData);
    }

    // 发送给主窗口，由主窗口对该条目进行销毁
    emit deleteTodoSignal();
}

// 调用修改，同步本地内容
void ToDoListItem::on_dataLine_editingFinished()
{
    if(0 == itemData.data.size()){
        // add
        itemData.data = ui->dataLine->text();
        ListControl::undoListAddNew(itemData);
    }else{
        // modify
        itemData.data = ui->dataLine->text();
        ListControl::todoListModify(itemData);
    }
}

