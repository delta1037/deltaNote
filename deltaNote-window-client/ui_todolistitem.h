/********************************************************************************
** Form generated from reading UI file 'todolistitem.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TODOLISTITEM_H
#define UI_TODOLISTITEM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ToDoListItem
{
public:
    QHBoxLayout *horizontalLayout;
    QPushButton *choose;
    QLineEdit *dataLine;

    void setupUi(QWidget *ToDoListItem)
    {
        if (ToDoListItem->objectName().isEmpty())
            ToDoListItem->setObjectName(QString::fromUtf8("ToDoListItem"));
        ToDoListItem->resize(266, 39);
        ToDoListItem->setMinimumSize(QSize(0, 39));
        ToDoListItem->setMaximumSize(QSize(16777215, 39));
        ToDoListItem->setContextMenuPolicy(Qt::NoContextMenu);
        ToDoListItem->setAcceptDrops(false);
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("lock");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        ToDoListItem->setWindowIcon(icon);
        ToDoListItem->setWindowOpacity(10.000000000000000);
        ToDoListItem->setStyleSheet(QString::fromUtf8("border-top-left-radius:10px;\n"
"border-bottom-left-radius:10px;"));
        horizontalLayout = new QHBoxLayout(ToDoListItem);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        choose = new QPushButton(ToDoListItem);
        choose->setObjectName(QString::fromUtf8("choose"));
        choose->setMinimumSize(QSize(36, 36));
        choose->setMaximumSize(QSize(36, 36));
        choose->setStyleSheet(QString::fromUtf8("border-top-left-radius:10px;\n"
"border-bottom-left-radius:10px;"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resource/unChoose.svg"), QSize(), QIcon::Active, QIcon::Off);
        icon1.addFile(QString::fromUtf8(":/resource/unChoose.svg"), QSize(), QIcon::Selected, QIcon::Off);
        icon1.addFile(QString::fromUtf8(":/resource/choose.svg"), QSize(), QIcon::Selected, QIcon::On);
        choose->setIcon(icon1);
        choose->setIconSize(QSize(17, 17));
        choose->setFlat(true);

        horizontalLayout->addWidget(choose);

        dataLine = new QLineEdit(ToDoListItem);
        dataLine->setObjectName(QString::fromUtf8("dataLine"));
        dataLine->setMinimumSize(QSize(211, 36));
        dataLine->setMaximumSize(QSize(16777215, 36));
        dataLine->setStyleSheet(QString::fromUtf8("border-color: rgb(0, 0, 0);"));
        dataLine->setFrame(false);
        dataLine->setDragEnabled(true);

        horizontalLayout->addWidget(dataLine);


        retranslateUi(ToDoListItem);

        QMetaObject::connectSlotsByName(ToDoListItem);
    } // setupUi

    void retranslateUi(QWidget *ToDoListItem)
    {
        ToDoListItem->setWindowTitle(QCoreApplication::translate("ToDoListItem", "Form", nullptr));
        choose->setText(QString());
        dataLine->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ToDoListItem: public Ui_ToDoListItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TODOLISTITEM_H
