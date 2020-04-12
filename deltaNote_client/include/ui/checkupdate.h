#ifndef CHECKUPDATE_H
#define CHECKUPDATE_H

#include <QDialog>
#include <QFile>
#include <QDir>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QMouseEvent>

#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>

#include <QString>

#include "common/untils.h"
#include "log/logCtrl.h"
#include "ui/notify.h"

namespace Ui {
class CheckUpdate;
}

class CheckUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit CheckUpdate(QWidget *parent = nullptr);
    ~CheckUpdate();

    void setUpdateLink(QString &);
    void setUpdateMessage(QString &);

    void getUpdateFile();
    bool createNewFile();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void updateDownloadProgress(qint64, qint64);

    void fileDowload();

    void fileDowloadFinished();

    void on_cancel_clicked();

    void on_abort_clicked();

    void on_yes_clicked();
    void processError(QNetworkReply::NetworkError);
private:
    Ui::CheckUpdate *ui;

    bool downloadFinish;
    bool downloadAbort;
    QString updateLink;

    QFile *updateFile;//文件指针
    QNetworkReply *updateReply;
    QNetworkAccessManager *updateManager;

    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
};

#endif // CHECKUPDATE_H
