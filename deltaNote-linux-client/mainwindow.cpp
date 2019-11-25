#include "mainwindow.h"
#include "ui_mainwindow.h"

extern char g_username[G_ARR_SIZE_USERNAME];
extern char g_passwd[G_ARR_SIZE_PASSWD];

extern char g_server[G_ARR_SIZE_SERVER];
extern int g_port;

extern bool isLogin;
extern bool isLocked;

extern QColor fontColor;
extern QColor iconColor;
extern int transparentPos;
extern bool cleanFlag;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
    ui->ToDoListWin->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->ToDoListWin->setStyleSheet("background-color:transparent");
    ui->ToDoListWin->setFrameShape(QListWidget::NoFrame);

    ui->ToDoListWin->clear();
    QWidget *addTag = new ToDoListItem(this);
    QListWidgetItem *addNew = new QListWidgetItem();
    addNew->setSizeHint(QSize(100, 50));

    ui->ToDoListWin->addItem(addNew);
    ui->ToDoListWin->setItemWidget(addNew, addTag);

    refreshBackground();

    //设置无边框和设置隐藏下部图标
    setWindowFlags(Qt::Tool);
    QIcon icon = QIcon(":/deltaNote.ico");
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->setParent(this);
    trayIcon->setToolTip("deltaNote");
    trayIcon->setVisible(true);
    trayIcon->show();

    officialAction = new QAction("官网", this);
    connect(officialAction, SIGNAL(triggered()), this, SLOT(on_openOfficialSite_triggered()));

    settingAction = new QAction("设置", this);
    connect(settingAction, SIGNAL(triggered()), this, SLOT(on_setting_clicked()));

    quitAction = new QAction("退出程序", this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit())); //关闭应用，qApp对应的是程序全局唯一指针

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(officialAction);
    trayIconMenu->addAction(settingAction);
    trayIconMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayIconMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_openOfficialSite_triggered(){
    QDesktopServices :: openUrl(QUrl(QLatin1String("http://www.delta1037.cn/2019/11/23/deltaNoteSite/")));
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    event->ignore();
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(255, 255, 255, 255 - transparentPos));
}

void MainWindow::refreshBackground(){
    // refresh block
    ui->add->setStyleSheet("background-color:transparent");
    ui->del->setStyleSheet("background-color:transparent");
    ui->insert->setStyleSheet("background-color:transparent");
    ui->clear->setStyleSheet("background-color:transparent");

    ui->setting->setStyleSheet("background-color:transparent");
    ui->refresh->setStyleSheet("background-color:transparent");
    ui->lock->setStyleSheet("background-color:transparent");

    ui->ToDoListWin->setStyleSheet("background-color:transparent");

    // change text color

    for(int index = ui->ToDoListWin->count() - 1; index >= 0; --index){
        QListWidgetItem *item = ui->ToDoListWin->item(index);
        ToDoListItem *todo = qobject_cast<ToDoListItem*>(ui->ToDoListWin->itemWidget(item));
        todo->refreshColor();
    }

    // change icon color
    GraphicsColorSvgItem svg_refresh(":/resource/refresh.svg");
    ui->refresh->setIcon(svg_refresh.setColor(iconColor));

    GraphicsColorSvgItem svg_setting(":/resource/setting.svg");
    ui->setting->setIcon(svg_setting.setColor(iconColor));

    if(isLocked){
        GraphicsColorSvgItem svg_lock(":/resource/locked.svg");
        ui->lock->setIcon(svg_lock.setColor(iconColor));
    }else{
        GraphicsColorSvgItem svg_lock(":/resource/lock.svg");
        ui->lock->setIcon(svg_lock.setColor(iconColor));
    }


    GraphicsColorSvgItem svg_add(":/resource/add.svg");
    ui->add->setIcon(svg_add.setColor(iconColor));

    GraphicsColorSvgItem svg_del(":/resource/delete.svg");
    ui->del->setIcon(svg_del.setColor(iconColor));

    GraphicsColorSvgItem svg_insert(":/resource/insert.svg");
    ui->insert->setIcon(svg_insert.setColor(iconColor));

    GraphicsColorSvgItem svg_clear(":/resource/clear.svg");
    ui->clear->setIcon(svg_clear.setColor(iconColor));
}
void MainWindow::on_setting_clicked()
{
    cleanFlag = false;
    if(isLogin){
        userInfo userinfo(this);
        userinfo.exec();
    }else {
        login loginWindow(this);
        loginWindow.exec();
    }

    // refreshBackground
    refreshBackground();

    // clean data
    if(cleanFlag){
        // clean client todo list
        ui->ToDoListWin->clear();
        if(isLogin){
            MSG_PACK synPack{};
            makeSocketPack(synPack, 1, MSG_FULL, Delete);

            if(CleanSuccess == synMsgToServer(synPack)){
                cleanFlag = false;
            }else{
                QMessageBox::warning(this, tr("Error"), tr("服务端清除数据错误!"), QMessageBox::Yes);
                LOG_ERROR("clean data error")
            }
        }
    }
}

