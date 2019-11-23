/********************************************************************************
** Form generated from reading UI file 'newuser.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
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
    QPushButton *pushButton;
    QLineEdit *server_port;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_2;
    QLineEdit *username;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_3;
    QLineEdit *passwd;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton_4;
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
        pushButton = new QPushButton(newUser);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(32, 32));
        pushButton->setMaximumSize(QSize(32, 32));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/server.svg"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/resource/server.svg"), QSize(), QIcon::Normal, QIcon::On);
        pushButton->setIcon(icon);
        pushButton->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(pushButton);

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
        pushButton_2 = new QPushButton(newUser);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(32, 32));
        pushButton_2->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resource/user.svg"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_2->setIcon(icon1);
        pushButton_2->setIconSize(QSize(32, 32));

        horizontalLayout_3->addWidget(pushButton_2);

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
        pushButton_3 = new QPushButton(newUser);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setMinimumSize(QSize(32, 32));
        pushButton_3->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resource/passwd.svg"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_3->setIcon(icon2);
        pushButton_3->setIconSize(QSize(32, 32));

        horizontalLayout_4->addWidget(pushButton_3);

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
        pushButton_4 = new QPushButton(newUser);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setMinimumSize(QSize(32, 32));
        pushButton_4->setMaximumSize(QSize(32, 32));
        pushButton_4->setIcon(icon2);
        pushButton_4->setIconSize(QSize(32, 32));

        horizontalLayout_5->addWidget(pushButton_4);

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

        horizontalLayout->addWidget(cancel);

        ok = new QPushButton(newUser);
        ok->setObjectName(QString::fromUtf8("ok"));
        ok->setMinimumSize(QSize(64, 32));
        ok->setMaximumSize(QSize(64, 32));

        horizontalLayout->addWidget(ok);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_10->addLayout(verticalLayout);


        retranslateUi(newUser);

        QMetaObject::connectSlotsByName(newUser);
    } // setupUi

    void retranslateUi(QDialog *newUser)
    {
        newUser->setWindowTitle(QCoreApplication::translate("newUser", "New User", nullptr));
        pushButton->setText(QString());
        pushButton_2->setText(QString());
        pushButton_3->setText(QString());
        pushButton_4->setText(QString());
        cancel->setText(QCoreApplication::translate("newUser", "Cancel", nullptr));
        ok->setText(QCoreApplication::translate("newUser", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class newUser: public Ui_newUser {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWUSER_H
