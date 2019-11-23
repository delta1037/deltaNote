#include "userinfo.h"
#include "ui_userinfo.h"

extern char g_username[G_ARR_SIZE_USERNAME];
extern char g_passdw[G_ARR_SIZE_PASSWD];

extern bool isLogin;

extern QColor fontColor;
extern QColor iconColor;
extern int transparentPos;
extern bool cleanFlag;

userInfo::userInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userInfo)
{
    ui->setupUi(this);

    ui->username->setText(QString(QLatin1String(g_username)));

    ui->transparent->setValue(transparentPos);
    ui->chooseFontColor->setStyleSheet("background-color:" + fontColor.name()+ ";");
    ui->chooseIconColor->setStyleSheet("background-color:" + iconColor.name()+ ";");
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

void userInfo::on_transparent_sliderMoved(int position)
{
    if(position >=0 && position <=255){
        transparentPos = position;
    }
}

void userInfo::on_chooseFontColor_clicked()
{
    QColorDialog color;
    QColor c = color.getColor();
    if(c.isValid()){
        fontColor = c;
        ui->chooseFontColor->setStyleSheet("background-color:" + c.name()+ ";");
    }else{
        QMessageBox::warning(this, tr("Error"), tr("color error!"), QMessageBox::Yes);
    }
}

void userInfo::on_chooseIconColor_clicked()
{
    QColorDialog color;
    QColor c = color.getColor();
    if(c.isValid()){
        iconColor = c;
        ui->chooseIconColor->setStyleSheet("background-color:" + c.name()+ ";");
    }else{
        QMessageBox::warning(this, tr("Error"), tr("color error!"), QMessageBox::Yes);
    }
}

void userInfo::on_clearData_clicked()
{
    // clear all data
    ClientSqlite sqlite;
    sqlite.cleanDatasetTable();
    sqlite.cleanChangeTable();

    cleanFlag = true;
}
