#ifndef USERINFO_H
#define USERINFO_H

#include <QDialog>
#include <QSettings>
#include <QDir>
#include <QColorDialog>
#include "untils.h"

namespace Ui {
class userInfo;
}

class userInfo : public QDialog
{
    Q_OBJECT

public:
    explicit userInfo(QWidget *parent = nullptr);
    ~userInfo();

private slots:
    void on_logout_clicked();

    void on_cancel_clicked();

    void on_transparent_sliderMoved(int position);

    void on_chooseFontColor_clicked();

    void on_chooseIconColor_clicked();

    void on_clearData_clicked();

    void on_radioButton_clicked(bool checked);

private:
    Ui::userInfo *ui;
};

#endif // USERINFO_H
