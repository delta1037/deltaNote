#include "login.h"
#include "ui_login.h"

extern char g_username[G_ARR_SIZE_USERNAME];
extern char g_passwd[G_ARR_SIZE_PASSWD];

extern char g_server[G_ARR_SIZE_SERVER];
extern int g_port;

extern bool isLogin;

extern QColor fontColor;
extern QColor iconColor;
extern int transparentPos;
extern bool cleanFlag;

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    ui->server_port->setPlaceholderText("server_port");
    string server_port = string(g_server) + ":" + to_string(g_port);
    ui->server_port->setText(QString(server_port.data()));
    ui->username->setPlaceholderText("username");
    //ui->username->setText(QString(g_username));
    ui->password->setPlaceholderText("password");
    ui->username->setFocus();

    ui->transparent->setValue(transparentPos);
    ui->chooseFontColor->setStyleSheet("background-color:" + fontColor.name()+ ";");
    ui->chooseIconColor->setStyleSheet("background-color:" + iconColor.name()+ ";");
}

login::~login()
{
    delete ui;
}

void login::on_Login_clicked()
{
    QString QS_server_port = ui->server_port->text().trimmed();
    QString QS_username = ui->username->text().trimmed();
    QString QS_passwd = ui->password->text();

    if(nullptr == QS_server_port){
        ui->server_port->setFocus();
        QMessageBox::warning(this, tr("Error"), tr("服务器不能为空!"), QMessageBox::Yes);
    } else if (nullptr == QS_username) {
        ui->username->setFocus();
        QMessageBox::warning(this, tr("Error"), tr("用户名不能为空!"), QMessageBox::Yes);
    } else if (nullptr == QS_passwd) {
        ui->password->setFocus();
        QMessageBox::warning(this, tr("Error"), tr("密码不能为空!"), QMessageBox::Yes);
    } else {
        // get server_port conn to server
        char server_port[G_ARR_SIZE_SERVER_PORT]{};
        strcpy(server_port, ui->server_port->text().trimmed().toUtf8().data());
        parserServerPort(server_port);

        ClientSqlite sqlite;
        sqlite.alterSetting("server", g_server);
        sqlite.alterSetting("port", to_string(g_port).data());

        // connect server
        SocketClient socketClient = SocketClient(g_server, g_port);
        if(SocketError == socketClient.getSocketOpState()){
            QMessageBox::warning(this, tr("Error"), tr("网络连接失败!"), QMessageBox::Yes);
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
                    QMessageBox::warning(this, tr("Error"), tr("服务端清除数据错误!"), QMessageBox::Yes);
                    LOG_ERROR("clean data error")
                }
            }

            // send local change to server
            synLocalChange();

            accept();
        } else if(recv.msgState == LoginPasswdError) {
            ui->password->clear();
            ui->password->setFocus();
            QMessageBox::warning(this, tr("Error"), tr("用户名或密码错误!"), QMessageBox::Yes);
        } else if(recv.msgState == LoginUserNotExits) {
            ui->username->clear();
            ui->password->clear();
            ui->username->setFocus();
            QMessageBox::warning(this, tr("Error"), tr("用户不存在!"), QMessageBox::Yes);
        } else if(recv.msgState == LoginUndefinedError) {
            ui->username->clear();
            ui->password->clear();
            ui->username->setFocus();
            QMessageBox::warning(this, tr("Error"), tr("服务器错误!"), QMessageBox::Yes);
        } else {
            ui->password->clear();
            ui->password->setFocus();
            QMessageBox::warning(this, tr("Error"), tr("网络连接失败!"), QMessageBox::Yes);
        }
    }

}

void login::on_creteNewUser_clicked()
{
    newUser newuser(this);
    if (newuser.exec() == QDialog::Accepted ) {
        accept();
    }
}

void login::on_cancel_clicked()
{
    reject();
}

void login::on_chooseFontColor_clicked()
{
    QColorDialog color;
    color.setCurrentColor(fontColor);
    color.show();
    QColor c = color.getColor();

    if(c.isValid()){
        ui->chooseFontColor->setStyleSheet("background-color:" + c.name()+ ";");
        ClientSqlite sqlite;
        sqlite.alterSetting("fontColor", c.name().toUtf8().data());
        fontColor = c;
    }
}

void login::on_chooseIconColor_clicked()
{
    QColorDialog color;
    color.setCurrentColor(iconColor);
    color.show();
    QColor c = color.getColor();
    if(c.isValid()){
        iconColor = c;
        ui->chooseIconColor->setStyleSheet("background-color:" + c.name()+ ";");
        ClientSqlite sqlite;
        sqlite.alterSetting("iconColor", c.name().toUtf8().data());
    }
}

void login::on_transparent_sliderMoved(int position)
{
    if(position >=0 && position <=255){
        transparentPos = position;
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
