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

    void on_openOfficialSite_triggered();

private:
    void paintEvent(QPaintEvent *event = nullptr);

    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *officialAction;
    QAction *settingAction;
    QAction *quitAction;
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void moveEvent(QMoveEvent *event) override;
};

#endif // MAINWINDOW_H
