/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *mainWindow;
    QPushButton *lock;
    QPushButton *refresh;
    QPushButton *setting;
    QListWidget *listWidget;
    QScrollBar *verticalScrollBar;
    QMenuBar *menuBar;
    QMenu *menudelta;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(274, 448);
        mainWindow = new QWidget(MainWindow);
        mainWindow->setObjectName(QString::fromUtf8("mainWindow"));
        lock = new QPushButton(mainWindow);
        lock->setObjectName(QString::fromUtf8("lock"));
        lock->setGeometry(QRect(200, 0, 41, 41));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/lock.svg"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/resource/locked.svg"), QSize(), QIcon::Normal, QIcon::On);
        icon.addFile(QString::fromUtf8(":/resource/lock.svg"), QSize(), QIcon::Disabled, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/resource/locked.svg"), QSize(), QIcon::Disabled, QIcon::On);
        icon.addFile(QString::fromUtf8(":/resource/lock.svg"), QSize(), QIcon::Active, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/resource/locked.svg"), QSize(), QIcon::Active, QIcon::On);
        icon.addFile(QString::fromUtf8(":/resource/lock.svg"), QSize(), QIcon::Selected, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/resource/locked.svg"), QSize(), QIcon::Selected, QIcon::On);
        lock->setIcon(icon);
        lock->setIconSize(QSize(32, 32));
        lock->setFlat(true);
        refresh = new QPushButton(mainWindow);
        refresh->setObjectName(QString::fromUtf8("refresh"));
        refresh->setGeometry(QRect(110, 0, 41, 41));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resource/refresh.svg"), QSize(), QIcon::Normal, QIcon::On);
        refresh->setIcon(icon1);
        refresh->setIconSize(QSize(32, 32));
        refresh->setFlat(true);
        setting = new QPushButton(mainWindow);
        setting->setObjectName(QString::fromUtf8("setting"));
        setting->setGeometry(QRect(20, 0, 41, 41));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resource/setting.svg"), QSize(), QIcon::Normal, QIcon::On);
        setting->setIcon(icon2);
        setting->setIconSize(QSize(32, 32));
        setting->setFlat(true);
        listWidget = new QListWidget(mainWindow);
        new QListWidgetItem(listWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(10, 40, 256, 361));
        listWidget->setFrameShadow(QFrame::Raised);
        verticalScrollBar = new QScrollBar(mainWindow);
        verticalScrollBar->setObjectName(QString::fromUtf8("verticalScrollBar"));
        verticalScrollBar->setEnabled(true);
        verticalScrollBar->setGeometry(QRect(260, 50, 16, 351));
        verticalScrollBar->setOrientation(Qt::Vertical);
        verticalScrollBar->setInvertedAppearance(false);
        verticalScrollBar->setInvertedControls(true);
        MainWindow->setCentralWidget(mainWindow);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 274, 25));
        menudelta = new QMenu(menuBar);
        menudelta->setObjectName(QString::fromUtf8("menudelta"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menudelta->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lock->setText(QString());
        refresh->setText(QString());
        setting->setText(QString());

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("MainWindow", "New Item", nullptr));
        listWidget->setSortingEnabled(__sortingEnabled);

        menudelta->setTitle(QCoreApplication::translate("MainWindow", "ToDo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