void MainWindow::on_refresh_clicked()
{
    // refresh list
    if(!isLogin){
        return;
    }

    MSG_PACK synPack{};
    makeSocketPack(synPack, 1, MSG_FULL, Pull);

    SocketClient socketClient = SocketClient(g_server, g_port);
    socketClient.sendMsg(&synPack, sizeof(synPack));

    MSG_PACK recv{};
    socketClient.recvMsg(&recv, sizeof (recv));
    vector<MSG_OP_PACK> datasetRecv;

    do {
        for (int i = 0; i < recv.msgSize; ++i) {
            MSG_OP_PACK opPack{};
            makeDataPack(opPack, recv.msgQueue[i].opTimestamp, recv.msgQueue[i].createTimestamp, recv.msgQueue[i].op, recv.msgQueue[i].isCheck, recv.msgQueue[i].data);
            datasetRecv.push_back(opPack);
        }

        if (recv.msg_seg == MSG_HALF) {
            socketClient.recvMsg(&recv, sizeof(recv));
        } else {
            break;
        }
    } while (true);

    ClientSqlite sqlite;
    sqlite.cleanDatasetTable();
    ui->ToDoListWin->clear();
    for(int i = 0; i < datasetRecv.size(); ++i){
        sqlite.insertDatasetItem(datasetRecv[i]);

        QString data = QString(QLatin1String(datasetRecv[i].data));
        QWidget *addTag = new ToDoListItem(this, "", datasetRecv[i].opTimestamp, datasetRecv[i].createTimestamp, datasetRecv[i].isCheck, datasetRecv[i].data);
        QListWidgetItem *addNew = new QListWidgetItem();
        addNew->setSizeHint(QSize(100, 50));

        ui->ToDoListWin->addItem(addNew);
        ui->ToDoListWin->setItemWidget(addNew, addTag);
    }
}

void MainWindow::on_lock_clicked()
{
    // lock location
    if (isLocked){
        setWindowFlags(Qt::Window | Qt::Tool);
        showNormal();
        GraphicsColorSvgItem svg_lock(":/resource/lock.svg");
        ui->lock->setIcon(svg_lock.setColor(iconColor));
    } else {
        setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool);
        showNormal();
        GraphicsColorSvgItem svg_lock(":/resource/locked.svg");
        ui->lock->setIcon(svg_lock.setColor(iconColor));
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
    if(ui->ToDoListWin->currentRow() < 0 || ui->ToDoListWin->currentRow() > ui->ToDoListWin->count()){
        return;
    }
    QListWidgetItem *item = ui->ToDoListWin->item(ui->ToDoListWin->currentRow());
    ToDoListItem *todo = qobject_cast<ToDoListItem*>(ui->ToDoListWin->itemWidget(item));

    if(isLogin) {
        MSG_PACK synPack{};
        makeSocketPack(synPack, 1, MSG_FULL, Push);

        sprintf(todo->opTime, "%ld", std::time(nullptr));
        makeDataPack(synPack.msgQueue[0], todo->opTime, todo->createTime, DEL, todo->isCheck, todo->data);

        if(PushError == synMsgToServer(synPack)){
            QMessageBox::warning(this, tr("Warning"), tr("服务端删除数据错误!"), QMessageBox::Yes);
        }
    } else {
        MSG_OP_PACK pack{};
        ClientSqlite clientSqlite;
        QListWidgetItem *item = ui->ToDoListWin->item(ui->ToDoListWin->currentRow());
        ToDoListItem *todo = qobject_cast<ToDoListItem*>(ui->ToDoListWin->itemWidget(item));
        makeDataPack(pack, todo->opTime, todo->createTime, DEL, todo->isCheck, todo->data);
        clientSqlite.insertChangeItem(pack);
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
    QWidget *addTag = new ToDoListItem(this);
    QListWidgetItem *insertNew = new QListWidgetItem();
    insertNew->setSizeHint(QSize(100, 50));
    //insertNew->setFlags(insertNew->flags() & ~Qt::ItemIsSelectable);

    ui->ToDoListWin->insertItem(ui->ToDoListWin->currentRow(), insertNew);
    ui->ToDoListWin->setItemWidget(insertNew, addTag);
}

void MainWindow::on_actClear_triggered()
{
    if(isLogin) {
        if(ui->ToDoListWin->count() > 0){
            SocketClient socketClient = SocketClient(g_server, g_port);
            if(SocketError == socketClient.getSocketOpState()){
                QMessageBox::warning(this, tr("Error"), tr("Socket connect error!"), QMessageBox::Yes);
            }else{
                MSG_PACK synPack{};
                for(int index = ui->ToDoListWin->count() - 1; index >= 0;){
                    int left = min(5, index + 1);
                    makeSocketPack(synPack, left, ((left == 5 && (index != 4))? MSG_HALF:MSG_FULL), Push);

                    for(int i = 0; i < 5 && index >= 0; ++i, --index){
                        QListWidgetItem *item = ui->ToDoListWin->item(index);
                        ToDoListItem *todo = qobject_cast<ToDoListItem*>(ui->ToDoListWin->itemWidget(item));
                        makeDataPack(synPack.msgQueue[i], todo->opTime, todo->createTime, DEL, todo->isCheck, todo->data);
                    }
                    socketClient.sendMsg(&synPack, sizeof(synPack));
                }

                MSG_PACK recvPack{};
                socketClient.recvMsg(&recvPack, sizeof (recvPack));

                if(PushError == recvPack.msgState){
                    QMessageBox::warning(this, tr("Warning"), tr("服务端删除数据错误!"), QMessageBox::Yes);
                } else {
                    ui->ToDoListWin->clear();
                }
            }
        }
    } else {
        MSG_OP_PACK pack{};
        ClientSqlite clientSqlite;
        for(int index = ui->ToDoListWin->count() - 1; index >= 0; --index){
            QListWidgetItem *item = ui->ToDoListWin->item(index);
            ToDoListItem *todo = qobject_cast<ToDoListItem*>(ui->ToDoListWin->itemWidget(item));
            if(todo->opTime[0] == '\0' && todo->createTime[0] == '\0' && todo->data[0] == '\0'){
                makeDataPack(pack, todo->opTime, todo->createTime, DEL, todo->isCheck, todo->data);
                clientSqlite.insertChangeItem(pack);
            }
        }
        ui->ToDoListWin->clear();
    }
}
