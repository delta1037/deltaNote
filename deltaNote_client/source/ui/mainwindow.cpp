#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置透明
    setAttribute(Qt::WA_TranslucentBackground);

    // 设置右键菜单
    ui->ToDoListWin->setContextMenuPolicy(Qt::CustomContextMenu);

    // 跟踪鼠标调整窗口大小和位置
    setMouseTracking(true); //开启鼠标追踪
    edgeMargin = 3;
    resizeDir = nodir;

    // 系统托盘图标事件连接
    officialAction = new QAction(MAIN_WIN_OFFICIAL_SITE, this);
    connect(officialAction, SIGNAL(triggered()), this, SLOT(openOfficialSite()));   // 打开官网动作
    settingAction = new QAction(MAIN_WIN_SETTING, this);
    connect(settingAction, SIGNAL(triggered()), this, SLOT(on_setting_clicked()));  // 打开设置动作
    updateAction = new QAction(MAIN_WIN_UPDATE, this);
    connect(updateAction, SIGNAL(triggered()), this, SLOT(checkUpdate()));           // 检查更新
    quitAction = new QAction(MAIN_WIN_QUIT, this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));                   // 关闭应用，qApp对应的是程序全局唯一指针
    clearAction = new QAction(MAIN_WIN_CLEAR_DONE, this);
    connect(clearAction, SIGNAL(triggered()), this, SLOT(clearDone()));             // 清理所有已做动作

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(officialAction);
    trayIconMenu->addAction(settingAction);
    trayIconMenu->addAction(updateAction);
    trayIconMenu->addAction(quitAction);

    // 无边框和设置隐藏下部图标
#ifdef LINUX_CLIENT
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnBottomHint);

    QString styleSheet = "QPushButton:pressed{\n	  background-color:rgba(68, 91, 237, 10);\n}";
    ui->setting->setStyleSheet(styleSheet);
    ui->refresh->setStyleSheet(styleSheet);
    ui->lock->setStyleSheet(styleSheet);
    ui->history->setStyleSheet(styleSheet);
#endif
#ifdef WINDOW_CLIENT
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::SubWindow | Qt::WindowStaysOnBottomHint);
#endif
    QIcon icon = QIcon(":/deltaNote.ico");
    trayIcon = new QSystemTrayIcon(icon, this);
    trayIcon->setParent(this);
    trayIcon->setToolTip("deltaNote");
    trayIcon->setVisible(true);
    trayIcon->show();
    trayIcon->setContextMenu(trayIconMenu);

    // 其它事件连接
    connect(ui->ToDoListWin, SIGNAL(addTodoSignal()), this, SLOT(addTodoClicked()));// 添加新TODO动作

    // 移动窗口和设置窗口位置
    move(xPos, yPos);
    setGeometry(xPos, yPos, frameWidth, frameHeight);

    // 程序刚刚启动，没有空的TODO，不现实历史TODO列表，刷新时间默认为0
    haveNullTodo = false;
    isShowHistory = false;
    refreshTime = 0;

    // 记录主窗口
    mainWindowWidget = this;

    on_refresh_clicked();           // 刷新主窗口内容
    refreshBackground();            // 刷新背景

    // 定时刷新
    refreshTimer = new QTimer(this);
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(on_refresh_clicked()));
    refreshTimer->start(TIMER_REFRESH);

    // 定时更新
    uploadTimer = new QTimer(this);
    connect(uploadTimer, SIGNAL(timeout()), this, SLOT(uploadLocalTodo()));
    uploadTimer->start(TIMER_UPLOAD);

#ifdef LINUX_CLIENT
    dataSaveTimer = new QTimer(this);
    connect(dataSaveTimer, SIGNAL(timeout()), this, SLOT(dataAudoSave()));
    dataSaveTimer->start(TIMER_SAVE_DATA);

    settingChange = false;
#endif
    // 程序启动检查更新
    updateMessage();
}

MainWindow::~MainWindow()
{
    // 保存设置
    DataCtrl dataCtrl;
    dataCtrl.setAllSetting();

    // 保存列表数据
    std::vector<SocketMsgOpPack> listBuffer;
    for(auto &it : ListControl::getDoneList()){
        listBuffer.push_back(SocketMsgOpPack(it));
    }

    for(auto &it : ListControl::getUndoList()){
        listBuffer.push_back(SocketMsgOpPack(it));
    }

    dataCtrl.cleanLocalList();
    dataCtrl.saveLocalList(listBuffer);
    LogCtrl::info("deltaNote close, save all todo - size:%d", listBuffer.size());

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e){
    // 保存设置
    DataCtrl dataCtrl;
    dataCtrl.setAllSetting();

    // 保存列表数据
    std::vector<SocketMsgOpPack> listBuffer;
    for(auto &it : ListControl::getDoneList()){
        listBuffer.push_back(SocketMsgOpPack(it));
    }

    for(auto &it : ListControl::getUndoList()){
        listBuffer.push_back(SocketMsgOpPack(it));
    }

    dataCtrl.cleanLocalList();
    dataCtrl.saveLocalList(listBuffer);
    LogCtrl::info("deltaNote close save all todo - size:%d", listBuffer.size());

    e->accept();
}

