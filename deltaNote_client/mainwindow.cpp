#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ToDoListWin->setContextMenuPolicy(Qt::CustomContextMenu);
    //ui->add->setDefaultAction();

    on_actClear_triggered();
    QWidget *addTag = new ToDoListItem(this);
    QListWidgetItem *addNew = new QListWidgetItem();
    addNew->setSizeHint(QSize(100, 50));

    ui->ToDoListWin->addItem(addNew);
    ui->ToDoListWin->setItemWidget(addNew, addTag);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_setting_clicked()
{
    if(isLogin){
        QDialog *userinfo = new userInfo(this);
        userinfo->exec();
    }else {
        QDialog *loginWindow = new login(this);
        loginWindow->exec();
    }
}

void MainWindow::on_refresh_clicked()
{
    // refresh list
    //SocketClient socketClient = SocketClient(g_server, g_port);
    //MSG synPack{};
}

void MainWindow::on_lock_clicked()
{
    // lock location
    if (isLocked){
        setWindowFlags(Qt::Window);
        showNormal();
        ui->lock->setIcon(QIcon(QPixmap(":/resource/lock.svg")));
    } else {
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        showNormal();
        ui->lock->setIcon(QIcon(QPixmap(":/resource/locked.svg")));
    }
    isLocked = !isLocked;
}

void MainWindow::on_ToDoListWin_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)
    QMenu* menu = new QMenu(this);                                         //创建菜单
    menu->addAction(ui->actAdd);
    menu->addAction(ui->actDel);
    menu->addAction(ui->actInsert);
    menu->addAction(ui->actClear);
    menu->exec(QCursor::pos());                                            //在鼠标光标位置显示右键快捷菜单
    delete menu;
}

void MainWindow::on_actAdd_triggered()
{
    QWidget *addTag = new ToDoListItem(this);
    QListWidgetItem *addNew = new QListWidgetItem();
    addNew->setSizeHint(QSize(100, 50));

    ui->ToDoListWin->addItem(addNew);
    ui->ToDoListWin->setItemWidget(addNew, addTag);
}

void MainWindow::on_actDel_triggered()
{
    //int currentRow();
    //QListWidgetItem* takeItem(int);

    QListWidgetItem *item = ui->ToDoListWin->takeItem(ui->ToDoListWin->currentRow());
    ToDoListItem *todo = (ToDoListItem *)(ui->ToDoListWin->itemWidget(item));

    MSG synPack{};
    makeSocketPack(synPack, 1, MSG_FULL, Push);

    sprintf(todo->opTime, "%ld", std::time(nullptr));
    makeDataPack(synPack.msgQueue[0], todo->createTime, todo->opTime, todo->data, DEL, todo->isCheck);

    SocketClient socketClient = SocketClient(g_server, g_port);
    socketClient.sendMsg(&synPack, sizeof(synPack));

    MSG recvPack{};
    socketClient.recvMsg(&recvPack, sizeof (recvPack));

    if(PushError == recvPack.msgState){
        QMessageBox::warning(this, tr("Warning"), tr("delete the todo error!"), QMessageBox::Yes);
    }

    if(nullptr != item){
        delete item;
    }

    if(nullptr != todo){
        delete todo;
    }
}

void MainWindow::on_actInsert_triggered()
{
//void insertItem(int, const QString&);
    QWidget *addTag = new ToDoListItem(this);
    QListWidgetItem *insertNew = new QListWidgetItem();
    insertNew->setSizeHint(QSize(100, 50));

    ui->ToDoListWin->insertItem(ui->ToDoListWin->currentRow(), insertNew);
    ui->ToDoListWin->setItemWidget(insertNew, addTag);
}

void MainWindow::on_actClear_triggered()
{
    //ui->ToDoListWin->clear();
    SocketClient socketClient = SocketClient(g_server, g_port);

    MSG synPack{};
    for(int index = ui->ToDoListWin->count() - 1; index >= 0; --index){
        int left = min(5, index + 1);

        for(int i = 0; i < 5 && index >= 0; ++i, --index){
            QListWidgetItem *item = ui->ToDoListWin->takeItem(index);
            ToDoListItem *todo = (ToDoListItem *)(ui->ToDoListWin->itemWidget(item));

            makeSocketPack(synPack, left, (left == 5? MSG_FULL:MSG_HALF), Push);
            makeDataPack(synPack.msgQueue[i], todo->createTime, todo->opTime, todo->data, DEL, todo->isCheck);

            if(nullptr != item){
                delete item;
            }

            if(nullptr != todo){
                delete todo;
            }
        }
        socketClient.sendMsg(&synPack, sizeof(synPack));
    }

    MSG recvPack{};
    socketClient.recvMsg(&recvPack, sizeof (recvPack));

    if(PushError == recvPack.msgState){
        QMessageBox::warning(this, tr("Warning"), tr("delete the todo error!"), QMessageBox::Yes);
    }
}
