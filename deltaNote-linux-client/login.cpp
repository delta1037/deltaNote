#include "login.h"
#include "ui_login.h"

extern char g_username[G_ARR_SIZE_USERNAME];
extern char g_passdw[G_ARR_SIZE_PASSWD];

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
    ui->server_port->setText("127.0.0.1:8888");
    ui->username->setPlaceholderText("username");
    ui->username->setText("admin");
    ui->password->setPlaceholderText("password");

    ui->transparent->setValue(transparentPos);
    //ui->chooseFontColor->setStyleSheet()
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
        QMessageBox::warning(this, tr("Error"), tr("server & port is null!"), QMessageBox::Yes);
    } else if (nullptr == QS_username) {
        ui->username->setFocus();
        QMessageBox::warning(this, tr("Error"), tr("username is null!"), QMessageBox::Yes);
    } else if (nullptr == QS_passwd) {
        ui->password->setFocus();
        QMessageBox::warning(this, tr("Error"), tr("password is null!"), QMessageBox::Yes);
    } else {
        // get server_port conn to server
        char *server_port = ui->server_port->text().trimmed().toLatin1().data();
        parserServerPort(server_port);

        // connect server
        SocketClient socketClient = SocketClient(g_server, g_port);
        if(SocketError == socketClient.getSocketOpState()){
            QMessageBox::warning(this, tr("Error"), tr("Socket connect error!"), QMessageBox::Yes);
        }

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

            if(cleanFlag){
                MSG synPack{};
                makeSocketPack(synPack, 1, MSG_FULL, Delete);

                if(CleanSuccess == synMsgToServer(synPack)){
                    cleanFlag = false;
                }else{
                    QMessageBox::warning(this, tr("Error"), tr("server clean data error!"), QMessageBox::Yes);
                    LOG_ERROR("clean data error")
                }
            }

            // send local change to server
            synLocalChange();

            accept();
        } else if(recv.msgState == LoginPasswdError) {
            ui->password->clear();
            ui->password->setFocus();
            QMessageBox::warning(this, tr("Error"), tr("user name or password error!"), QMessageBox::Yes);

        } else if(recv.msgState == LoginUserNotExits) {
            ui->username->clear();
            ui->password->clear();
            ui->username->setFocus();
            QMessageBox::warning(this, tr("Error"), tr("user is not exists!"), QMessageBox::Yes);
        } else if(recv.msgState == LoginUndefinedError) {
            ui->username->clear();
            ui->password->clear();
            ui->username->setFocus();
            QMessageBox::warning(this, tr("Error"), tr("server undefined error!"), QMessageBox::Yes);
        } else {
            ui->password->clear();
            ui->password->setFocus();
            QMessageBox::warning(this, tr("Error"), tr("client undefined error!"), QMessageBox::Yes);
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

void login::on_chooseFontColor_clicked()
{
    QColorDialog color;
    color.setCurrentColor(fontColor);
    QColor c = color.getColor();

    if(c.isValid()){
        ui->chooseFontColor->setStyleSheet("background-color:" + c.name()+ ";");
        fontColor = c;
    } else {
        QMessageBox::warning(this, tr("Error"), tr("color error!"), QMessageBox::Yes);
    }
}

void login::on_chooseIconColor_clicked()
{
    QColorDialog color;
    color.setCurrentColor(iconColor);
    QColor c = color.getColor();
    if(c.isValid()){
        iconColor = c;
        ui->chooseIconColor->setStyleSheet("background-color:" + c.name()+ ";");
    } else {
        QMessageBox::warning(this, tr("Error"), tr("color error!"), QMessageBox::Yes);
    }
}

void login::on_transparent_sliderMoved(int position)
{

    if(position >=0 && position <=255){
        transparentPos = position;
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
