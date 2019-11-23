#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QSystemTrayIcon>
#include <QListWidgetItem>
#include<QDesktopServices>
#include<QUrl>
#include <QMenu>

#include "login.h"
#include "userinfo.h"
#include "todolistitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void refreshBackground();
    ~MainWindow();

private slots:
    void on_setting_clicked();

    void on_refresh_clicked();

    void on_lock_clicked();

    void on_ToDoListWin_customContextMenuRequested(const QPoint &pos);

    void on_actAdd_triggered();

    void on_actDel_triggered();

    void on_actInsert_triggered();

    void on_actClear_triggered();

    void on_traySetting_clicked();
    void on_openOfficialSite_triggered();

private:
    void paintEvent(QPaintEvent *event = nullptr);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent*event);
    void detectEdge();
    int edgeMargin;     //鼠标检测的边缘距离
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
    enum {
        nodir = 0x01,
        bottom = 0x02
    }resizeDir; //更改尺寸的方向

    Ui::MainWindow *ui;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *officialAction;
    QAction *settingAction;
    QAction *quitAction;
};

#endif // MAINWINDOW_H
