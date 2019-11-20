#include "userinfo.h"
#include "ui_userinfo.h"
#include "log.h"

extern char g_server[16];
extern int g_port;

extern char g_username[8];
extern char g_passdw[12];

extern bool isLogin;
extern bool isLocked;

userInfo::userInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userInfo)
{
    ui->setupUi(this);

    std::cout << g_username << "\n";
    ui->username->setText(QString(QLatin1String(g_username)));
}

userInfo::~userInfo()
{
    delete ui;
}

void userInfo::on_logout_clicked()
{
    isLogin = false;
    memset(g_username, 0, sizeof (g_username));
    memset(g_passdw, 0, sizeof (g_passdw));
    accept();
}

void userInfo::on_cancel_clicked()
{
    reject();
}
