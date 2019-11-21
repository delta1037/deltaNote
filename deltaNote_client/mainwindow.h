#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QListWidgetItem>
#include <QGraphicsSvgItem>
#include <QSharedPointer>
#include <QGraphicsColorizeEffect>
#include <QMenu>
#include <QFile>

#include "login.h"
#include "userinfo.h"
#include "todolistitem.h"
#include "graphicscolorsvgitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
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

private:
    void paintEvent(QPaintEvent *event = nullptr);

    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