void MainWindow::updateMessage(){
    ConnectCtrl getUpdateLinkCtrl;
    string updateLink = "";
    string updateMessage = "";
    MsgStatus status = getUpdateLinkCtrl.checkUpdate(updateLink, updateMessage);

    if(status != ConnectError && status != AlreadyLatest){
#ifdef LINUX_CLIENT
    int ret = system("notify-send '更新提示' 'deltaNode有新版本，点击小图标检查更新' '-t' 5000");
    LogCtrl::debug("send massage status code : %d", ret);
#endif
#ifdef WINDOW_CLIENT
    trayIcon->showMessage("更新提示", "deltaNode有新版本，点击小图标检查更新");
#endif
    }
}

void MainWindow::checkUpdate(){
    ConnectCtrl getUpdateLinkCtrl;
    string updateLink = "";
    string updateMessage = "";
    MsgStatus status = getUpdateLinkCtrl.checkUpdate(updateLink, updateMessage);

    if(status == ConnectError){
        MessagesBox::error(this, LOGIN_SERVER_CON_ERROR);
        LogCtrl::debug("update connect server error");
        return;
    }else if(status == AlreadyLatest){
        MessagesBox::info(this, UPDATE_ALREADY_NEW);
        LogCtrl::debug("update dont need");
        return;
    }

    QString QUpdateLink(updateLink.c_str());
    QString QUpdateMessage(updateMessage.c_str());

    CheckUpdate checkupdateDialog(this);
    checkupdateDialog.setUpdateLink(QUpdateLink);
    checkupdateDialog.setUpdateMessage(QUpdateMessage);
    int ret = checkupdateDialog.exec();
    if(ret == QDialog::Rejected){
        MessagesBox::error(this, UPDATE_FAIL);
        LogCtrl::debug("update fail");
    }else if(ret == QDialog::Accepted){
        LogCtrl::debug("get update file success");
        QString updateProgram = QCoreApplication::applicationDirPath() + "/" + UPDATE_PROGRAM;
        QProcess *deltaNote = new QProcess();
        deltaNote->setWorkingDirectory(QCoreApplication::applicationDirPath());
        deltaNote->start(updateProgram, QStringList());
        LogCtrl::info("update program:%s", updateProgram.toLatin1().data());
        if(deltaNote->waitForStarted() == true){
            LogCtrl::debug("start success");
            qApp->quit();
        }else{
            LogCtrl::debug("start fail");
        }
    }
}

void MainWindow::openOfficialSite(){
    QDesktopServices :: openUrl(QUrl(QLatin1String("http://www.delta1037.cn/2019/11/23/deltaNoteSite/")));
}

