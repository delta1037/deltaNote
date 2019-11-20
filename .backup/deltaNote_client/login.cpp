#include "login.h"
#include "ui_login.h"
#include "newuser.h"

#include <iostream>

using namespace deltaNote;

extern char g_server[16];
extern int g_port;

extern char g_username[8];
extern char g_passdw[12];

extern bool isLogin;

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    ui->server_port->setPlaceholderText("server_port");
    ui->server_port->setText("127.0.0.1:8888");
    ui->username->setPlaceholderText("username");
    ui->username->setText("admin");
    ui->password->setPlaceholderText("password");
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
        QMessageBox::warning(this, tr("Warning"),
                             tr("server & port is null!"),
                             QMessageBox::Yes);
    } else if (nullptr == QS_username) {
        ui->username->setFocus();
        QMessageBox::warning(this, tr("Warning"),
                             tr("username is null!"),
                             QMessageBox::Yes);
    } else if (nullptr == QS_passwd) {
        ui->password->setFocus();
        QMessageBox::warning(this, tr("Warning"),
                             tr("password is null!"),
                             QMessageBox::Yes);
    } else {
        // get server_port conn to server
        char *server_port = ui->server_port->text().trimmed().toLatin1().data();
        bool isIP = true;
        char server[16] = {0};
        int port = 0;
        for(int i = 0; server_port[i] != '\0'; ++i){
            if(server_port[i] == ':'){
                isIP = false;
                server[i] = '\0';
                continue;
            }

            if(isIP){
                server[i] = server_port[i];
            } else {
                port = port * 10 + server_port[i] - '0';
            }
        }

        // store log info
        strcpy(g_server, server);
        g_port = port;

        // connect server
        SocketClient socketClient = SocketClient(server, port);

        // get username and passwd
        MSG send{};
        send.msgOp = Login;
        strcpy(send.userName, ui->username->text().trimmed().toLatin1().data());
        strcpy(send.passwd, ui->password->text().toLatin1().data());

        ret = socketClient.sendMsg(&send, sizeof(send));

        MSG recv{};
        ret = socketClient.recvMsg(&recv, sizeof(recv));

        if (recv.msgState == LoginSuccess) {
            isLogin = true;
            strcpy(g_username, send.userName);
            strcpy(g_passdw, send.passwd);
            accept();
        } else if(recv.msgState == LoginPasswdError) {
            ui->password->clear();
            ui->password->setFocus();
            QMessageBox::warning(this, tr("Warning"),
                                 tr("user name or password error!"),
                                 QMessageBox::Yes);

        } else if(recv.msgState == LoginUserNotExits) {
            ui->username->clear();
            ui->password->clear();
            ui->username->setFocus();
            QMessageBox::warning(this, tr("Warning"),
                                 tr("user is not exists!"),
                                 QMessageBox::Yes);
        } else if(recv.msgState == LoginUndefinedError) {
            ui->username->clear();
            ui->password->clear();
            ui->username->setFocus();
            QMessageBox::warning(this, tr("Warning"),
                                 tr("undefined error!"),
                                 QMessageBox::Yes);
        } else {
            ui->password->clear();
            ui->password->setFocus();
            QMessageBox::warning(this, tr("Error"),
                                 tr("packet error!"),
                                 QMessageBox::Yes);
        }
    }

}

void login::on_creteNewUser_clicked()
{
    QDialog *newuser = new newUser();
    if (newuser->exec() == QDialog::Accepted ) {
        accept();
    }
}

void login::on_cancel_clicked()
{
    reject();
}
