#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include "userinfo.h"
#include "log.h"
#include "pack.h"

extern char g_server[16];
extern int g_port;

extern char g_username[8];
extern char g_passdw[12];

extern bool isLogin;
extern bool isLocked;

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
    synPack.msgSize = 1;
    synPack.msg_seg = MSG_FULL;
    strcpy(synPack.userName, g_username);
    strcpy(synPack.passwd, g_passdw);
    synPack.msgOp = Push;

    strcpy(synPack.msgQueue[0].createTimestamp, todo->createTime);
    sprintf(synPack.msgQueue[0].opTimestamp, "%ld", std::time(0));
    strcpy(synPack.msgQueue[0].data, todo->data);
    synPack.msgQueue[0].op = DEL;
    synPack.msgQueue->isCheck = todo->isCheck;

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
    ui->ToDoListWin->clear();
}
