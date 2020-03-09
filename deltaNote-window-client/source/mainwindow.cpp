#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWindowWidget = this; // 记录主窗口

    setAttribute(Qt::WA_TranslucentBackground);
    ui->ToDoListWin->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->ToDoListWin->setStyleSheet("background-color:transparent");
    ui->ToDoListWin->setFrameShape(QListWidget::NoFrame);

    QModelIndex todoIndex;
    //ui->ToDoListWin->indexWidget(todoIndex);
    ui->ToDoListWin->setIndexWidget(todoIndex, this);

    ui->ToDoListWin->clear();
    QWidget *addTag = new ToDoListItem(this);
    QListWidgetItem *addNew = new QListWidgetItem();
    addNew->setSizeHint(QSize(100, 50));

    ui->ToDoListWin->addItem(addNew);
    ui->ToDoListWin->setItemWidget(addNew, addTag);

    refreshBackground();

    //设置无边框和设置隐藏下部图标
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::SubWindow);
    setMouseTracking(true); //开启鼠标追踪
    ui->ToDoListWin->setMouseTracking(true);
    edgeMargin = 3;
    resizeDir = nodir;

    QIcon icon = QIcon(":/deltaNote.ico");
    trayIcon = new QSystemTrayIcon(icon, this);
    trayIcon->setToolTip("deltaNote");
    trayIcon->setVisible(true);
    trayIcon->show();

    officialAction = new QAction(MAIN_WIN_OFFICIAL_SITE, this);
    connect(officialAction, SIGNAL(triggered()), this, SLOT(openOfficialSite()));
    settingAction = new QAction(MAIN_WIN_SETTING, this);
    connect(settingAction, SIGNAL(triggered()), this, SLOT(on_setting_clicked()));
    quitAction = new QAction(MAIN_WIN_QUIT, this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit())); //关闭应用，qApp对应的是程序全局唯一指针

    clearAction = new QAction(MAIN_WIN_CLEAR_DONE, this);
    connect(clearAction, SIGNAL(triggered()), this, SLOT(clearDone()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(officialAction);
    trayIconMenu->addAction(settingAction);
    trayIconMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayIconMenu);

    move(xPos, yPos);
    setGeometry(xPos, yPos, frameWidth, frameHeight);

    ServerConnectControl control;
    control.loginToServer();

    on_refresh_clicked();
    // 程序刚刚启动，没有空的TODO
    haveNullTodo = false;

    connect(ui->ToDoListWin, SIGNAL(addTodoSignal()), this, SLOT(addTodoClicked()));

    isShowHistory = false;
    refreshTime = 0;
}

MainWindow::~MainWindow()
{
    UserDataControl control;
    control.setAllSetting();
    // save data
    std::vector<SocketMsgOpPack> opListBuffer;
    for(auto it : TodoListControl::m_opList){
        opListBuffer.push_back(SocketMsgOpPack(it));
    }
    LOG_INFO("saveLocalOpList:%d", opListBuffer.size())
    control.cleanLocalOpList();
    control.saveLocalOpList(opListBuffer);

    std::vector<SocketMsgOpPack> listBuffer;
    for(auto it : TodoListControl::m_doneList){
        listBuffer.push_back(SocketMsgOpPack(it));
    }
    for(auto it : TodoListControl::m_undoList){
        listBuffer.push_back(SocketMsgOpPack(it));
    }
    LOG_INFO("saveLocalList:%d", listBuffer.size())
    control.cleanLocalList();
    control.saveLocalList(listBuffer);
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e){
    e->ignore();
    UserDataControl control;
    control.setAllSetting();
    // save data
    std::vector<SocketMsgOpPack> opListBuffer;
    for(auto it : TodoListControl::m_opList){
        opListBuffer.push_back(SocketMsgOpPack(it));
    }
    LOG_INFO("saveLocalOpList:%d", opListBuffer.size())
    control.cleanLocalOpList();
    control.saveLocalOpList(opListBuffer);

    std::vector<SocketMsgOpPack> listBuffer;
    for(auto it : TodoListControl::m_doneList){
        listBuffer.push_back(SocketMsgOpPack(it));
    }
    for(auto it : TodoListControl::m_undoList){
        listBuffer.push_back(SocketMsgOpPack(it));
    }
    LOG_INFO("saveLocalList:%d", listBuffer.size())
    control.cleanLocalList();
    control.saveLocalList(listBuffer);
}

void MainWindow::openOfficialSite(){
    QDesktopServices :: openUrl(QUrl(QLatin1String("http://www.delta1037.cn/2019/11/23/deltaNoteSite/")));
}

