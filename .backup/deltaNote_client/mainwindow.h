#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMenu>

#include "todolistitem.h"

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
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
