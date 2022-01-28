/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "todolistwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *mainWindow;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *settingLayer;
    QPushButton *setting;
    QPushButton *refresh;
    QPushButton *lock;
    QPushButton *history;
    TodoListWidget *ToDoListWin;

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
        MainWindow->setMaximumSize(QSize(560, 16777215));
        MainWindow->setMouseTracking(false);
        MainWindow->setWindowOpacity(1.000000000000000);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        MainWindow->setTabShape(QTabWidget::Rounded);
        MainWindow->setDockNestingEnabled(false);
        mainWindow = new QWidget(MainWindow);
        mainWindow->setObjectName(QString::fromUtf8("mainWindow"));
        mainWindow->setMinimumSize(QSize(280, 0));
        mainWindow->setMaximumSize(QSize(280, 16777215));
        mainWindow->setMouseTracking(true);
        mainWindow->setFocusPolicy(Qt::NoFocus);
        mainWindow->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(mainWindow);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
        verticalLayout->setContentsMargins(0, 10, 0, 8);
        settingLayer = new QHBoxLayout();
        settingLayer->setSpacing(30);
        settingLayer->setObjectName(QString::fromUtf8("settingLayer"));
        settingLayer->setContentsMargins(15, 4, 15, 8);
        setting = new QPushButton(mainWindow);
        setting->setObjectName(QString::fromUtf8("setting"));
        setting->setMinimumSize(QSize(32, 32));
        setting->setMaximumSize(QSize(32, 32));
        setting->setStyleSheet(QString::fromUtf8("background-color:transparent"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/setting.svg"), QSize(), QIcon::Normal, QIcon::On);
        setting->setIcon(icon);
        setting->setIconSize(QSize(32, 32));
        setting->setFlat(true);

        settingLayer->addWidget(setting);

        refresh = new QPushButton(mainWindow);
        refresh->setObjectName(QString::fromUtf8("refresh"));
        refresh->setMinimumSize(QSize(32, 32));
        refresh->setMaximumSize(QSize(32, 32));
        refresh->setStyleSheet(QString::fromUtf8("background-color:transparent"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resource/refresh.svg"), QSize(), QIcon::Normal, QIcon::On);
        refresh->setIcon(icon1);
        refresh->setIconSize(QSize(32, 32));
        refresh->setFlat(true);

        settingLayer->addWidget(refresh);

        lock = new QPushButton(mainWindow);
        lock->setObjectName(QString::fromUtf8("lock"));
        lock->setMinimumSize(QSize(32, 32));
        lock->setMaximumSize(QSize(32, 32));
        lock->setStyleSheet(QString::fromUtf8("background-color:transparent"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resource/lock.svg"), QSize(), QIcon::Normal, QIcon::Off);
        icon2.addFile(QString::fromUtf8(":/resource/locked.svg"), QSize(), QIcon::Normal, QIcon::On);
        icon2.addFile(QString::fromUtf8(":/resource/lock.svg"), QSize(), QIcon::Disabled, QIcon::Off);
        icon2.addFile(QString::fromUtf8(":/resource/locked.svg"), QSize(), QIcon::Disabled, QIcon::On);
        icon2.addFile(QString::fromUtf8(":/resource/lock.svg"), QSize(), QIcon::Active, QIcon::Off);
        icon2.addFile(QString::fromUtf8(":/resource/locked.svg"), QSize(), QIcon::Active, QIcon::On);
        icon2.addFile(QString::fromUtf8(":/resource/lock.svg"), QSize(), QIcon::Selected, QIcon::Off);
        icon2.addFile(QString::fromUtf8(":/resource/locked.svg"), QSize(), QIcon::Selected, QIcon::On);
        lock->setIcon(icon2);
        lock->setIconSize(QSize(32, 32));
        lock->setFlat(true);

        settingLayer->addWidget(lock);

        history = new QPushButton(mainWindow);
        history->setObjectName(QString::fromUtf8("history"));
        history->setMinimumSize(QSize(32, 32));
        history->setMaximumSize(QSize(32, 32));
        history->setStyleSheet(QString::fromUtf8("background-color:transparent"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resource/history_close.svg"), QSize(), QIcon::Normal, QIcon::Off);
        icon3.addFile(QString::fromUtf8(":/resource/history_close.svg"), QSize(), QIcon::Normal, QIcon::On);
        history->setIcon(icon3);
        history->setIconSize(QSize(32, 32));
        history->setFlat(true);

        settingLayer->addWidget(history);


        verticalLayout->addLayout(settingLayer);

        ToDoListWin = new TodoListWidget(mainWindow);
        new QListWidgetItem(ToDoListWin);
        ToDoListWin->setObjectName(QString::fromUtf8("ToDoListWin"));
        ToDoListWin->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(ToDoListWin->sizePolicy().hasHeightForWidth());
        ToDoListWin->setSizePolicy(sizePolicy1);
        ToDoListWin->setMinimumSize(QSize(280, 0));
        ToDoListWin->setMaximumSize(QSize(560, 16777215));
        ToDoListWin->setSizeIncrement(QSize(0, -1));
        ToDoListWin->setBaseSize(QSize(0, -1));
        ToDoListWin->setMouseTracking(true);
        ToDoListWin->setFocusPolicy(Qt::WheelFocus);
        ToDoListWin->setContextMenuPolicy(Qt::CustomContextMenu);
        ToDoListWin->setLayoutDirection(Qt::LeftToRight);
        ToDoListWin->setAutoFillBackground(true);
        ToDoListWin->setStyleSheet(QString::fromUtf8("background-color: rgba(0, 0, 0, 0);"));
        ToDoListWin->setFrameShape(QFrame::NoFrame);
        ToDoListWin->setFrameShadow(QFrame::Raised);
        ToDoListWin->setLineWidth(0);
        ToDoListWin->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        ToDoListWin->setAutoScrollMargin(1);
        ToDoListWin->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ToDoListWin->setProperty("showDropIndicator", QVariant(false));
        ToDoListWin->setDragEnabled(false);
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

        verticalLayout->setStretch(1, 99);
        MainWindow->setCentralWidget(mainWindow);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "TODO", nullptr));
#if QT_CONFIG(whatsthis)
        setting->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
        setting->setText(QString());
#if QT_CONFIG(whatsthis)
        refresh->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
        refresh->setText(QString());
#if QT_CONFIG(whatsthis)
        lock->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
        lock->setText(QString());
#if QT_CONFIG(whatsthis)
        history->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
        history->setText(QString());

        const bool __sortingEnabled = ToDoListWin->isSortingEnabled();
        ToDoListWin->setSortingEnabled(false);
        ToDoListWin->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
