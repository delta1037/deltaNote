#include "login.h"
#include "ui_login.h"
#include "log.h"

using namespace std;

login::login(QWidget *parent, SettingCtrl *setting_ctrl, CSyncData *sync_data) :
        QDialog(parent),
        ui(new Ui::login),
        m_setting_ctrl(setting_ctrl),
        m_sync_data(sync_data) {
    ui->setupUi(this);
    m_desktop_rect = QGuiApplication::screens()[0]->availableGeometry();

    set_value();

    refresh_icon();
    refresh_text();

    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("background-color:transparent");

    //设置无边框和设置隐藏下部图标
#ifdef LINUX_CLIENT
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool);

    ui->label->setVisible(false);
    ui->radioButton->setVisible(false);

    QString styleSheet = "QPushButton:pressed{\n	  background-color:rgba(68, 91, 237, 10);\n}";
    ui->exit->setStyleSheet(styleSheet);
#endif
#ifdef WINDOW_CLIENT
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::SubWindow);
#endif
    // 将界面移到中心
    d_ui_debug("login desktop w:%d h:%d", m_desktop_rect.width(), m_desktop_rect.height())
    move(m_desktop_rect.width()/2 - this->width()/2, m_desktop_rect.height()/2 - this->height()/2);
}


login::~login() {
    delete ui;
}

void login::set_value() {
    ui->server_port->setPlaceholderText("server_port");
    string t_server = m_setting_ctrl->get_string(SETTING_SERVER);
    if(!t_server.empty()){
        ui->server_port->setText(QString::fromStdString(t_server));
    }

    string t_username = m_setting_ctrl->get_string(SETTING_USERNAME);
    if(m_setting_ctrl->get_bool(SETTING_IS_LOGIN)){
        ui->server_port->setReadOnly(true);
        ui->username->setText(QString::fromStdString(t_username));
        ui->username->setReadOnly(true);
        ui->password->setText("password");
        ui->password->setReadOnly(true);
        ui->Login->setText("Logout");
        ui->creteNewUser->hide();
    }else {
        ui->server_port->setReadOnly(false);
        ui->username->setPlaceholderText("username");
        ui->password->setPlaceholderText("password");
        ui->username->setReadOnly(false);
        ui->password->setReadOnly(false);

        if('\0' != t_username[0]){
            ui->username->setText(QString::fromStdString(t_username));
            ui->password->setFocus();
        }else{
            ui->username->setFocus();
        }
        ui->Login->setText("Login");
        ui->creteNewUser->show();
    }

    ui->transparent->setValue(m_setting_ctrl->get_int(SETTING_TRAN_POS) + 5);
    ui->fontSizeSlider->setValue(m_setting_ctrl->get_int(SETTING_FONT_SIZE));

    ui->mainWinWidthSlider->setValue(m_setting_ctrl->get_int(SETTING_WIDTH));

    ui->chooseFontColor->setStyleSheet("background-color:" + m_setting_ctrl->get_color(SETTING_FONT_COLOR).name() + ";");
    ui->choose_bg_color->setStyleSheet("background-color:" + m_setting_ctrl->get_color(SETTING_BG_COLOR).name() + ";");
    ui->chooseIconColor->setStyleSheet("background-color:" + m_setting_ctrl->get_color(SETTING_ICON_COLOR).name() + ";");

    ui->radioButton->setChecked(m_setting_ctrl->get_bool(SETTING_IS_AUTO_START));
}

void login::refresh_icon() {
    QColor t_icon_color = m_setting_ctrl->get_color(SETTING_ICON_COLOR);
    SvgColor svg_server(":/resource/server.svg");
    ui->S->setIcon(svg_server.setColor(t_icon_color));

    SvgColor svg_username(":/resource/user.svg");
    ui->U->setIcon(svg_username.setColor(t_icon_color));

    SvgColor svg_passwd(":/resource/passwd.svg");
    ui->P->setIcon(svg_passwd.setColor(t_icon_color));

    SvgColor svg_exit(":/resource/exit.svg");
    ui->exit->setIcon(svg_exit.setColor(t_icon_color));
}

