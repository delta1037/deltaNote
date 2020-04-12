#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>

#include <QFile>

#include <QProcess>
#include <QTimer>
#include <QFileInfo>

#include "untils.h"

#include "log/logCtrl.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;

    QString srcPath;
    QString srcFile ;

    QString desPath;
    QString desFile;
    QString backFile;

    int startFailCount;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private slots:
    void on_ok_clicked();

    void doUpdate();

    void updateFail();

};

#endif // MAINWINDOW_H
