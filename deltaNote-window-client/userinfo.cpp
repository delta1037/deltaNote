#include "userinfo.h"
#include "ui_userinfo.h"

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
    //memset(g_username, 0, sizeof (g_username));
    ClientSqlite sqlite;
    sqlite.alterSetting("passwd", "nullptr");
    memset(g_passwd, 0, sizeof (g_passwd));
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
        ClientSqlite sqlite;
        sqlite.alterSetting("transparentPos", to_string(transparentPos).data());
    }
}

void userInfo::on_chooseFontColor_clicked()
{
    QColorDialog color;
    color.show();
    QColor c = color.getColor();
    if(c.isValid()){
        fontColor = c;
        ui->chooseFontColor->setStyleSheet("background-color:" + c.name()+ ";");
        ClientSqlite sqlite;
        sqlite.alterSetting("fontColor", c.name().toUtf8().data());
    }
}

void userInfo::on_chooseIconColor_clicked()
{
    QColorDialog color;
    color.show();
    QColor c = color.getColor();
    if(c.isValid()){
        iconColor = c;
        ui->chooseIconColor->setStyleSheet("background-color:" + c.name()+ ";");
        ClientSqlite sqlite;
        sqlite.alterSetting("iconColor", c.name().toUtf8().data());
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

void userInfo::on_radioButton_clicked(bool checked)
{
    QSettings regedit(REGEDIT_AUTO_START_PATH, QSettings::NativeFormat);
    if(checked) {
        QString sAppPath = QApplication::applicationFilePath();
        regedit.setValue(REGEDIT_KEY, QVariant(QDir::toNativeSeparators(sAppPath)));
    } else {
        regedit.setValue(REGEDIT_KEY, QVariant());
    }
}
