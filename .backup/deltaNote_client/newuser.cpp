#include "newuser.h"
#include "ui_newuser.h"

extern char g_server[16];
extern int g_port;

extern char g_username[8];
extern char g_passdw[12];

extern bool isLogin;

using namespace deltaNote;

newUser::newUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newUser)
{
    ui->setupUi(this);
    ui->server_port->setPlaceholderText("server:port");
    ui->username->setPlaceholderText("username");
    ui->passwd->setPlaceholderText("password");
    ui->repasswd ->setPlaceholderText("rewrite password");
    ui->server_port->setFocus();
}

newUser::~newUser()
{
    delete ui;
}

void newUser::on_ok_clicked()
{
    QString QS_server_port = ui->server_port->text().trimmed();
    QString QS_username = ui->username->text().trimmed();
    QString QS_passwd = ui->passwd->text();
    QString QS_repasswd = ui->repasswd->text();

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
        ui->passwd->setFocus();
        QMessageBox::warning(this, tr("Warning"),
                             tr("password is null!"),
                             QMessageBox::Yes);
    } else if (nullptr == QS_repasswd) {
        ui->repasswd->setFocus();
        QMessageBox::warning(this, tr("Warning"),
                             tr("password is null!"),
                             QMessageBox::Yes);
    } else if (0 != QS_passwd.compare(QS_repasswd)) {
        ui->passwd->clear();
        ui->repasswd->clear();
        ui->passwd->setFocus();
        QMessageBox::warning(this, tr("Warning"),
                             tr("password is inconpable!"),
                             QMessageBox::Yes);
    } else {
        // read data and create user
        char *server_port = QS_server_port.toLatin1().data();
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
        send.msgOp = CreateUser;
        strcpy(send.userName, QS_username.toLatin1().data());
        strcpy(send.passwd, QS_passwd.toLatin1().data());

        ret = socketClient.sendMsg(&send, sizeof(send));

        MSG recv{};
        ret = socketClient.recvMsg(&recv, sizeof(recv));

        if (recv.msgState == CreateUserSuccess) {
            isLogin = true;
            strcpy(g_username, send.userName);
            strcpy(g_passdw, send.passwd);
            accept();
        } else if (recv.msgState == CreateUserUserExists) {
            ui->username->clear();
            ui->username->setFocus();
            QMessageBox::warning(this, tr("Warning"),
                                 tr("username is exists!"),
                                 QMessageBox::Yes);
        } else if (recv.msgState == CreateUserUndefinedError) {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("server undefined error!"),
                                 QMessageBox::Yes);
        } else {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("client undefined error!"),
                                 QMessageBox::Yes);
        }
    }
}

void newUser::on_cancel_clicked()
{
    reject();
}
