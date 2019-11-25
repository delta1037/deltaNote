#include "todolistitem.h"
#include "ui_todolistitem.h"
#include "log.h"
#include <iostream>
#include <QMessageBox>

ToDoListItem::ToDoListItem(QWidget *parent, const QString &data) :
    QWidget(parent),
    ui(new Ui::ToDoListItem)
{
    ui->setupUi(this);

    ui->data->setText(data);
    isChoose = false;
}

ToDoListItem::~ToDoListItem()
{
    delete ui;
}

void ToDoListItem::on_choose_clicked()
{
    if(isChoose){
        ui->choose->setIcon(QIcon(QPixmap(":/resource/unChoose.svg")));
    }else{
        ui->choose->setIcon(QIcon(QPixmap(":/resource/choose.svg")));
    }
    isChoose = !isChoose;
}

void ToDoListItem::on_data_editingFinished()
{
    QMessageBox::warning(this, tr("Warning"),
                         tr("loss focuse!"),
                         QMessageBox::Yes);
}
