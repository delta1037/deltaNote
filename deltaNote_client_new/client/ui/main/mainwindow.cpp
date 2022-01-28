#include <QGuiApplication>
#include <unistd.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 把所有的配置加载到表层一份
    m_setting_ctrl.load_all_setting();
    m_sync_data = new CSyncData(&m_setting_ctrl, &m_data_ctrl);

    // 初始化一些子界面
    m_login_window = new login(this, &m_setting_ctrl, m_sync_data);

    // 程序刚刚启动,不现实历史TODO列表，刷新时间默认为0
    m_is_show_history = false;
    m_location_change = false;
    last_refresh_time = 0;
    is_editing = false;

    // 连接事件
    event_connect();

    // 设置背景,系统图标显示,位置
    set_background();

    // 设置鼠标
    set_mouse();

    // 设置右键图标
    set_menu();

    // 通知模块前提：trayIcon初始化；m_data_ctrl初始化；m_setting_ctrl初始化
    // 连接完事件之后，系统图标就有了，把通知模块搞起来
    m_reminder = new Reminder(&m_data_ctrl, &m_setting_ctrl, trayIcon);

    // 前提：通知模块搞完，因为这里需要注册通知了
    on_refresh_clicked();     // 刷新主窗口内容，与服务端进行同步
    // 初始化完里面一个都没有，整一个空的
    if(0 == ui->ToDoListWin->count()){
        d_ui_debug("%s", "todo list init window is null, add new todo");
        add_new_todo_item();
    }
    m_desktop_rect = QGuiApplication::screens()[0]->availableGeometry();

    // 程序启动检查更新
    // updateMessage();
}

void MainWindow::set_mouse(){
    //开启鼠标追踪
    setMouseTracking(true);

    // 检测边缘
    m_edge_margin = 3;
    // 初始化
    m_dir_resize = Dir_None;
}

void MainWindow::set_menu(){
    // 设置右键菜单
    ui->ToDoListWin->setContextMenuPolicy(Qt::CustomContextMenu);

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(action_official);
    trayIconMenu->addAction(action_setting);
    trayIconMenu->addAction(action_update);
    trayIconMenu->addAction(action_quit);

    QIcon icon = QIcon(":/deltaNote.ico");
    trayIcon = new QSystemTrayIcon(icon, this);
    trayIcon->setToolTip("deltaNote");
    trayIcon->setVisible(true);
    trayIcon->show();
    trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::event_connect(){
    // 系统托盘图标事件连接
    action_official = new QAction(MAIN_WIN_OFFICIAL_SITE, this);
    connect(action_official, SIGNAL(triggered()), this, SLOT(do_open_official()));   // 打开官网动作
    action_setting = new QAction(MAIN_WIN_SETTING, this);
    connect(action_setting, SIGNAL(triggered()), this, SLOT(on_setting_clicked()));  // 打开设置动作
    action_update = new QAction(MAIN_WIN_UPDATE, this);
    connect(action_update, SIGNAL(triggered()), this, SLOT(do_check_update()));           // 检查更新
    action_quit = new QAction(MAIN_WIN_QUIT, this);
    connect(action_quit, SIGNAL(triggered()), qApp, SLOT(quit()));                   // 关闭应用，qApp对应的是程序全局唯一指针
    action_clear = new QAction(MAIN_WIN_CLEAR_DONE, this);
    connect(action_clear, SIGNAL(triggered()), this, SLOT(do_clear_done()));             // 清理所有已做动作

    // 其它事件连接
    connect(ui->ToDoListWin, SIGNAL(addSignal()), this, SLOT(do_add_clicked()));// 添加新TODO动作

    // 定时刷新
    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, [this](){ sync_todo_list(true, true); });
    refreshTimer->start(TIMER_REFRESH);

    // 定时更新
    //uploadTimer = new QTimer(this);
    //connect(uploadTimer, SIGNAL(timeout()), this, SLOT(uploadLocalTodo()));
    //uploadTimer->start(TIMER_UPLOAD);

    // 定时处理通知内容
    remind_timer = new QTimer(this);
    connect(remind_timer, SIGNAL(timeout()), this, SLOT(sync_reminder()));
    remind_timer->start(TIMER_REMINDER);

    // 设置界面刷新界面宽度
    connect(m_login_window, SIGNAL(refresh_width()), this, SLOT(refresh_width()));
    connect(m_login_window, SIGNAL(refresh_icon_color()), this, SLOT(refresh_icon_color()));
    connect(m_login_window, SIGNAL(refresh_font_color()), this, SLOT(refresh_font_color()));
    connect(m_login_window, SIGNAL(refresh_bg_color()), this, SLOT(update()));
}

