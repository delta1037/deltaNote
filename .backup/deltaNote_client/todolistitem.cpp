#include "todolistitem.h"
#include "ui_todolistitem.h"
#include "log.h"
#include "pack.h"
#include <iostream>
#include <QMessageBox>
#include "socket.h"

#include<cstdlib>
using namespace deltaNote;
extern char g_server[16];
extern int g_port;

extern char g_username[8];
extern char g_passdw[12];

extern bool isLogin;
extern bool isLocked;

ToDoListItem::ToDoListItem(QWidget *parent, const QString &data) :
    QWidget(parent),
    ui(new Ui::ToDoListItem)
{
    ui->setupUi(this);
    ui->data->setText(data);
    ui->data->setPlaceholderText("TODO");
    isChoose = false;
    oldData = nullptr;

    op = 0;
    isCheck = UnCheck;
}

ToDoListItem::~ToDoListItem()
{
    delete ui;
}

void ToDoListItem::on_choose_clicked()
{
    if(isChoose){
        isCheck = UnCheck;
        ui->choose->setIcon(QIcon(QPixmap(":/resource/unChoose.svg")));
    }else{
        isCheck = Checked;
        ui->choose->setIcon(QIcon(QPixmap(":/resource/choose.svg")));
    }
    isChoose = !isChoose;

    // change check
    MSG synPack{};
    synPack.msgSize = 1;
    synPack.msg_seg = MSG_FULL;
    strcpy(synPack.userName, g_username);
    strcpy(synPack.passwd, g_passdw);
    synPack.msgOp = Push;

    strcpy(synPack.msgQueue[0].createTimestamp, createTime);
    strcpy(synPack.msgQueue[0].opTimestamp, opTime);
    strcpy(synPack.msgQueue[0].data, data);
    synPack.msgQueue[0].op = CHECK;
    synPack.msgQueue->isCheck = isCheck;
}

void ToDoListItem::on_data_editingFinished()
{
    // edit success start process
    QString newData = ui->data->text().trimmed();
    if(oldData == nullptr && newData != nullptr){
        // add
        strcpy(data, newData.toLatin1().data());
        oldData = newData;
        sprintf(createTime, "%ld", std::time(0));
        sprintf(opTime, "%ld", std::time(0));

        MSG synPack{};
        synPack.msgSize = 1;
        synPack.msg_seg = MSG_FULL;
        strcpy(synPack.userName, g_username);
        strcpy(synPack.passwd, g_passdw);
        synPack.msgOp = Push;

        strcpy(synPack.msgQueue[0].createTimestamp, createTime);
        strcpy(synPack.msgQueue[0].opTimestamp, opTime);
        strcpy(synPack.msgQueue[0].data, data);
        synPack.msgQueue[0].op = ADD;
        synPack.msgQueue->isCheck = isCheck;

        SocketClient socketClient = SocketClient(g_server, g_port);
        socketClient.sendMsg(&synPack, sizeof(synPack));

        QMessageBox::warning(this, tr("INFO"),
                             tr("add a new todo!"),
                             QMessageBox::Yes);
    } else if(oldData.compare(newData) != 0){
        // alter
        memset(opTime, 0, sizeof(opTime));
        sprintf(opTime, "%ld", std::time(0));
        strcpy(data, newData.toLatin1().data());
        oldData = newData;

        MSG synPack{};
        synPack.msgSize = 1;
        synPack.msg_seg = MSG_FULL;
        strcpy(synPack.userName, g_username);
        strcpy(synPack.passwd, g_passdw);
        synPack.msgOp = Push;

        strcpy(synPack.msgQueue[0].createTimestamp, createTime);
        strcpy(synPack.msgQueue[0].opTimestamp, opTime);
        strcpy(synPack.msgQueue[0].data, data);
        synPack.msgQueue[0].op = ALTER;
        synPack.msgQueue->isCheck = isCheck;

        QMessageBox::warning(this, tr("INFO"),
                             tr("alter todo!"),
                             QMessageBox::Yes);
    }
}