void login::refresh_text() {
    QPalette palette;
    QFont font = ui->server_port->font();
    font.setPixelSize(m_setting_ctrl->get_int(SETTING_FONT_SIZE) - 2);
    palette.setColor(QPalette::Text, m_setting_ctrl->get_color(SETTING_FONT_COLOR));
    palette.setColor(QPalette::PlaceholderText, m_setting_ctrl->get_color(SETTING_FONT_COLOR));
    ui->server_port->setPalette(palette);
    ui->server_port->setFont(font);
    ui->username->setPalette(palette);
    ui->username->setFont(font);
    ui->password->setPalette(palette);
    ui->password->setFont(font);

    QPalette labelPal = ui->transparentLabel->palette();
    //font = ui->transparentLabel->font();
    //font.setPixelSize(fontSize);
    labelPal.setColor(QPalette::WindowText, m_setting_ctrl->get_color(SETTING_FONT_COLOR));
    ui->transparentLabel->setPalette(labelPal);
    //ui->transparentLabel->setFont(font);
    ui->fontSizeLabel->setPalette(labelPal);
    //ui->fontSizeLabel->setFont(font);
    ui->mainWinWidthLabel->setPalette(labelPal);
    //ui->mainWinWidthLabel->setFont(font);
    ui->auto_start->setPalette(labelPal);
}

void login::refresh_background() {
    // refresh block
    setStyleSheet("background-color:transparent");
}

void login::paintEvent(QPaintEvent *event)
{
    event->ignore();
    QPainter painter(this);
    QColor icon_color = m_setting_ctrl->get_color(SETTING_ICON_COLOR);
    QColor bg_color = m_setting_ctrl->get_color(SETTING_BG_COLOR);

    painter.fillRect(this->rect(), QColor(bg_color.red(), bg_color.green(), bg_color.blue(), 255 - m_setting_ctrl->get_int(SETTING_TRAN_POS)));
    //setStyleSheet("background-color:transparent");
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(icon_color.red(), icon_color.green(), icon_color.blue(), 255 - m_setting_ctrl->get_int(SETTING_TRAN_POS)));
    setPalette(pal);

    // 主界面位置
    QRect new_rect;
    int bound = 20;
    new_rect.setX(rect().x() + bound);
    new_rect.setY(rect().y() + bound);
    new_rect.setWidth(this->rect().width() - 2*bound);
    new_rect.setHeight(this->rect().height() - 2*bound);

    // 右上角按钮位置
    QRect buttonRect;
    buttonRect.setX(rect().x() + this->rect().width() - 2*bound);
    buttonRect.setY(rect().y());
    buttonRect.setWidth(2*bound);
    buttonRect.setHeight(2*bound);

    // 设置圆角蒙版
    QBitmap bmp(rect().size());
    bmp.fill();
    QPainter painter_bmp(&bmp);
    painter_bmp.setRenderHint(QPainter::Antialiasing, true);
    painter_bmp.setPen(Qt::NoPen);
    painter_bmp.setBrush(Qt::black);
    painter_bmp.drawRoundedRect(buttonRect, 20, 20, Qt::AbsoluteSize);
    painter_bmp.drawRoundedRect(new_rect, 20, 20, Qt::AbsoluteSize);
    setMask(bmp);

    // 画边界的框
    painter.setPen(m_setting_ctrl->get_color(SETTING_ICON_COLOR));
    QRect lineRect = new_rect;
    lineRect.setHeight(lineRect.height() - 1);
    lineRect.setWidth(lineRect.width() - 1);
    painter.drawRoundedRect(lineRect, 20, 20, Qt::AbsoluteSize);
    /*
    painter.setPen(QColor(0,0,0,20));
    QRect arcRect = buttonRect;
    arcRect.setX(arcRect.x() - 20);
    arcRect.setY(arcRect.y() + 20);
    arcRect.setWidth(2*bound);
    arcRect.setHeight(2*bound);
    //painter.drawArc(arcRect, 0, 1600);
    painter_bmp.drawRoundedRect(arcRect, 20, 20 , Qt::AbsoluteSize);
    */
}

void login::doLogin(){
    QString QS_server_port = ui->server_port->text().trimmed();
    QString QS_username = ui->username->text().trimmed();
    QString QS_passwd = ui->password->text();

    if(nullptr == QS_server_port){
        ui->server_port->setFocus();
        //MessagesBox::warn(this, LOGIN_SERVER_NULL);
    } else if (nullptr == QS_username) {
        ui->username->setFocus();
        //MessagesBox::warn(this, LOGIN_USERNAME_NULL);
    } else if (nullptr == QS_passwd) {
        ui->password->setFocus();
        //MessagesBox::warn(this, LOGIN_PASSWORD_NULL);
    } else {
        // get server_port conn to server
        //char server_port[SETTING_SERVER_IP_SIZE + SETTING_SERVER_PORT_SIZE]{};
        //strcpy(server_port, QS_server_port.toLatin1().data());

        // get username and passwd
        m_setting_ctrl->set_string(SETTING_USERNAME, QS_username.toStdString());
        m_setting_ctrl->set_string(SETTING_PASSWORD, QS_passwd.toStdString());

        SyncStatus net_status;
        ErrorCode error_code;
        d_ui_debug("%s", "do sign in progress")
        m_sync_data->sync_sign_in(net_status, error_code);

        if (net_status == Sync_login_success) {
            accept();
        } else if(net_status == Sync_login_passwd_error) {
            ui->password->clear();
            ui->password->setFocus();
            MessagesBox::warn(this, LOGIN_PASSWORD_ERROR, m_setting_ctrl);
        } else if(net_status == Sync_login_user_not_exits) {
            ui->username->clear();
            ui->password->clear();
            ui->username->setFocus();
            MessagesBox::warn(this, LOGIN_USER_N_EXITS, m_setting_ctrl);
        } else if(net_status == Sync_sign_up_undefined_error) {
            ui->username->clear();
            ui->password->clear();
            ui->username->setFocus();
            MessagesBox::warn(this, LOGIN_SERVER_ERROR, m_setting_ctrl);
        } else {
            ui->password->clear();
            ui->password->setFocus();
            MessagesBox::warn(this, LOGIN_SERVER_CON_ERROR, m_setting_ctrl);
        }
    }
}