void MainWindow::set_background(){
    // 移动窗口和设置窗口位置
    move(
            m_setting_ctrl.get_int(SETTING_X_POS),
            m_setting_ctrl.get_int(SETTING_Y_POS)
    );
    setGeometry(
            m_setting_ctrl.get_int(SETTING_X_POS),
            m_setting_ctrl.get_int(SETTING_Y_POS),
            m_setting_ctrl.get_int(SETTING_WIDTH),
            m_setting_ctrl.get_int(SETTING_HEIGHT)
    );

    // 设置透明
    setAttribute(Qt::WA_TranslucentBackground);
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
    // 启动时清空内容
    ui->ToDoListWin->clear();

    // 刷新界面
    refresh_background();
}
MainWindow::~MainWindow(){
    delete m_sync_data;
    delete ui;
}

void MainWindow::set_edit_status(bool edit_status){
    d_ui_debug("edit_status set to %d", edit_status)
    is_editing = edit_status;
}

// 通知定时操作内容
void MainWindow::sync_reminder() {
    if(m_reminder != nullptr){
        d_ui_debug("%s", "check and notify")
        if(m_reminder->check_and_notify()){
            // 发起过通知才更新列表
            sync_todo_list();
            d_ui_debug("%s", "sync_reminder do _sync_todo_list")
        }
    }
}

void MainWindow::on_setting_clicked(){
    d_ui_debug("main desktop w:%d h:%d", m_desktop_rect.width(), m_desktop_rect.height())

    // 显示setting界面
    //login m_login_window(this, &m_setting_ctrl, m_sync_data);
    m_login_window->exec();

    // 列表内容重绘，这里可能在设置里做了登录操作，所以需要做完整的刷新操作
    d_ui_debug("%s", "on_setting_clicked do _on_refresh_clicked")
    on_refresh_clicked();

    // 新的配置过来可能需要刷新界面
    // refresh_background();
    // QTimer::singleShot(1, this, SLOT(refresh_background()));
}

void MainWindow::on_refresh_clicked(){
    d_ui_debug("%s", "do refresh auto or click")
    // change icon
    if(m_is_show_history){
        SvgColor svg_clear(":/resource/history_close.svg");
        ui->history->setIcon(svg_clear.setColor(m_setting_ctrl.get_color(SETTING_ICON_COLOR)));
    }
    m_is_show_history = false;
    // 刷新重置状态
    set_edit_status(false);

    // 刷新界面:先做本地同步刷新，再做网络异步刷新
    d_ui_debug("%s", "on_refresh_clicked do _sync_todo_list")
    sync_todo_list();
    sync_todo_list(true, true);
}

// 锁定操作
void MainWindow::on_lock_clicked(){
    // 如果窗口进行了移动操作
    if(m_location_change){
        m_location_change = false;
        m_setting_ctrl.set_int(SETTING_X_POS, this->geometry().x());
        m_setting_ctrl.set_int(SETTING_Y_POS, this->geometry().y());
        m_setting_ctrl.set_int(SETTING_WIDTH, this->geometry().width());
        m_setting_ctrl.set_int(SETTING_HEIGHT, this->geometry().height());
    }

    // 点击锁定或者解锁
    if (m_setting_ctrl.get_bool(SETTING_IS_LOCK)){
        SvgColor svg_lock(":/resource/lock.svg");
        ui->lock->setIcon(svg_lock.setColor(m_setting_ctrl.get_color(SETTING_ICON_COLOR)));
        // 画边框
        setStyleSheet(".QWidget{border:1px solid "+ m_setting_ctrl.get_color(SETTING_ICON_COLOR).name() +"}");
        m_setting_ctrl.set_bool(SETTING_IS_LOCK, false);
    } else {
        SvgColor svg_lock(":/resource/locked.svg");
        ui->lock->setIcon(svg_lock.setColor(m_setting_ctrl.get_color(SETTING_ICON_COLOR)));
        // 去掉边框
        setStyleSheet("");
        m_setting_ctrl.set_bool(SETTING_IS_LOCK, true);
    }
}

