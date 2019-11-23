/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actAdd;
    QAction *actDel;
    QAction *actInsert;
    QAction *actClear;
    QWidget *mainWindow;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *settingLayer;
    QPushButton *setting;
    QPushButton *refresh;
    QPushButton *lock;
    QListWidget *ToDoListWin;
    QHBoxLayout *opLayer;
    QPushButton *add;
    QPushButton *del;
    QPushButton *insert;
    QPushButton *clear;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(280, 471);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(10);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(280, 0));
        MainWindow->setMaximumSize(QSize(280, 16777215));
        MainWindow->setMouseTracking(false);
        MainWindow->setWindowOpacity(1.000000000000000);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        MainWindow->setTabShape(QTabWidget::Rounded);
        MainWindow->setDockNestingEnabled(false);
        actAdd = new QAction(MainWindow);
        actAdd->setObjectName(QString::fromUtf8("actAdd"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/add.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actAdd->setIcon(icon);
        actDel = new QAction(MainWindow);
        actDel->setObjectName(QString::fromUtf8("actDel"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resource/delete.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actDel->setIcon(icon1);
        actInsert = new QAction(MainWindow);
        actInsert->setObjectName(QString::fromUtf8("actInsert"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resource/insert.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actInsert->setIcon(icon2);
        actClear = new QAction(MainWindow);
        actClear->setObjectName(QString::fromUtf8("actClear"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resource/clear.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actClear->setIcon(icon3);
        mainWindow = new QWidget(MainWindow);
        mainWindow->setObjectName(QString::fromUtf8("mainWindow"));
        mainWindow->setMinimumSize(QSize(280, 0));
        mainWindow->setMaximumSize(QSize(280, 16777215));
        mainWindow->setMouseTracking(true);
        mainWindow->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(mainWindow);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 15, 0, 8);
        settingLayer = new QHBoxLayout();
        settingLayer->setSpacing(0);
        settingLayer->setObjectName(QString::fromUtf8("settingLayer"));
        settingLayer->setContentsMargins(-1, 0, -1, -1);
        setting = new QPushButton(mainWindow);
        setting->setObjectName(QString::fromUtf8("setting"));
        setting->setStyleSheet(QString::fromUtf8(""));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/resource/setting.svg"), QSize(), QIcon::Normal, QIcon::On);
        setting->setIcon(icon4);
        setting->setIconSize(QSize(32, 32));
        setting->setFlat(true);

        settingLayer->addWidget(setting);

        refresh = new QPushButton(mainWindow);
        refresh->setObjectName(QString::fromUtf8("refresh"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/resource/refresh.svg"), QSize(), QIcon::Normal, QIcon::On);
        refresh->setIcon(icon5);
        refresh->setIconSize(QSize(32, 32));
        refresh->setFlat(true);

        settingLayer->addWidget(refresh);

        lock = new QPushButton(mainWindow);
        lock->setObjectName(QString::fromUtf8("lock"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/resource/lock.svg"), QSize(), QIcon::Normal, QIcon::Off);
        icon6.addFile(QString::fromUtf8(":/resource/locked.svg"), QSize(), QIcon::Normal, QIcon::On);
        icon6.addFile(QString::fromUtf8(":/resource/lock.svg"), QSize(), QIcon::Disabled, QIcon::Off);
        icon6.addFile(QString::fromUtf8(":/resource/locked.svg"), QSize(), QIcon::Disabled, QIcon::On);
        icon6.addFile(QString::fromUtf8(":/resource/lock.svg"), QSize(), QIcon::Active, QIcon::Off);
        icon6.addFile(QString::fromUtf8(":/resource/locked.svg"), QSize(), QIcon::Active, QIcon::On);
        icon6.addFile(QString::fromUtf8(":/resource/lock.svg"), QSize(), QIcon::Selected, QIcon::Off);
        icon6.addFile(QString::fromUtf8(":/resource/locked.svg"), QSize(), QIcon::Selected, QIcon::On);
        lock->setIcon(icon6);
        lock->setIconSize(QSize(32, 32));
        lock->setFlat(true);

        settingLayer->addWidget(lock);


        verticalLayout->addLayout(settingLayer);

        ToDoListWin = new QListWidget(mainWindow);
        new QListWidgetItem(ToDoListWin);
        ToDoListWin->setObjectName(QString::fromUtf8("ToDoListWin"));
        ToDoListWin->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ToDoListWin->sizePolicy().hasHeightForWidth());
        ToDoListWin->setSizePolicy(sizePolicy1);
        ToDoListWin->setMinimumSize(QSize(280, 280));
        ToDoListWin->setMaximumSize(QSize(280, 800));
        ToDoListWin->setSizeIncrement(QSize(0, 520));
        ToDoListWin->setBaseSize(QSize(0, 520));
        ToDoListWin->setMouseTracking(false);
        ToDoListWin->setContextMenuPolicy(Qt::CustomContextMenu);
        ToDoListWin->setLayoutDirection(Qt::LeftToRight);
        ToDoListWin->setAutoFillBackground(true);
        ToDoListWin->setFrameShape(QFrame::NoFrame);
        ToDoListWin->setFrameShadow(QFrame::Plain);
        ToDoListWin->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        ToDoListWin->setEditTriggers(QAbstractItemView::CurrentChanged|QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);
        ToDoListWin->setDragEnabled(true);
        ToDoListWin->setDragDropOverwriteMode(false);
        ToDoListWin->setDragDropMode(QAbstractItemView::DragDrop);
        ToDoListWin->setDefaultDropAction(Qt::MoveAction);
        ToDoListWin->setIconSize(QSize(8, 8));
        ToDoListWin->setTextElideMode(Qt::ElideLeft);
        ToDoListWin->setProperty("isWrapping", QVariant(false));
        ToDoListWin->setResizeMode(QListView::Adjust);
        ToDoListWin->setLayoutMode(QListView::SinglePass);
        ToDoListWin->setSpacing(7);
        ToDoListWin->setViewMode(QListView::ListMode);
        ToDoListWin->setUniformItemSizes(true);

        verticalLayout->addWidget(ToDoListWin);

        opLayer = new QHBoxLayout();
        opLayer->setSpacing(0);
        opLayer->setObjectName(QString::fromUtf8("opLayer"));
        opLayer->setSizeConstraint(QLayout::SetDefaultConstraint);
        add = new QPushButton(mainWindow);
        add->setObjectName(QString::fromUtf8("add"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/resource/add.svg"), QSize(), QIcon::Normal, QIcon::On);
        add->setIcon(icon7);
        add->setIconSize(QSize(32, 32));
        add->setFlat(true);

        opLayer->addWidget(add);

        del = new QPushButton(mainWindow);
        del->setObjectName(QString::fromUtf8("del"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/resource/delete.svg"), QSize(), QIcon::Normal, QIcon::On);
        del->setIcon(icon8);
        del->setIconSize(QSize(32, 32));
        del->setFlat(true);

        opLayer->addWidget(del);

        insert = new QPushButton(mainWindow);
        insert->setObjectName(QString::fromUtf8("insert"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/resource/insert.svg"), QSize(), QIcon::Normal, QIcon::On);
        insert->setIcon(icon9);
        insert->setIconSize(QSize(32, 32));
        insert->setFlat(true);

        opLayer->addWidget(insert);

        clear = new QPushButton(mainWindow);
        clear->setObjectName(QString::fromUtf8("clear"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/resource/clear.svg"), QSize(), QIcon::Normal, QIcon::On);
        clear->setIcon(icon10);
        clear->setIconSize(QSize(32, 32));
        clear->setFlat(true);

        opLayer->addWidget(clear);


        verticalLayout->addLayout(opLayer);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 20);
        verticalLayout->setStretch(2, 1);
        MainWindow->setCentralWidget(mainWindow);

        retranslateUi(MainWindow);
        QObject::connect(clear, SIGNAL(clicked()), actClear, SLOT(trigger()));
        QObject::connect(add, SIGNAL(clicked()), actAdd, SLOT(trigger()));
        QObject::connect(del, SIGNAL(clicked()), actDel, SLOT(trigger()));
        QObject::connect(insert, SIGNAL(clicked()), actInsert, SLOT(trigger()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "TODO", nullptr));
        actAdd->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
#if QT_CONFIG(tooltip)
        actAdd->setToolTip(QCoreApplication::translate("MainWindow", "add new todo", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actAdd->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actDel->setText(QCoreApplication::translate("MainWindow", "Del", nullptr));
#if QT_CONFIG(tooltip)
        actDel->setToolTip(QCoreApplication::translate("MainWindow", "delete a todo", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actDel->setShortcut(QCoreApplication::translate("MainWindow", "Del", nullptr));
#endif // QT_CONFIG(shortcut)
        actInsert->setText(QCoreApplication::translate("MainWindow", "Insert", nullptr));
#if QT_CONFIG(tooltip)
        actInsert->setToolTip(QCoreApplication::translate("MainWindow", "insert a todo", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actInsert->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+I", nullptr));
#endif // QT_CONFIG(shortcut)
        actClear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
#if QT_CONFIG(tooltip)
        actClear->setToolTip(QCoreApplication::translate("MainWindow", "clear all todo", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actClear->setShortcut(QCoreApplication::translate("MainWindow", "Shift+Del", nullptr));
#endif // QT_CONFIG(shortcut)
        setting->setText(QString());
        refresh->setText(QString());
        lock->setText(QString());

        const bool __sortingEnabled = ToDoListWin->isSortingEnabled();
        ToDoListWin->setSortingEnabled(false);
        ToDoListWin->setSortingEnabled(__sortingEnabled);

        add->setText(QString());
        del->setText(QString());
        insert->setText(QString());
        clear->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
