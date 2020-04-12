#include "ui/login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    refreshBackground();
    ui->server_port->setPlaceholderText("server_port");
    string server_port = string(g_server) + ":" + to_string(g_port);
    ui->server_port->setText(QString(server_port.data()));
    if(isLogin){
        ui->server_port->setReadOnly(true);
        ui->username->setText(g_username);
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
        if('\0' != g_username[0]){
            ui->username->setText(QString(g_username));
            ui->password->setFocus();
        }else{
            ui->username->setFocus();
        }
        ui->Login->setText("Login");
        ui->creteNewUser->show();
    }

    ui->transparent->setValue(transparentPos + 5);
    ui->fontSizeSlider->setValue(fontSize);

    ui->mainWinWidthSlider->setValue(frameWidth);

    ui->chooseFontColor->setStyleSheet("background-color:" + fontColor.name()+ ";");
    ui->chooseIconColor->setStyleSheet("background-color:" + iconColor.name()+ ";");

    ui->radioButton->setChecked(isAutoStart);

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
    move(desktopRect.width()/2 - this->width()/2, desktopRect.height()/2 - this->height()/2);
}

login::~login()
{
    delete ui;
}

void login::paintEvent(QPaintEvent *event)
{
    event->ignore();
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(255, 255, 255, 255 - transparentPos));
    //setStyleSheet("background-color:transparent");
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(255, 255, 255, 255 - transparentPos));
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
    painter.setPen(iconColor);
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

void login::refreshBackground(){
    // refresh block
    setStyleSheet("background-color:transparent");

    // change icon color
    GraphicsColorSvgItem svg_server(":/resource/server.svg");
    ui->S->setIcon(svg_server.setColor(iconColor));

    GraphicsColorSvgItem svg_username(":/resource/user.svg");
    ui->U->setIcon(svg_username.setColor(iconColor));

    GraphicsColorSvgItem svg_passwd(":/resource/passwd.svg");
    ui->P->setIcon(svg_passwd.setColor(iconColor));

    GraphicsColorSvgItem svg_exit(":/resource/exit.svg");
    ui->exit->setIcon(svg_exit.setColor(iconColor));

    QPalette palette;
    QFont font = ui->server_port->font();
    font.setPixelSize(fontSize-2);
    palette.setColor(QPalette::Text, fontColor);
    ui->server_port->setPalette(palette);
    ui->server_port->setFont(font);
    ui->username->setPalette(palette);
    ui->username->setFont(font);
    ui->password->setPalette(palette);
    ui->password->setFont(font);

    QPalette labelPal = ui->transparentLabel->palette();
    //font = ui->transparentLabel->font();
    //font.setPixelSize(fontSize);
    labelPal.setColor(QPalette::WindowText, fontColor);
    ui->transparentLabel->setPalette(labelPal);
    //ui->transparentLabel->setFont(font);
    ui->fontSizeLabel->setPalette(labelPal);
    //ui->fontSizeLabel->setFont(font);
    ui->mainWinWidthLabel->setPalette(labelPal);
    //ui->mainWinWidthLabel->setFont(font);
}

void login::doLogin(){
    QString QS_server_port = ui->server_port->text().trimmed();
    QString QS_username = ui->username->text().trimmed();
    QString QS_passwd = ui->password->text();

    if(nullptr == QS_server_port){
        ui->server_port->setFocus();
        MessagesBox::warn(this, LOGIN_SERVER_NULL);
    } else if (nullptr == QS_username) {
        ui->username->setFocus();
        MessagesBox::warn(this, LOGIN_USERNAME_NULL);
    } else if (nullptr == QS_passwd) {
        ui->password->setFocus();
        MessagesBox::warn(this, LOGIN_PASSWORD_NULL);
    } else {
        // get server_port conn to server
        char server_port[SETTING_SERVER_IP_SIZE + SETTING_SERVER_PORT_SIZE]{};
        strcpy(server_port, QS_server_port.toLatin1().data());

        // get username and passwd
        strcpy(g_username, QS_username.toUtf8().data());
        strcpy(g_passwd, QS_passwd.toUtf8().data());

        // do login
        ConnectCtrl loginCtrl;
        loginCtrl.parserServerPort(server_port);
        MsgStatus status = loginCtrl.loginToServer();

        if (status == LoginSuccess) {
            accept();
        } else if(status == LoginPasswdError) {
            ui->password->clear();
            ui->password->setFocus();
            MessagesBox::warn(this, LOGIN_PASSWORD_ERROR);
        } else if(status == LoginUserNotExits) {
            ui->username->clear();
            ui->password->clear();
            ui->username->setFocus();
            MessagesBox::warn(this, LOGIN_USER_N_EXITS);
        } else if(status == LoginUndefinedError) {
            ui->username->clear();
            ui->password->clear();
            ui->username->setFocus();
            MessagesBox::warn(this, LOGIN_SERVER_ERROR);
        } else {
            ui->password->clear();
            ui->password->setFocus();
            MessagesBox::warn(this, LOGIN_SERVER_CON_ERROR);
        }
    }
}

void login::doLogout(){
    isLogin = false;
    //memset(g_passwd, 0, sizeof (g_passwd));
    accept();
}

void login::on_Login_clicked()
{
    if(isLogin){
        doLogout();
    }else{
        doLogin();
    }
}

void login::on_creteNewUser_clicked()
{
    newUser newuser(this);
    setWindowOpacity(0);
    if (newuser.exec() == QDialog::Accepted ) {
        accept();
    }
    setWindowOpacity(1);
}

void login::on_cancel_clicked()
{
    reject();
}

void login::on_chooseFontColor_clicked()
{
    setWindowOpacity(0);

    chooseColor color(this);
    color.exec();
    QColor c = color.getCurColor();

    if(c.isValid()){
        fontColor = c;
        ui->chooseFontColor->setStyleSheet("background-color:" + c.name()+ ";");
    }

    setWindowOpacity(1);
    refreshBackground();
}

void login::on_chooseIconColor_clicked()
{
    setWindowOpacity(0);

    chooseColor color(this);
    color.exec();
    QColor c = color.getCurColor();
    if(c.isValid()){
        iconColor = c;
        ui->chooseIconColor->setStyleSheet("background-color:" + c.name()+ ";");
    }

    setWindowOpacity(1);
    refreshBackground();
}

void login::on_transparent_sliderMoved(int position)
{
    if(position >=0 && position <=255){
        position -= 1;
        transparentPos = position > 0? position : 0;
    }
}

void login::on_radioButton_clicked(bool checked)
{
    isAutoStart = checked;
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
        fontSize = position;
    }
    refreshBackground();
}

void login::on_mainWinWidthSlider_sliderMoved(int position)
{
    if(position >= 280 && position <= 560){
        frameWidth = position;
    }
}

void login::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->buttons() & Qt::LeftButton){
        move(event->globalX()-m_nMouseClick_X_Coordinate, event->globalY()-m_nMouseClick_Y_Coordinate);
    }
}

void login::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
    {
        m_nMouseClick_X_Coordinate = event->x();
        m_nMouseClick_Y_Coordinate = event->y();
    }
}
