/********************************************************************************
** Form generated from reading UI file 'notify.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
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
        notify->resize(220, 88);
        notify->setMinimumSize(QSize(162, 88));
        notify->setMaximumSize(QSize(220, 88));
        notify->setCursor(QCursor(Qt::SizeAllCursor));
        message = new QLineEdit(notify);
        message->setObjectName(QString::fromUtf8("message"));
        message->setGeometry(QRect(0, 10, 220, 31));
        message->setMinimumSize(QSize(220, 31));
        message->setMaximumSize(QSize(220, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("JetBrains Mono"));
        font.setPointSize(8);
        message->setFont(font);
        message->setCursor(QCursor(Qt::ArrowCursor));
        message->setMouseTracking(false);
        message->setAcceptDrops(false);
        message->setLayoutDirection(Qt::LeftToRight);
        message->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 0);"));
        message->setFrame(false);
        message->setAlignment(Qt::AlignCenter);
        message->setReadOnly(true);
        OK = new QPushButton(notify);
        OK->setObjectName(QString::fromUtf8("OK"));
        OK->setGeometry(QRect(80, 50, 61, 31));
        OK->setMinimumSize(QSize(61, 31));
        OK->setMaximumSize(QSize(61, 31));
        OK->setFont(font);
        OK->setCursor(QCursor(Qt::PointingHandCursor));
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
