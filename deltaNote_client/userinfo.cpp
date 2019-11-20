#include "userinfo.h"
#include "ui_userinfo.h"

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