void MainWindow::deleteTodoClicked(){
    // 刷新窗口，删除已经删掉的item
    MsgOpPack last;
    if(isShowHistory){
        last = TodoListControl::getLastOfUndo();
    }else{
        last = TodoListControl::getLastOfDone();
    }

    for(int i = 0; i < ui->ToDoListWin->count(); ++i){
        QListWidgetItem *item = ui->ToDoListWin->item(i);
        ToDoListItem *todo = qobject_cast<ToDoListItem*>(ui->ToDoListWin->itemWidget(item));
        if(todo->getItemData().createTime == last.createTime){
            if(nullptr != item){
                delete item;
            }
            if(nullptr != todo){
                delete todo;
            }
            break;
        }
    }
}
void MainWindow::addNewTodo(){
    // add new todo
    QWidget *addTag = new ToDoListItem(this);
    connect(addTag, SIGNAL(deleteTodoSignal()), this, SLOT(deleteTodoClicked()));

    QListWidgetItem *addNew = new QListWidgetItem();
    addNew->setSizeHint(QSize(100, 40));

    ui->ToDoListWin->addItem(addNew);
    ui->ToDoListWin->setItemWidget(addNew, addTag);
}

void MainWindow::addTodoClicked(){
    LOG_INFO("click signal")
    if(isShowHistory){
        return;
    }

    // add new todo
    if(0 == ui->ToDoListWin->count()){
        LOG_INFO("count 0, return ")
        addNewTodo();
        return;
    }

    QListWidgetItem *item = ui->ToDoListWin->item(ui->ToDoListWin->count() - 1);
    ToDoListItem *todo = qobject_cast<ToDoListItem*>(ui->ToDoListWin->itemWidget(item));
    if(todo->getTextData().size() == 0){
        // clear last todo
        if(nullptr != item){
            delete item;
        }

        if(nullptr != todo){
            delete todo;
        }
    } else {
        addNewTodo();
    }
}

