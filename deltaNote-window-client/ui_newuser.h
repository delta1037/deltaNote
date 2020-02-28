/********************************************************************************
** Form generated from reading UI file 'newuser.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWUSER_H
#define UI_NEWUSER_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_newUser
{
public:
    QHBoxLayout *horizontalLayout_10;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *S;
    QLineEdit *server_port;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *U;
    QLineEdit *username;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *P1;
    QLineEdit *passwd;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *P2;
    QLineEdit *repasswd;
    QHBoxLayout *horizontalLayout;
    QPushButton *cancel;
    QPushButton *ok;

    void setupUi(QDialog *newUser)
    {
        if (newUser->objectName().isEmpty())
            newUser->setObjectName(QString::fromUtf8("newUser"));
        newUser->resize(280, 349);
        newUser->setMinimumSize(QSize(280, 0));
        newUser->setMaximumSize(QSize(280, 16777215));
        horizontalLayout_10 = new QHBoxLayout(newUser);
        horizontalLayout_10->setSpacing(40);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(20, 30, 20, 30);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, 10, -1);
        S = new QPushButton(newUser);
        S->setObjectName(QString::fromUtf8("S"));
        S->setMinimumSize(QSize(32, 32));
        S->setMaximumSize(QSize(32, 32));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/server.svg"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/resource/server.svg"), QSize(), QIcon::Normal, QIcon::On);
        S->setIcon(icon);
        S->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(S);

        server_port = new QLineEdit(newUser);
        server_port->setObjectName(QString::fromUtf8("server_port"));
        server_port->setMinimumSize(QSize(0, 32));
        server_port->setMaximumSize(QSize(16777215, 32));
        server_port->setMaxLength(22);

        horizontalLayout_2->addWidget(server_port);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, -1, 10, -1);
        U = new QPushButton(newUser);
        U->setObjectName(QString::fromUtf8("U"));
        U->setMinimumSize(QSize(32, 32));
        U->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resource/user.svg"), QSize(), QIcon::Normal, QIcon::Off);
        U->setIcon(icon1);
        U->setIconSize(QSize(32, 32));

        horizontalLayout_3->addWidget(U);

        username = new QLineEdit(newUser);
        username->setObjectName(QString::fromUtf8("username"));
        username->setMinimumSize(QSize(0, 32));
        username->setMaximumSize(QSize(16777215, 32));
        username->setMaxLength(8);

        horizontalLayout_3->addWidget(username);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, -1, 10, -1);
        P1 = new QPushButton(newUser);
        P1->setObjectName(QString::fromUtf8("P1"));
        P1->setMinimumSize(QSize(32, 32));
        P1->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resource/passwd.svg"), QSize(), QIcon::Normal, QIcon::Off);
        P1->setIcon(icon2);
        P1->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(P1);

        passwd = new QLineEdit(newUser);
        passwd->setObjectName(QString::fromUtf8("passwd"));
        passwd->setMinimumSize(QSize(0, 32));
        passwd->setMaximumSize(QSize(16777215, 32));
        passwd->setMaxLength(12);
        passwd->setEchoMode(QLineEdit::Password);

        horizontalLayout_4->addWidget(passwd);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(-1, -1, 10, -1);
        P2 = new QPushButton(newUser);
        P2->setObjectName(QString::fromUtf8("P2"));
        P2->setMinimumSize(QSize(32, 32));
        P2->setMaximumSize(QSize(32, 32));
        P2->setIcon(icon2);
        P2->setIconSize(QSize(32, 32));

        horizontalLayout_5->addWidget(P2);

        repasswd = new QLineEdit(newUser);
        repasswd->setObjectName(QString::fromUtf8("repasswd"));
        repasswd->setMinimumSize(QSize(0, 32));
        repasswd->setMaximumSize(QSize(16777215, 32));
        repasswd->setMaxLength(12);
        repasswd->setEchoMode(QLineEdit::Password);

        horizontalLayout_5->addWidget(repasswd);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cancel = new QPushButton(newUser);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setMinimumSize(QSize(64, 32));
        cancel->setMaximumSize(QSize(64, 32));
        cancel->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));

        horizontalLayout->addWidget(cancel);

        ok = new QPushButton(newUser);
        ok->setObjectName(QString::fromUtf8("ok"));
        ok->setMinimumSize(QSize(64, 32));
        ok->setMaximumSize(QSize(64, 32));
        ok->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));

        horizontalLayout->addWidget(ok);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_10->addLayout(verticalLayout);


        retranslateUi(newUser);

        QMetaObject::connectSlotsByName(newUser);
    } // setupUi

    void retranslateUi(QDialog *newUser)
    {
        newUser->setWindowTitle(QCoreApplication::translate("newUser", "New User", nullptr));
        S->setText(QString());
        U->setText(QString());
        P1->setText(QString());
        P2->setText(QString());
        cancel->setText(QCoreApplication::translate("newUser", "Cancel", nullptr));
        ok->setText(QCoreApplication::translate("newUser", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class newUser: public Ui_newUser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWUSER_H