void MainWindow::on_history_clicked(){
    m_is_show_history = !m_is_show_history;
    if(m_is_show_history){
        SvgColor svg_clear(":/resource/history_open.svg");
        ui->history->setIcon(svg_clear.setColor(m_setting_ctrl.get_color(SETTING_ICON_COLOR)));
    }else{
        SvgColor svg_clear(":/resource/history_close.svg");
        ui->history->setIcon(svg_clear.setColor(m_setting_ctrl.get_color(SETTING_ICON_COLOR)));
    }

    // 如果停留在历史页面，就禁止刷新了
    if(m_is_show_history){
        // 重绘显示内容，如果与自动刷新冲突，就要等一会
        d_ui_debug("%s", "on_history_clicked do _sync_todo_list")
        sync_todo_list(false, false, true);
        set_edit_status(true);
    }else{
        set_edit_status(false);
        // 重绘显示内容，如果与自动刷新冲突，就要等一会
        d_ui_debug("%s", "on_history_clicked do _sync_todo_list")
        sync_todo_list(false, false, true);
    }
}

void MainWindow::do_add_clicked(){
    d_ui_debug("%s", "recv a add signal")
    if(m_is_show_history){
        return;
    }

    // 新增一个条目
    if(0 == ui->ToDoListWin->count()){
        d_logic_debug("%s", "todo list window is null, add new todo");
        add_new_todo_item();
        return;
    }
    // 获取最后一条，最后一条是新创建的，还没编辑
    QListWidgetItem *item_data = ui->ToDoListWin->item(ui->ToDoListWin->count() - 1);
    auto *todo = qobject_cast<ToDoListItem*>(ui->ToDoListWin->itemWidget(item_data));
    if(todo->get_item_data().data.empty()){
        // 判断是否是新生成的，如果不是，获取一下信息，做一下删除操作，再清理该条目
        if(todo->get_item_status() == Item_old){
            TodoItem todo_data = todo->get_item_data();
            del_todo_item(&(todo_data));
        }
        delete item_data;
        delete todo;
    } else {
        add_new_todo_item();
    }
}

void MainWindow::sync_todo_list(bool async, bool net_sync, bool is_wait){
    // 正在编辑不允许刷新
    if(is_editing){
        d_logic_warn("%s", "item is editing now")
        return;
    }

    // 准入控制，防止多个刷新同时进行，造成混乱（这是由于自动刷新是另一个线程）
    /**
     * 刷新的三个线程
     * 1、主进程
     * 2、自动刷新进程
     * 3、通知进程
     */
    static bool is_sync = false;
    if(is_sync){
        d_ui_info("%s", "sync is doing, wait for another sync")
        if(!is_wait){
            // 不等就直接返回
            return;
        }
        // 如果选择等待，等一秒之后还没等到，就不等了
        d_ui_warn("%s", "sync is doing, wait for 1s")
        sleep(1);
        if(is_sync){
            return;
        }
    }
    is_sync = true;
    // 如果需要异步则转异步处理
    if(async){
        is_sync = false;
        d_ui_debug("%s", "use async !!!")
        QTimer::singleShot(1, this, [net_sync,is_wait,this](){ sync_todo_list(false, net_sync, is_wait); });
        return;
    }

    // 如果需要网络刷新先进行网络刷新
    if(net_sync){
        d_ui_debug("%s", "do net sync !!!")
        // 网络刷新刷新频率
        if(last_refresh_time != 0 && std::time(nullptr) - last_refresh_time <= 3){
            d_ui_debug("%s", "net sync too fast")
            is_sync = false;
            return;
        }
        last_refresh_time = std::time(nullptr);

        // TODO 与服务器同步，获取并更新数据
    }
    ErrorCode error_code;
    TodoList ret_list;
    int ret = m_data_ctrl.sel_todo(ListType_UI, ret_list, error_code);
    if(ret == SQLITE_ERROR){
        d_logic_error("%s", "DataCtrlUI sel_todo error")
        is_sync = false;
        return;
    }
    d_logic_debug("%s", "DataCtrlUI sel_todo success")

    // 删除旧的显示
    ui->ToDoListWin->clear();
    m_list_map_lock.lock();
    m_list_map.clear();
    m_list_map_lock.unlock();
    m_reminder->clear_registered(); // 通知模块也全都清理掉，注册新的内容
    // 更新界面
    for(auto it : ret_list){
        // 只显示其中一种
        if(!m_is_show_history && it.is_check == Check_false){
            add_new_todo_item(&it);
        }else if(m_is_show_history && it.is_check == Check_true){
            add_new_todo_item(&it);
        }
    }
    is_sync = false;
}

