#include "newuser.h"
#include "ui_newuser.h"

newUser::newUser(QWidget *parent, SettingCtrl *setting_ctrl, SyncData *sync_data) :
        QDialog(parent),
        ui(new Ui::newUser),
        m_setting_ctrl(setting_ctrl),
        m_sync_data(sync_data)
{
    ui->setupUi(this);

    refreshBackground();
    ui->server_port->setPlaceholderText("server:port");
    std::string t_server = m_setting_ctrl->get_string(SETTING_SERVER);
    if(!t_server.empty()){
        ui->server_port->setText(QString::fromStdString(t_server));
    }

    ui->username->setPlaceholderText("username");
    ui->passwd->setPlaceholderText("password");
    ui->repasswd ->setPlaceholderText("rewrite password");
    ui->username->setFocus();

    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("background-color:transparent");

    QFont font = ui->server_port->font();
    font.setPixelSize(m_setting_ctrl->get_int(SETTING_FONT_SIZE)-2);
    ui->server_port->setFont(font);
    ui->username->setFont(font);
    ui->passwd->setFont(font);
    ui->repasswd->setFont(font);

    //设置无边框和设置隐藏下部图标
#ifdef LINUX_CLIENT
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool);
#endif
#ifdef WINDOW_CLIENT
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::SubWindow);
#endif
}

newUser::~newUser()
{
    delete ui;
}
void newUser::paintEvent(QPaintEvent *event)
{
    event->ignore();
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(255, 255, 255, 255 - m_setting_ctrl->get_int(SETTING_TRAN_POS)));

    // 主界面位置
    QRect new_rect;
    int bound = 10;
    new_rect.setX(rect().x() + bound);
    new_rect.setY(rect().y() + bound);
    new_rect.setWidth(this->rect().width() - 2*bound);
    new_rect.setHeight(this->rect().height() - 2*bound);

    // 设置主界面圆角蒙版
    QBitmap bmp(rect().size());
    bmp.fill();
    QPainter painter_bmp(&bmp);
    painter_bmp.setRenderHint(QPainter::Antialiasing, true);
    painter_bmp.setPen(Qt::NoPen);
    painter_bmp.setBrush(Qt::black);
    painter_bmp.drawRoundedRect(new_rect, 10, 10, Qt::AbsoluteSize);
    setMask(bmp);

    // 画边界的框
    painter.setPen(m_setting_ctrl->get_color(SETTING_ICON_COLOR));
    QRect lineRect = new_rect;
    lineRect.setHeight(lineRect.height() - 1);
    lineRect.setWidth(lineRect.width() - 1);
    painter.drawRoundedRect(lineRect, 10, 10, Qt::AbsoluteSize);
}

void newUser::refreshBackground(){
    QColor t_icon_color = m_setting_ctrl->get_color(SETTING_ICON_COLOR);
    SvgColor svg_server(":/resource/server.svg");
    ui->S->setIcon(svg_server.setColor(t_icon_color));

    SvgColor svg_username(":/resource/user.svg");
    ui->U->setIcon(svg_username.setColor(t_icon_color));

    SvgColor svg_passwd(":/resource/passwd.svg");
    ui->P1->setIcon(svg_passwd.setColor(t_icon_color));
    ui->P2->setIcon(svg_passwd.setColor(t_icon_color));

    QColor t_font_color = m_setting_ctrl->get_color(SETTING_FONT_COLOR);
    QPalette palette;
    palette.setColor(QPalette::Text, t_font_color);
    palette.setColor(QPalette::PlaceholderText, t_font_color);
    ui->server_port->setPalette(palette);
    ui->username->setPalette(palette);
    ui->passwd->setPalette(palette);
    ui->repasswd->setPalette(palette);
}

void newUser::on_ok_clicked()
{
    QString QS_server_port = ui->server_port->text().trimmed();
    QString QS_username = ui->username->text().trimmed();
    QString QS_passwd = ui->passwd->text();
    QString QS_re_passwd = ui->repasswd->text();

    if(nullptr == QS_server_port){
        ui->server_port->setFocus();
        MessagesBox::warn(this, NEW_USER_SERVER_NULL, m_setting_ctrl);
    } else if (nullptr == QS_username) {
        ui->username->setFocus();
        MessagesBox::warn(this, NEW_USER_USERNAME_NULL, m_setting_ctrl);
    } else if (nullptr == QS_passwd) {
        ui->passwd->setFocus();
        MessagesBox::warn(this, NEW_USER_PASSWORD_NULL, m_setting_ctrl);
    } else if (nullptr == QS_re_passwd) {
        ui->repasswd->setFocus();
        MessagesBox::warn(this, NEW_USER_PASSWORD_NULL, m_setting_ctrl);
    } else if (0 != QS_passwd.compare(QS_re_passwd)) {
        ui->passwd->clear();
        ui->repasswd->clear();
        ui->passwd->setFocus();
        MessagesBox::warn(this, NEW_USER_PASSWD_INCPABLE, m_setting_ctrl);
    } else {
        // read data and create user
        // char server_port[SETTING_SERVER_IP_SIZE + SETTING_SERVER_PORT_SIZE]{};
        // strcpy(server_port, QS_server_port.toUtf8().data());

        m_setting_ctrl->set_string(SETTING_USERNAME, QS_username.toStdString());
        m_setting_ctrl->set_string(SETTING_PASSWORD, QS_passwd.toStdString());
/*
        // do login
        ConnectCtrl createUserCtrl;
        createUserCtrl.parserServerPort(server_port);
        NetStatus state = createUserCtrl.createNewUser();

        if (state == CreateUserSuccess) {
            accept();
        } else if (state == CreateUserUserExists) {
            ui->username->clear();
            ui->username->setFocus();
            MessagesBox::warn(this, NEW_USER_USER_EXITS);
        } else if (state == CreateUserUndefinedError) {
            MessagesBox::warn(this, NEW_USER_SERVER_ERROR);
        } else {
            MessagesBox::warn(this, NEW_USER_SERVER_CON_ERROR);
        }
        */
    }
}

void newUser::on_cancel_clicked()
{
    reject();
}

void newUser::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->buttons() & Qt::LeftButton){
        move(event->globalPosition().x() - m_mouse_click_x, event->globalPosition().y() - m_mouse_click_y);
    }
}

void newUser::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
    {
        m_mouse_click_x = event->position().x();
        m_mouse_click_y = event->position().y();
    }
}
