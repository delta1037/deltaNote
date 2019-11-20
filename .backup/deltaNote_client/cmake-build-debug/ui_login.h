/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QPushButton *Login;
    QLineEdit *server_port;
    QLineEdit *username;
    QLineEdit *password;
    QPushButton *cancel;

    void setupUi(QDialog *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName(QString::fromUtf8("login"));
        login->resize(400, 300);
        Login = new QPushButton(login);
        Login->setObjectName(QString::fromUtf8("Login"));
        Login->setGeometry(QRect(200, 230, 81, 31));
        Login->setFlat(false);
        server_port = new QLineEdit(login);
        server_port->setObjectName(QString::fromUtf8("server_port"));
        server_port->setGeometry(QRect(80, 70, 201, 31));
        server_port->setInputMethodHints(Qt::ImhNone);
        server_port->setMaxLength(21);
        server_port->setClearButtonEnabled(true);
        username = new QLineEdit(login);
        username->setObjectName(QString::fromUtf8("username"));
        username->setGeometry(QRect(80, 120, 201, 31));
        username->setInputMethodHints(Qt::ImhNone);
        username->setMaxLength(8);
        username->setClearButtonEnabled(true);
        password = new QLineEdit(login);
        password->setObjectName(QString::fromUtf8("password"));
        password->setGeometry(QRect(80, 180, 201, 31));
        password->setInputMethodHints(Qt::ImhNone);
        password->setMaxLength(12);
        password->setFrame(true);
        password->setEchoMode(QLineEdit::Password);
        password->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        password->setClearButtonEnabled(true);
        cancel = new QPushButton(login);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setGeometry(QRect(80, 230, 81, 31));

        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QDialog *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "Dialog", nullptr));
        Login->setText(QCoreApplication::translate("login", "Login", nullptr));
        server_port->setText(QString());
        server_port->setPlaceholderText(QString());
        username->setText(QString());
        username->setPlaceholderText(QString());
        password->setText(QString());
        password->setPlaceholderText(QString());
        cancel->setText(QCoreApplication::translate("login", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
