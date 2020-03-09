#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QSystemTrayIcon>
#include <QListWidgetItem>
#include<QDesktopServices>
#include<QUrl>
#include <QMenu>
#include <QListWidget>
#include <QCloseEvent>

#include "login.h"
#include "todolistitem.h"
#include "sqlite.h"

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

public:
    void closeEvent(QCloseEvent *e);

private slots:
    void on_setting_clicked();

    void on_refresh_clicked();

    void on_lock_clicked();

    void on_ToDoListWin_customContextMenuRequested(const QPoint &pos);

    void openOfficialSite();

    void deleteTodoClicked();
    void addTodoClicked();

    void clearDone();

    void on_history_clicked();

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
    QAction *clearAction;

    // 唯一的空的TODO标识符
    bool  haveNullTodo;
    void addNewTodo();
    void loadList(std::vector<MsgOpPack> packList);
    bool isShowHistory;

    long long refreshTime;
};



#endif // MAINWINDOW_H
