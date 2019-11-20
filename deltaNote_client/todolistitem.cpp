#include "todolistitem.h"
#include "ui_todolistitem.h"

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

    MSG synPack{};
    makeSocketPack(synPack, 1, MSG_FULL, Push);
    makeDataPack(synPack.msgQueue[0], createTime, opTime, data, CHECK, isCheck);

    SocketClient socketClient = SocketClient(g_server, g_port);
    socketClient.sendMsg(&synPack, sizeof(synPack));

    MSG recvPack{};
    socketClient.recvMsg(&recvPack, sizeof (recvPack));

    if(PushError == recvPack.msgState){
        QMessageBox::warning(this, tr("Warning"), tr("check the todo error!"), QMessageBox::Yes);
    }
}

void ToDoListItem::on_data_editingFinished()
{
    // edit success start process
    QString newData = ui->data->text().trimmed();
    if(oldData == nullptr && newData != nullptr){
        // add
        strcpy(data, newData.toLatin1().data());
        oldData = newData;
        sprintf(createTime, "%ld", std::time(nullptr));
        sprintf(opTime, "%ld", std::time(nullptr));

        MSG synPack{};
        makeSocketPack(synPack, 1, MSG_FULL, Push);
        makeDataPack(synPack.msgQueue[0], createTime, opTime, data, ADD, isCheck);

        SocketClient socketClient = SocketClient(g_server, g_port);
        socketClient.sendMsg(&synPack, sizeof(synPack));

        MSG recvPack{};
        socketClient.recvMsg(&recvPack, sizeof (recvPack));

        if(PushError == recvPack.msgState){
            QMessageBox::warning(this, tr("Warning"), tr("add new todo error!"), QMessageBox::Yes);
        }
    } else if(oldData.compare(newData) != 0){
        // alter
        memset(opTime, 0, sizeof(opTime));
        sprintf(opTime, "%ld", std::time(nullptr));
        strcpy(data, newData.toLatin1().data());
        oldData = newData;

        MSG synPack{};
        makeSocketPack(synPack, 1, MSG_FULL, Push);
        makeDataPack(synPack.msgQueue[0], createTime, opTime, data, ALTER, isCheck);

        SocketClient socketClient = SocketClient(g_server, g_port);
        socketClient.sendMsg(&synPack, sizeof(synPack));

        MSG recvPack{};
        socketClient.recvMsg(&recvPack, sizeof (recvPack));

        if(PushError == recvPack.msgState){
            QMessageBox::warning(this, tr("Warning"), tr("alter new todo error!"), QMessageBox::Yes);
        }
    }
}