void MainWindow::detectEdge()
{
    int diffBottom = abs(cursor().pos().y() - frameGeometry().bottom());
    QCursor tempCursor;                                    //获得当前鼠标样式，注意:只能获得当前鼠标样式然后再重新设置鼠标样式
    tempCursor = cursor();                                 //因为获得的不是鼠标指针，所以不能这样用:cursor().setXXXXX
    if(diffBottom < edgeMargin){
        resizeDir = bottom;
        tempCursor.setShape(Qt::SizeVerCursor);
    } else{
        resizeDir = nodir;
        tempCursor.setShape(Qt::ArrowCursor);
    }

    setCursor(tempCursor);                    //重新设置鼠标,主要是改样式
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    if(isLocked){
        return;
    }
    if (event->buttons() & Qt::LeftButton){
        if(resizeDir == nodir){
            if(event->globalX()-m_nMouseClick_X_Coordinate > desktopRect.width() - 20
                    || event->globalX()-m_nMouseClick_X_Coordinate <= 0
                    || event->globalY()-m_nMouseClick_Y_Coordinate > desktopRect.height() - 50
                    || event->globalY()-m_nMouseClick_Y_Coordinate <= 0){
                return;
            }
            move(event->globalX()-m_nMouseClick_X_Coordinate, event->globalY()-m_nMouseClick_Y_Coordinate);
            xPos = this->geometry().x();
            yPos = this->geometry().y();
        }else{
            int pbottom = frameGeometry().bottom();
            int ptop = frameGeometry().top();
            if(height() == minimumHeight()){
                pbottom = max(event->globalY(), ptop);
            }
            else if(height() == maximumHeight()){
                pbottom = min(event->globalY(), ptop);
            }
            else{
                pbottom = event->globalY();
            }
            frameHeight = this->geometry().height();
            setGeometry(QRect(QPoint(frameGeometry().left(), ptop),QPoint(frameGeometry().right(), pbottom)));
        }
    }else {
        detectEdge();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    event->ignore();

    if(isLocked){
        return;
    }

    if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
    {
        m_nMouseClick_X_Coordinate = event->x();
        m_nMouseClick_Y_Coordinate = event->y();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();

    if(isLocked){
        return;
    }

    if(resizeDir != nodir){         //还原鼠标样式
        detectEdge();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    event->ignore();
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(255, 255, 255, 255 - transparentPos));
}

void MainWindow::refreshBackground(){
    // refresh block
    ui->setting->setStyleSheet("background-color:transparent");
    ui->refresh->setStyleSheet("background-color:transparent");
    ui->lock->setStyleSheet("background-color:transparent");
    ui->history->setStyleSheet("background-color:transparent");
    ui->ToDoListWin->setStyleSheet("background-color:transparent");

    // change text color
    for(int index = ui->ToDoListWin->count() - 1; index >= 0; --index){
        QListWidgetItem *item = ui->ToDoListWin->item(index);
        ToDoListItem *todo = qobject_cast<ToDoListItem*>(ui->ToDoListWin->itemWidget(item));
        todo->refreshItem();
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

    if(isShowHistory){
        GraphicsColorSvgItem svg_clear(":/resource/history_open.svg");
        ui->history->setIcon(svg_clear.setColor(iconColor));
    }else{
        GraphicsColorSvgItem svg_clear(":/resource/history_close.svg");
        ui->history->setIcon(svg_clear.setColor(iconColor));
    }

    if (!isLocked){
        setStyleSheet(".QWidget{border:1px solid "+ iconColor.name() +"}");
    } else {
        setStyleSheet("");
    }

    setGeometry(QRect(QPoint(frameGeometry().left(), frameGeometry().top()),
                      QPoint(frameGeometry().left() + frameWidth, frameGeometry().bottom())));
}

// 设置面板
void MainWindow::on_setting_clicked()
{
    login loginWindow(this);
    loginWindow.exec();

    if(isLogin){
        on_refresh_clicked();
    }

    // refreshBackground
    refreshBackground();
}

// 刷新内容
void MainWindow::on_refresh_clicked()
{
    //LOG_INFO("refresh time : %lld", refreshTime)
    if(refreshTime != 0 && std::time(nullptr) - refreshTime <= 3){
        //loadList(TodoListControl::getUndoList());
        LOG_INFO("fresh too ofen, load from local")
        return;
    }
    refreshTime = std::time(nullptr);

    if(!isLogin){
        // try do login
        ServerConnectControl control;
        control.loginToServer();
    }

    // refresh list
    TodoListControl::loadFromServer();
    loadList(TodoListControl::getUndoList());

    // change icon
    if(isShowHistory){
        loadList(TodoListControl::getUndoList());
        GraphicsColorSvgItem svg_clear(":/resource/history_close.svg");
        ui->history->setIcon(svg_clear.setColor(iconColor));
    }
    isShowHistory = false;
}

// 窗口锁定
void MainWindow::on_lock_clicked()
{
    // lock location
    if (isLocked){
        showNormal();
        GraphicsColorSvgItem svg_lock(":/resource/lock.svg");
        ui->lock->setIcon(svg_lock.setColor(iconColor));
        // 画边框
        setStyleSheet(".QWidget{border:1px solid "+ iconColor.name() +"}");
    } else {
        showNormal();
        GraphicsColorSvgItem svg_lock(":/resource/locked.svg");
        ui->lock->setIcon(svg_lock.setColor(iconColor));
        setStyleSheet("");
    }
    isLocked = !isLocked;
    ClientSqlite sqlite;
}

// 清理Done动作
void MainWindow::clearDone(){
    TodoListControl::clearDoneList();
    on_history_clicked();
}

// 右键菜单
void MainWindow::on_ToDoListWin_customContextMenuRequested(const QPoint &pos)
{
    if(!isShowHistory){
        return;
    }

    Q_UNUSED(pos)
    QMenu* menu = new QMenu(this);                                         //创建菜单
    menu->addAction(clearAction);
    menu->exec(QCursor::pos());                                            //在鼠标光标位置显示右键快捷菜单
    delete menu;
}

// 刷新window中的列表
void MainWindow::loadList(std::vector<MsgOpPack> packList){
    ui->ToDoListWin->clear();
    for(auto it : packList){
        ToDoListItem *addTag = new ToDoListItem(this);
        addTag->setItemData(it);
        connect(addTag, SIGNAL(deleteTodoSignal()), this, SLOT(deleteTodoClicked()));

        QListWidgetItem *addNew = new QListWidgetItem();
        addNew->setSizeHint(QSize(100, 40));

        ui->ToDoListWin->addItem(addNew);
        ui->ToDoListWin->setItemWidget(addNew, addTag);
    }
}

// 历史按钮
void MainWindow::on_history_clicked()
{
    isShowHistory = !isShowHistory;
    if(isShowHistory){
        loadList(TodoListControl::getDoneList());
        GraphicsColorSvgItem svg_clear(":/resource/history_open.svg");
        ui->history->setIcon(svg_clear.setColor(iconColor));
    }else{
        loadList(TodoListControl::getUndoList());
        GraphicsColorSvgItem svg_clear(":/resource/history_close.svg");
        ui->history->setIcon(svg_clear.setColor(iconColor));
    }

}