// 内容编辑完成处理
void MainWindow::ui_alt_todo(const std::string& key, AltType alt_type){
    if(alt_type == Alt_add){
        d_ui_debug("alt type : %d", alt_type)
        m_list_map_lock.lock();
        TodoItem item_data = m_list_map[key]->get_item_data();
        m_list_map_lock.unlock();
        ErrorCode error_code;
        int ret = m_data_ctrl.add_todo(item_data, error_code);
        if(ret != RET_SUCCESS){
            d_ui_error("add new todo %s:%s error", item_data.create_key.c_str(), item_data.data.c_str())
            return;
        }
        d_ui_debug("add new todo %s:%s success", item_data.create_key.c_str(), item_data.data.c_str())
    }else if(alt_type == Alt_alt || alt_type == Alt_chk){
        d_logic_debug("alt type : %d", alt_type)
        m_list_map_lock.lock();
        TodoItem item_data = m_list_map[key]->get_item_data();
        m_list_map_lock.unlock();
        ErrorCode error_code;
        int ret = m_data_ctrl.alt_todo(item_data, error_code);
        if(ret != RET_SUCCESS){
            d_ui_error("alt todo %s:%s error", item_data.create_key.c_str(), item_data.data.c_str())
            return;
        }
        d_ui_debug("alt todo %s:%s success", item_data.create_key.c_str(), item_data.data.c_str())
    }else{
        d_ui_error("unknown alt type : %d", alt_type)
        return;
    }
    // 重绘前端
    d_ui_debug("%s", "ui_alt_todo do _sync_todo_list")
    set_edit_status(false); // 重置编辑状态
    // 这里重绘是：可能有check操作；修改操作可能修改了优先级；新增操作可能新增了高优先级的
    sync_todo_list(false, false, true);
}

// 删除单条内容
void MainWindow::del_todo_item(TodoItem *item_data){
    ErrorCode error_code;
    int ret = m_data_ctrl.del_todo(item_data->create_key, error_code);
    if(ret != RET_SUCCESS){
        d_ui_error("del todo %s:%s error", item_data->create_key.c_str(), item_data->data.c_str())
        return;
    }
    d_ui_debug("del todo %s:%s success", item_data->create_key.c_str(), item_data->data.c_str())
}

void MainWindow::add_new_todo_item(TodoItem *item_data){
    d_ui_debug("%s", "add_new_todo_item")
    // 点击空白准备新建
    auto *widget = new ToDoListItem(this, &m_setting_ctrl);
    m_list_map_lock.lock();
    if(item_data != nullptr){
        // 从已有创建
        widget->set_item_data(*item_data);
        m_list_map[item_data->create_key] = widget;
        // 注册reminder，由reminder做提醒控制
        if(!item_data->reminder.empty() && item_data->is_check == Check_false){
            m_reminder->register_notify(item_data->create_key, item_data->reminder);
        }
    }else{
        // 从无到有
        TodoItem t_item_data;
        t_item_data.create_key = get_time_key();
        widget->set_item_data(t_item_data, Item_new);
        m_list_map[t_item_data.create_key] = widget;
    }
    m_list_map_lock.unlock();
    // 添加状态修改通知
    connect(widget, SIGNAL(todo_alt_signal(std::string, AltType)), this, SLOT(ui_alt_todo(std::string, AltType)));
    // 添加正在编辑的通知
    connect(widget, SIGNAL(todo_edit_status(bool)), this, SLOT(set_edit_status(bool)));

    auto *item = new QListWidgetItem();
    item->setSizeHint(QSize(100, 40));
    item->setFlags(Qt::ItemFlag::NoItemFlags);

    ui->ToDoListWin->addItem(item);
    ui->ToDoListWin->setItemWidget(item, widget);
}

void MainWindow::do_clear_done(){
    // 清理掉所有的已完成内容
    ErrorCode error_code;
    TodoList ret_list;
    int ret = m_data_ctrl.sel_todo(ListType_UI, ret_list, error_code);
    if(ret == SQLITE_ERROR){
        d_logic_error("%s", "MainWindow sel_todo error")
        return;
    }
    d_logic_debug("%s", "MainWindow sel_todo success")

    for(auto it : ret_list){
        if(it.is_check == Check_true){
            del_todo_item(&it);
        }
    }

    // 重新渲染前端
    if(m_is_show_history){
        SvgColor svg_clear(":/resource/history_close.svg");
        ui->history->setIcon(svg_clear.setColor(m_setting_ctrl.get_color(SETTING_ICON_COLOR)));
    }
    m_is_show_history = false;
    // 重设状态
    set_edit_status(false);

    // 从数据库加载数据并刷新界面（从本地刷新即可）
    d_ui_debug("%s", "do_clear_done do _sync_todo_list")
    sync_todo_list();
}

