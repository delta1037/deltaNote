/********************************************************************************
** Form generated from reading UI file 'notify.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOTIFY_H
#define UI_NOTIFY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_notify
{
public:
    QLineEdit *message;
    QPushButton *OK;

    void setupUi(QDialog *notify)
    {
        if (notify->objectName().isEmpty())
            notify->setObjectName(QString::fromUtf8("notify"));
        notify->resize(162, 88);
        notify->setMinimumSize(QSize(162, 88));
        notify->setMaximumSize(QSize(162, 88));
        message = new QLineEdit(notify);
        message->setObjectName(QString::fromUtf8("message"));
        message->setGeometry(QRect(10, 9, 141, 31));
        message->setMinimumSize(QSize(141, 31));
        message->setMaximumSize(QSize(141, 31));
        message->setCursor(QCursor(Qt::ArrowCursor));
        message->setMouseTracking(false);
        message->setAcceptDrops(false);
        message->setLayoutDirection(Qt::LeftToRight);
        message->setFrame(false);
        message->setAlignment(Qt::AlignCenter);
        message->setReadOnly(true);
        OK = new QPushButton(notify);
        OK->setObjectName(QString::fromUtf8("OK"));
        OK->setGeometry(QRect(50, 50, 61, 31));
        OK->setMinimumSize(QSize(61, 31));
        OK->setMaximumSize(QSize(61, 31));
        OK->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));
        OK->setFlat(false);

        retranslateUi(notify);

        QMetaObject::connectSlotsByName(notify);
    } // setupUi

    void retranslateUi(QDialog *notify)
    {
        notify->setWindowTitle(QCoreApplication::translate("notify", "Dialog", nullptr));
        OK->setText(QCoreApplication::translate("notify", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class notify: public Ui_notify {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NOTIFY_H