void login::doLogout(){
    m_setting_ctrl->set_bool(SETTING_IS_LOGIN, false);
    accept();
}

void login::on_Login_clicked()
{
    if(m_setting_ctrl->get_bool(SETTING_IS_LOGIN)){
        doLogout();
    }else{
        doLogin();
    }
}

void login::on_creteNewUser_clicked()
{
    newUser new_user(this, m_setting_ctrl, m_sync_data);
    setWindowOpacity(0);
    if (new_user.exec() == QDialog::Accepted ) {
        accept();
    }
    setWindowOpacity(1);
}

void login::on_cancel_clicked()
{
    reject();
}

void login::on_chooseFontColor_clicked() {
    setWindowOpacity(0);
    chooseColor color(this, m_setting_ctrl);
    color.exec();
    QColor c = color.get_color();
    d_ui_debug("%s", "setting font color")

    if(c.isValid()){
        d_ui_debug("%s", "setting font color success")
        m_setting_ctrl->set_color(SETTING_FONT_COLOR, c);
        ui->chooseFontColor->setStyleSheet("background-color:" + c.name()+ ";");
    }

    setWindowOpacity(1);
    refresh_text();
}

void login::on_choose_bg_color_clicked()
{
    setWindowOpacity(0);
    chooseColor color(this, m_setting_ctrl);
    color.exec();
    QColor c = color.get_color();
    d_ui_debug("%s", "setting font color")

    if(c.isValid()){
        d_ui_debug("%s", "setting bg color success")
        m_setting_ctrl->set_color(SETTING_BG_COLOR, c);
        ui->choose_bg_color->setStyleSheet("background-color:" + c.name()+ ";");
    }
    setWindowOpacity(1);
}

void login::on_chooseIconColor_clicked() {
    setWindowOpacity(0);
    chooseColor color(this, m_setting_ctrl);
    color.exec();
    QColor c = color.get_color();
    d_ui_debug("%s", "setting icon color")
    if(c.isValid()){
        d_ui_debug("%s", "setting icon color success")
        m_setting_ctrl->set_color(SETTING_ICON_COLOR, c);
        ui->chooseIconColor->setStyleSheet("background-color:" + c.name()+ ";");
    }

    setWindowOpacity(1);
    refresh_icon();
}

void login::on_transparent_sliderMoved(int position)
{
    if(position >=0 && position <=255){
        position -= 1;
        m_setting_ctrl->set_int(SETTING_TRAN_POS, position > 0 ? position : 0);
    }
}

void login::on_radioButton_clicked(bool checked)
{
    m_setting_ctrl->set_bool(SETTING_IS_AUTO_START, checked);
    QSettings regedit(REGEDIT_AUTO_START_PATH, QSettings::NativeFormat);
    if(checked) {
        QString sAppPath = QApplication::applicationFilePath();
        regedit.setValue(REGEDIT_KEY, QVariant(QDir::toNativeSeparators(sAppPath)));
    } else {
        regedit.setValue(REGEDIT_KEY, QVariant());
    }
}

void login::on_exit_clicked()
{
    accept();
}

void login::on_fontSizeSlider_sliderMoved(int position)
{
    if(position >= 9 && position <= 16){
        m_setting_ctrl->set_int(SETTING_FONT_SIZE, position);
    }
    refresh_text();
}

void login::on_mainWinWidthSlider_sliderMoved(int position)
{
    if(position >= 280 && position <= 560){
        m_setting_ctrl->set_int(SETTING_WIDTH, position);
    }
}

void login::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->buttons() & Qt::LeftButton){
        move(event->globalPosition().x() - m_mouse_click_x, event->globalPosition().y() - m_mouse_click_y);
    }
}

void login::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
    {
        m_mouse_click_x = event->position().x();
        m_mouse_click_y = event->position().y();
    }
}