void MainWindow::do_open_official(){
    QDesktopServices :: openUrl(QUrl(QLatin1String("http://www.delta1037.cn/2019/11/23/deltaNoteSite/")));
}

void MainWindow::do_check_update(){

}

void MainWindow::refresh_background(){
    // refresh block
    //setStyleSheet("background-color:transparent;");

    //ui->setting->setStyleSheet("background-color:transparent");
    //ui->refresh->setStyleSheet("background-color:transparent");
    //ui->lock->setStyleSheet("background-color:transparent");
    //ui->history->setStyleSheet("background-color:transparent");

    // 图标颜色修改
    refresh_icon_color();

    // 宽度修改的时候
    refresh_width();
}

void MainWindow::refresh_width(){
    // 宽度修改的时候
    setGeometry(QRect(QPoint(frameGeometry().left(), frameGeometry().top()),
                      QPoint(frameGeometry().left() + m_setting_ctrl.get_int(SETTING_WIDTH), frameGeometry().bottom())));

}

void MainWindow::refresh_icon_color(){
    // change icon color
    SvgColor svg_refresh(":/resource/refresh.svg");
    ui->refresh->setIcon(svg_refresh.setColor(m_setting_ctrl.get_color(SETTING_ICON_COLOR)));

    SvgColor svg_setting(":/resource/setting.svg");
    ui->setting->setIcon(svg_setting.setColor(m_setting_ctrl.get_color(SETTING_ICON_COLOR)));

    if(m_setting_ctrl.get_bool(SETTING_IS_LOCK)){
        SvgColor svg_lock(":/resource/locked.svg");
        ui->lock->setIcon(svg_lock.setColor(m_setting_ctrl.get_color(SETTING_ICON_COLOR)));
    }else{
        SvgColor svg_lock(":/resource/lock.svg");
        ui->lock->setIcon(svg_lock.setColor(m_setting_ctrl.get_color(SETTING_ICON_COLOR)));
    }

    if(m_is_show_history){
        SvgColor svg_clear(":/resource/history_open.svg");
        ui->history->setIcon(svg_clear.setColor(m_setting_ctrl.get_color(SETTING_ICON_COLOR)));
    }else{
        SvgColor svg_clear(":/resource/history_close.svg");
        ui->history->setIcon(svg_clear.setColor(m_setting_ctrl.get_color(SETTING_ICON_COLOR)));
    }

    if (!m_setting_ctrl.get_bool(SETTING_IS_LOCK)){
        setStyleSheet(".QWidget{border:1px solid " + m_setting_ctrl.get_color(SETTING_ICON_COLOR).name() + "}");
    } else {
        setStyleSheet("");
    }
}

void MainWindow::refresh_font_color(){
    set_edit_status(true);
    m_list_map_lock.lock();
    for(const auto &it : m_list_map){
        it.second->update_render();
    }
    m_list_map_lock.unlock();
    set_edit_status(false);
}

