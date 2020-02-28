#include "login.h"
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
    ui->chooseFontColor->setStyleSheet("background-color:" + fontColor.name()+ ";");
    ui->chooseIconColor->setStyleSheet("background-color:" + iconColor.name()+ ";");

    ui->radioButton->setChecked(isAutoStart);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("background-color:transparent");

    //设置无边框和设置隐藏下部图标
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::SubWindow);

    // 将界面移到中心
    move(desktop.width()/2 - this->width()/2, desktop.height()/2 - this->height()/2);
    //LOG_INFO("x:%d, y:%d", desktop.width(), desktop.height())
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
    painter_bmp.drawRoundedRect(new_rect, 20, 20, Qt::AbsoluteSize);
    painter_bmp.drawRoundedRect(buttonRect, 20, 20, Qt::AbsoluteSize);
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
    palette.setColor(QPalette::Text, fontColor);
    ui->server_port->setPalette(palette);
    ui->username->setPalette(palette);
    ui->password->setPalette(palette);

    QPalette labelPal = ui->transparentLabel->palette();
    labelPal.setColor(QPalette::WindowText, fontColor);
    ui->transparentLabel->setPalette(labelPal);
    ui->fontColorLabel->setPalette(labelPal);
    ui->IconColorLabel->setPalette(labelPal);
    ui->clearDataLabel->setPalette(labelPal);
}

void login::doLogin(){
    QString QS_server_port = ui->server_port->text().trimmed();
    QString QS_username = ui->username->text().trimmed();
    QString QS_passwd = ui->password->text();

    if(nullptr == QS_server_port){
        ui->server_port->setFocus();
        MessagesBox::warn(this, "server & port is null!");
    } else if (nullptr == QS_username) {
        ui->username->setFocus();
        MessagesBox::warn(this, "username is null!");
    } else if (nullptr == QS_passwd) {
        ui->password->setFocus();
        MessagesBox::warn(this, "password is null!");
    } else {
        // get server_port conn to server
        char server_port[G_ARR_SIZE_SERVER_PORT]{};
        strcpy(server_port, ui->server_port->text().trimmed().toLatin1().data());
        parserServerPort(server_port);

        ClientSqlite sqlite;
        sqlite.alterSetting("server", g_server);
        sqlite.alterSetting("port", to_string(g_port).data());

        // connect server
        SocketClient socketClient = SocketClient(g_server, g_port);
        if(SocketError == socketClient.getSocketOpState()){
            MessagesBox::warn(this, "Socket connect error!");
            return;
        }

        // get username and passwd
        MSG_PACK send{};
        send.msgOp = Login;
        strcpy(send.userName, ui->username->text().trimmed().toUtf8().data());
        strcpy(send.passwd, ui->password->text().toUtf8().data());
        ret = socketClient.sendMsg(&send, sizeof(send));

        MSG_PACK recv{};
        ret = socketClient.recvMsg(&recv, sizeof(recv));

        if (recv.msgState == LoginSuccess) {
            isLogin = true;
            strcpy(g_username, send.userName);
            strcpy(g_passwd, send.passwd);
            sqlite.alterSetting("username", g_username);
            sqlite.alterSetting("passwd", g_passwd);

            if(cleanFlag){
                MSG_PACK synPack{};
                makeSocketPack(synPack, 1, MSG_FULL, Delete);

                if(CleanSuccess == synMsgToServer(synPack)){
                    cleanFlag = false;
                }else{
                    MessagesBox::warn(this, "server clean data error!");
                    LOG_ERROR("clean data error")
                }
            }

            // send local change to server
            synLocalChange();

            accept();
        } else if(recv.msgState == LoginPasswdError) {
            ui->password->clear();
            ui->password->setFocus();
            MessagesBox::warn(this, "user name or password error!");
        } else if(recv.msgState == LoginUserNotExits) {
            ui->username->clear();
            ui->password->clear();
            ui->username->setFocus();
            MessagesBox::warn(this, "user is not exists!");
        } else if(recv.msgState == LoginUndefinedError) {
            ui->username->clear();
            ui->password->clear();
            ui->username->setFocus();
            MessagesBox::warn(this, "server undefined error!");
        } else {
            ui->password->clear();
            ui->password->setFocus();
            MessagesBox::warn(this, "client undefined error!");
        }
    }
}

void login::doLogout(){
    isLogin = false;
    //memset(g_username, 0, sizeof (g_username));
    ClientSqlite sqlite;
    sqlite.alterSetting("passwd", "nullptr");
    memset(g_passwd, 0, sizeof (g_passwd));
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
        ui->chooseFontColor->setStyleSheet("background-color:" + c.name()+ ";");
        ClientSqlite sqlite;
        sqlite.alterSetting("fontColor", c.name().toUtf8().data());
        fontColor = c;
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
        ClientSqlite sqlite;
        sqlite.alterSetting("iconColor", c.name().toUtf8().data());
    }

    setWindowOpacity(1);
    refreshBackground();
}

void login::on_transparent_sliderMoved(int position)
{
    if(position >=0 && position <=255){
        position -= 5;
        transparentPos = position > 0? position : 0;
        ClientSqlite sqlite;
        sqlite.alterSetting("transparentPos", to_string(transparentPos).data());
    }
}

void login::on_clearData_clicked()
{
    // clear all data
    ClientSqlite sqlite;
    sqlite.cleanDatasetTable();
    sqlite.cleanChangeTable();
    cleanFlag = true;
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

    ClientSqlite sqlite;
    sqlite.alterSetting("isAutoStart", to_string(isAutoStart).data());
    //LOG_INFO("suto:%s", to_string(isAutoStart).data())
}

void login::on_exit_clicked()
{
    accept();
}
