#include "newuser.h"
#include "ui_newuser.h"

newUser::newUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newUser)
{
    ui->setupUi(this);

    refreshBackground();

    ui->server_port->setPlaceholderText("server:port");
    string server_port = string(g_server) + ":" + to_string(g_port);
    ui->server_port->setText(QString(server_port.data()));
    ui->username->setPlaceholderText("username");
    ui->passwd->setPlaceholderText("password");
    ui->repasswd ->setPlaceholderText("rewrite password");
    ui->username->setFocus();

    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("background-color:transparent");

    //设置无边框和设置隐藏下部图标
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::SubWindow);
}

newUser::~newUser()
{
    delete ui;
}
void newUser::paintEvent(QPaintEvent *event)
{
    event->ignore();
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(255, 255, 255, 255 - transparentPos));

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
    painter.setPen(iconColor);
    QRect lineRect = new_rect;
    lineRect.setHeight(lineRect.height() - 1);
    lineRect.setWidth(lineRect.width() - 1);
    painter.drawRoundedRect(lineRect, 10, 10, Qt::AbsoluteSize);
}

void newUser::refreshBackground(){
    // change icon color
    GraphicsColorSvgItem svg_server(":/resource/server.svg");
    ui->S->setIcon(svg_server.setColor(iconColor));

    GraphicsColorSvgItem svg_username(":/resource/user.svg");
    ui->U->setIcon(svg_username.setColor(iconColor));

    GraphicsColorSvgItem svg_passwd(":/resource/passwd.svg");
    ui->P1->setIcon(svg_passwd.setColor(iconColor));
    ui->P2->setIcon(svg_passwd.setColor(iconColor));

    QPalette palette;
    palette.setColor(QPalette::Text, fontColor);
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
    QString QS_repasswd = ui->repasswd->text();

    if(nullptr == QS_server_port){
        ui->server_port->setFocus();
        MessagesBox::warn(this, "server & port is null!");
    } else if (nullptr == QS_username) {
        ui->username->setFocus();
        MessagesBox::warn(this, "username is null!");
    } else if (nullptr == QS_passwd) {
        ui->passwd->setFocus();
        MessagesBox::warn(this, "password is null!");
    } else if (nullptr == QS_repasswd) {
        ui->repasswd->setFocus();
        MessagesBox::warn(this, "password is null!");
    } else if (0 != QS_passwd.compare(QS_repasswd)) {
        ui->passwd->clear();
        ui->repasswd->clear();
        ui->passwd->setFocus();
        MessagesBox::warn(this, "password is inconpable!");
    } else {
        // read data and create user
        char server_port[G_ARR_SIZE_SERVER_PORT]{};
        strcpy(server_port, QS_server_port.toUtf8().data());
        parserServerPort(server_port);

        ClientSqlite sqlite;
        sqlite.alterSetting("server", g_server);
        sqlite.alterSetting("port", to_string(g_port).data());

        // connect server
        SocketClient socketClient = SocketClient(g_server, g_port);
        if(SocketError == socketClient.getSocketOpState()){
            MessagesBox::warn(this, "Socket connect error!");
        }

        // get username and passwd
        MSG_PACK send{};
        send.msgOp = CreateUser;
        strcpy(send.userName, QS_username.toUtf8().data());
        strcpy(send.passwd, QS_passwd.toUtf8().data());

        ret = socketClient.sendMsg(&send, sizeof(send));

        MSG_PACK recv{};
        ret = socketClient.recvMsg(&recv, sizeof(recv));

        if (recv.msgState == CreateUserSuccess) {
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
        } else if (recv.msgState == CreateUserUserExists) {
            ui->username->clear();
            ui->username->setFocus();
            MessagesBox::warn(this, "username is exists!");
        } else if (recv.msgState == CreateUserUndefinedError) {
            MessagesBox::warn(this, "server undefined error!");
        } else {
            MessagesBox::warn(this, "client undefined error!");
        }
    }
}

void newUser::on_cancel_clicked()
{
    reject();
}