void MainWindow::deleteTodoClicked(){
    // 刷新窗口，删除已经删掉的item
    MsgOpPack last;
    if(isShowHistory){
        last = ListControl::getLastOfUndo();
    }else{
        last = ListControl::getLastOfDone();
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
    addNew->setFlags(Qt::ItemFlag::NoItemFlags);

    ui->ToDoListWin->addItem(addNew);
    ui->ToDoListWin->setItemWidget(addNew, addTag);
}

void MainWindow::addTodoClicked(){
    //LOG_INFO("click signal")
    if(isShowHistory){
        return;
    }

    // add new todo
    if(0 == ui->ToDoListWin->count()){
        LogCtrl::debug("todo list window is null, add new todo");
        addNewTodo();
        return;
    }

    QListWidgetItem *item = ui->ToDoListWin->item(ui->ToDoListWin->count() - 1);
    ToDoListItem *todo = qobject_cast<ToDoListItem*>(ui->ToDoListWin->itemWidget(item));
    if(todo->getTextData().size() == 0){
        // clear last null todo
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

// 设置面板
void MainWindow::on_setting_clicked()
{
    login loginWindow(this);
    loginWindow.exec();

    if(isLogin){
        on_refresh_clicked();
    }

    // refreshBackground
    QTimer::singleShot(1, this, SLOT(refreshBackground()));

#ifdef LINUX_CLIENT
    settingChange = true;
#endif
}

// 刷新内容
void MainWindow::on_refresh_clicked()
{
    // 控制刷新频率
    if(refreshTime != 0 && std::time(nullptr) - refreshTime <= 3){
        return;
    }
    refreshTime = std::time(nullptr);

    if(!isLogin){
        // try do login
        ConnectCtrl loginCtrl;
        loginCtrl.loginToServer();
    }

    // refresh list
    ListControl::loadFromServer();
    loadList(ListControl::getUndoList());

    // change icon
    if(isShowHistory){
        GraphicsColorSvgItem svg_clear(":/resource/history_close.svg");
        ui->history->setIcon(svg_clear.setColor(iconColor));
    }
    isShowHistory = false;
}

// 上传本地操作记录
void MainWindow::uploadLocalTodo(){
    if(ListControl::getOpList().size() > 0){
        ListControl::uploadToServer();
    }
}

// 窗口锁定
void MainWindow::on_lock_clicked()
{
    // lock location
    if (isLocked){
        GraphicsColorSvgItem svg_lock(":/resource/lock.svg");
        ui->lock->setIcon(svg_lock.setColor(iconColor));

        // 画边框
        setStyleSheet(".QWidget{border:1px solid "+ iconColor.name() +"}");
    } else {
        GraphicsColorSvgItem svg_lock(":/resource/locked.svg");
        ui->lock->setIcon(svg_lock.setColor(iconColor));

        // 去掉边框
        setStyleSheet("");
    }
    isLocked = !isLocked;

#ifdef LINUX_CLIENT
    settingChange = true;
#endif
}

// 历史按钮
void MainWindow::on_history_clicked()
{
    isShowHistory = !isShowHistory;
    if(isShowHistory){
        loadList(ListControl::getDoneList());
        GraphicsColorSvgItem svg_clear(":/resource/history_open.svg");
        ui->history->setIcon(svg_clear.setColor(iconColor));
    }else{
        loadList(ListControl::getUndoList());
        GraphicsColorSvgItem svg_clear(":/resource/history_close.svg");
        ui->history->setIcon(svg_clear.setColor(iconColor));
    }
}

// 清理Done动作
void MainWindow::clearDone(){
    ListControl::clearDoneList();
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
        addNew->setFlags(Qt::ItemFlag::NoItemFlags);

        ui->ToDoListWin->addItem(addNew);
        ui->ToDoListWin->setItemWidget(addNew, addTag);
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
    setStyleSheet("background-color:transparent;");

    ui->setting->setStyleSheet("background-color:transparent");
    ui->refresh->setStyleSheet("background-color:transparent");
    ui->lock->setStyleSheet("background-color:transparent");
    ui->history->setStyleSheet("background-color:transparent");

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

void MainWindow::detectEdge()
{
    int diffBottom = abs(cursor().pos().y() - frameGeometry().bottom());
    int diffTop = abs(cursor().pos().y() - frameGeometry().top());
    QCursor tempCursor;                                    //获得当前鼠标样式，注意:只能获得当前鼠标样式然后再重新设置鼠标样式
    tempCursor = cursor();                                 //因为获得的不是鼠标指针，所以不能这样用:cursor().setXXXXX
    if(diffBottom < edgeMargin){
        resizeDir = bottom;
        tempCursor.setShape(Qt::SizeVerCursor);
    } else {
        resizeDir = nodir;
        if(diffTop < edgeMargin + 2){
            tempCursor.setShape(Qt::SizeAllCursor);
        }else{
            tempCursor.setShape(Qt::ArrowCursor);
        }
    }
    setCursor(tempCursor);                    //重新设置鼠标,主要是改样式
}

void MainWindow::mouseSetArrow(){
    QCursor tempCursor;                                    //获得当前鼠标样式，注意:只能获得当前鼠标样式然后再重新设置鼠标样式
    tempCursor = cursor();                                 //因为获得的不是鼠标指针，所以不能这样用:cursor().setXXXXX
    resizeDir = nodir;
    tempCursor.setShape(Qt::ArrowCursor);
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
#ifdef LINUX_CLIENT
            settingChange = true;
#endif
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
#ifdef LINUX_CLIENT
            settingChange = true;
#endif
        }
    }else {
        detectEdge();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    event->ignore();

    if(isLocked){
        mouseSetArrow();
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

#ifdef LINUX_CLIENT
void MainWindow::dataAudoSave(){
    // 保存设置
    if(settingChange){
        LogCtrl::debug("linux device setting auto save");
        DataCtrl dataCtrl;
        dataCtrl.setAllSetting();
        settingChange = false;
    }

    // 保存列表数据
    if(ListControl::todoListChange){
        LogCtrl::debug("linux device data auto save");
        std::vector<SocketMsgOpPack> listBuffer;
        for(auto &it : ListControl::getDoneList()){
            listBuffer.push_back(SocketMsgOpPack(it));
        }

        for(auto &it : ListControl::getUndoList()){
            listBuffer.push_back(SocketMsgOpPack(it));
        }

        DataCtrl dataCtrl;
        dataCtrl.cleanLocalList();
        dataCtrl.saveLocalList(listBuffer);

        ListControl::todoListChange = false;
    }
}
#endif