#if 0
void MainWindow::updateMessage(){
    ConnectCtrl getUpdateLinkCtrl;
    string updateLink = "";
    string updateMessage = "";
    SyncStatus status = getUpdateLinkCtrl.checkUpdate(updateLink, updateMessage);

    if(status != ConnectError && status != AlreadyLatest){
#ifdef LINUX_CLIENT
    int ret = system("notify-send '更新提示' 'deltaNode有新版本，点击小图标检查更新' '-t' 5000");
    d_logic_debug("send massage status code : %d", ret);
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
    SyncStatus status = getUpdateLinkCtrl.checkUpdate(updateLink, updateMessage);

    if(status == ConnectError){
        MessagesBox::error(this, LOGIN_SERVER_CON_ERROR);
        d_logic_debug("%s", "update connect server error");
        return;
    }else if(status == AlreadyLatest){
        MessagesBox::info(this, UPDATE_ALREADY_NEW);
        d_logic_debug("%s", "update dont need");
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
        d_logic_debug("%s", "update fail");
    }else if(ret == QDialog::Accepted){
        d_logic_debug("%s", "get update file success");
        QString updateProgram = QCoreApplication::applicationDirPath() + "/" + UPDATE_PROGRAM;
        QProcess *logic = new QProcess();
        logic->setWorkingDirectory(QCoreApplication::applicationDirPath());
        logic->start(updateProgram, QStringList());
        d_logic_info("update program:%s", updateProgram.toLatin1().data());
        if(logic->waitForStarted() == true){
            d_logic_debug("%s", "start success");
            qApp->quit();
        }else{
            d_logic_debug("%s", "start fail");
        }
    }
}

#endif

// 右键菜单
void MainWindow::on_ToDoListWin_customContextMenuRequested(const QPoint &pos)
{
    if(!m_is_show_history){
        return;
    }

    Q_UNUSED(pos)
    QMenu* menu = new QMenu(this);                                         //创建菜单
    menu->addAction(action_clear);
    menu->exec(QCursor::pos());                                            //在鼠标光标位置显示右键快捷菜单
    delete menu;
}

void MainWindow::closeEvent(QCloseEvent *e){
    e->accept();
    qApp->quit();
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    if(event != nullptr){
        event->ignore();
    }

    QColor bg_color = m_setting_ctrl.get_color(SETTING_BG_COLOR);
    QPainter painter(this);
    painter.fillRect(
            this->rect(),
            QColor(bg_color.red(), bg_color.green(), bg_color.blue(), 255 - m_setting_ctrl.get_int(SETTING_TRAN_POS))
    );
}

void MainWindow::mouse_reset(){
    m_dir_resize = Dir_None;

    // 重置鼠标样式
    QCursor t_cursor;
    t_cursor = cursor();
    t_cursor.setShape(Qt::ArrowCursor);
    setCursor(t_cursor);
}

void MainWindow::detect_edge() {
    int diff_bottom = abs(cursor().pos().y() - frameGeometry().bottom());
    int diff_top = abs(cursor().pos().y() - frameGeometry().top());
    d_logic_debug("diff_b:%d, diff_t:%d", diff_bottom, diff_top)
    QCursor t_cursor;
    t_cursor = cursor();
    if(diff_bottom < m_edge_margin){
        m_dir_resize = Dir_Down;
        d_logic_debug("%s", "SizeVerCursor")
        t_cursor.setShape(Qt::SizeVerCursor);
    } else {
        m_dir_resize = Dir_None;
        if(diff_top < m_edge_margin + 2){
            t_cursor.setShape(Qt::SizeAllCursor);
            d_logic_debug("%s", "SizeAllCursor")
        }else{
            t_cursor.setShape(Qt::ArrowCursor);
            d_logic_debug("%s", "ArrowCursor")
        }
    }
    //重新设置鼠标,主要是改样式
    setCursor(t_cursor);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    if(m_setting_ctrl.get_bool(SETTING_IS_LOCK)){
        return;
    }

    if (event->buttons() & Qt::LeftButton){
        if(m_dir_resize == Dir_None){
            if(event->globalPosition().x() - m_mouse_click_x > m_desktop_rect.width() - 20
                    || event->globalPosition().x() - m_mouse_click_x <= 0
                    || event->globalPosition().y() - m_mouse_click_y > m_desktop_rect.height() - 50
                    || event->globalPosition().y() - m_mouse_click_y <= 0){
                return;
            }
            move(event->globalPosition().x() - m_mouse_click_x, event->globalPosition().y() - m_mouse_click_y);
            m_location_change = true;
        }else{
            qreal bottom = frameGeometry().bottom();
            qreal top = frameGeometry().top();
            if(height() == minimumHeight()){
                bottom = max(event->globalPosition().y(), top);
            }
            else if(height() == maximumHeight()){
                bottom = min(event->globalPosition().y(), top);
            }
            else{
                bottom = event->globalPosition().y();
            }
            setGeometry(QRect(
                QPoint(frameGeometry().left(), top),
                QPoint(frameGeometry().right(), bottom)
            ));
            m_location_change = true;
        }
    }else {
        detect_edge();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    d_logic_debug("mousePressEvent, lock:%d", m_setting_ctrl.get_bool(SETTING_IS_LOCK))
    if(m_setting_ctrl.get_bool(SETTING_IS_LOCK)){
        mouse_reset();
        return;
    }

    if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
    {
        m_mouse_click_x = event->position().x();
        m_mouse_click_y = event->position().y();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    event->ignore();
    d_logic_debug("mouseReleaseEvent, lock:%d", m_setting_ctrl.get_bool(SETTING_IS_LOCK))
    if(m_setting_ctrl.get_bool(SETTING_IS_LOCK)){
        return;
    }

    if(m_dir_resize != Dir_None){         //还原鼠标样式
        detect_edge();
    }
}
