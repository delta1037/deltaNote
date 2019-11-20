#ifndef USERINFO_H
#define USERINFO_H

#include <QDialog>

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

private:
    Ui::userInfo *ui;
};

#endif // USERINFO_H
