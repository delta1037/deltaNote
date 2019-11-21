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
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *S;
    QLineEdit *server_port;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *U;
    QLineEdit *username;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *P;
    QLineEdit *password;
    QHBoxLayout *horizontalLayout;
    QPushButton *cancel;
    QPushButton *creteNewUser;
    QPushButton *Login;
    QFrame *line;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *transparentLabel;
    QSlider *transparent;
    QHBoxLayout *horizontalLayout_6;
    QLabel *fontColorLabel;
    QPushButton *chooseFontColor;
    QHBoxLayout *horizontalLayout_7;
    QLabel *IconColorLabel;
    QPushButton *chooseIconColor;
    QHBoxLayout *horizontalLayout_8;
    QLabel *clearDataLabel;
    QPushButton *clearData;

    void setupUi(QDialog *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName(QString::fromUtf8("login"));
        login->resize(560, 360);
        login->setMinimumSize(QSize(560, 360));
        login->setMaximumSize(QSize(560, 360));
        horizontalLayout_9 = new QHBoxLayout(login);
        horizontalLayout_9->setSpacing(20);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(20, 31, 20, 30);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, 10, -1);
        S = new QPushButton(login);
        S->setObjectName(QString::fromUtf8("S"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/server.svg"), QSize(), QIcon::Normal, QIcon::Off);
        S->setIcon(icon);
        S->setIconSize(QSize(32, 32));
        S->setFlat(true);

        horizontalLayout_2->addWidget(S);

        server_port = new QLineEdit(login);
        server_port->setObjectName(QString::fromUtf8("server_port"));
        server_port->setMinimumSize(QSize(0, 32));
        server_port->setMaximumSize(QSize(16777215, 32));
        server_port->setInputMethodHints(Qt::ImhNone);
        server_port->setMaxLength(22);
        server_port->setClearButtonEnabled(true);

        horizontalLayout_2->addWidget(server_port);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(-1, -1, 10, -1);
        U = new QPushButton(login);
        U->setObjectName(QString::fromUtf8("U"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resource/user.svg"), QSize(), QIcon::Normal, QIcon::Off);
        U->setIcon(icon1);
        U->setIconSize(QSize(32, 32));
        U->setFlat(true);

        horizontalLayout_3->addWidget(U);

        username = new QLineEdit(login);
        username->setObjectName(QString::fromUtf8("username"));
        username->setMinimumSize(QSize(0, 32));
        username->setMaximumSize(QSize(16777215, 32));
        username->setInputMethodHints(Qt::ImhNone);
        username->setMaxLength(8);
        username->setClearButtonEnabled(true);

        horizontalLayout_3->addWidget(username);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(-1, -1, 10, -1);
        P = new QPushButton(login);
        P->setObjectName(QString::fromUtf8("P"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resource/passwd.svg"), QSize(), QIcon::Normal, QIcon::Off);
        P->setIcon(icon2);
        P->setIconSize(QSize(32, 32));
        P->setFlat(true);

        horizontalLayout_4->addWidget(P);

        password = new QLineEdit(login);
        password->setObjectName(QString::fromUtf8("password"));
        password->setMinimumSize(QSize(0, 32));
        password->setMaximumSize(QSize(16777215, 32));
        password->setInputMethodHints(Qt::ImhHiddenText|Qt::ImhNoAutoUppercase|Qt::ImhNoPredictiveText|Qt::ImhSensitiveData);
        password->setMaxLength(12);
        password->setFrame(true);
        password->setEchoMode(QLineEdit::Password);
        password->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        password->setClearButtonEnabled(true);

        horizontalLayout_4->addWidget(password);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(20);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, -1, 0, -1);
        cancel = new QPushButton(login);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setMinimumSize(QSize(0, 32));
        cancel->setMaximumSize(QSize(64, 32));
        cancel->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));
        cancel->setFlat(false);

        horizontalLayout->addWidget(cancel);

        creteNewUser = new QPushButton(login);
        creteNewUser->setObjectName(QString::fromUtf8("creteNewUser"));
        creteNewUser->setMinimumSize(QSize(0, 32));
        creteNewUser->setMaximumSize(QSize(64, 32));
        creteNewUser->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));
        creteNewUser->setFlat(false);

        horizontalLayout->addWidget(creteNewUser);

        Login = new QPushButton(login);
        Login->setObjectName(QString::fromUtf8("Login"));
        Login->setMinimumSize(QSize(0, 32));
        Login->setMaximumSize(QSize(64, 32));
        Login->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));
        Login->setFlat(false);

        horizontalLayout->addWidget(Login);


        verticalLayout->addLayout(horizontalLayout);


        horizontalLayout_9->addLayout(verticalLayout);

        line = new QFrame(login);
        line->setObjectName(QString::fromUtf8("line"));
        line->setMinimumSize(QSize(0, 0));
        line->setStyleSheet(QString::fromUtf8("background-color: rgb(32, 74, 135);"));
        line->setLineWidth(2);
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_9->addWidget(line);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        transparentLabel = new QLabel(login);
        transparentLabel->setObjectName(QString::fromUtf8("transparentLabel"));

        horizontalLayout_5->addWidget(transparentLabel);

        transparent = new QSlider(login);
        transparent->setObjectName(QString::fromUtf8("transparent"));
        transparent->setMaximum(255);
        transparent->setSingleStep(5);
        transparent->setPageStep(25);
        transparent->setValue(100);
        transparent->setSliderPosition(100);
        transparent->setOrientation(Qt::Horizontal);

        horizontalLayout_5->addWidget(transparent);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        fontColorLabel = new QLabel(login);
        fontColorLabel->setObjectName(QString::fromUtf8("fontColorLabel"));

        horizontalLayout_6->addWidget(fontColorLabel);

        chooseFontColor = new QPushButton(login);
        chooseFontColor->setObjectName(QString::fromUtf8("chooseFontColor"));
        chooseFontColor->setMinimumSize(QSize(150, 40));
        chooseFontColor->setMaximumSize(QSize(150, 16777215));
        chooseFontColor->setStyleSheet(QString::fromUtf8("border-radius:3px;\n"
"background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 255), stop:0.166 rgba(255, 255, 0, 255), stop:0.333 rgba(0, 255, 0, 255), stop:0.5 rgba(0, 255, 255, 255), stop:0.666 rgba(0, 0, 255, 255), stop:0.833 rgba(255, 0, 255, 255), stop:1 rgba(255, 0, 0, 255));"));

        horizontalLayout_6->addWidget(chooseFontColor);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        IconColorLabel = new QLabel(login);
        IconColorLabel->setObjectName(QString::fromUtf8("IconColorLabel"));

        horizontalLayout_7->addWidget(IconColorLabel);

        chooseIconColor = new QPushButton(login);
        chooseIconColor->setObjectName(QString::fromUtf8("chooseIconColor"));
        chooseIconColor->setMinimumSize(QSize(150, 40));
        chooseIconColor->setMaximumSize(QSize(150, 16777215));
        chooseIconColor->setStyleSheet(QString::fromUtf8("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 255), stop:0.166 rgba(255, 255, 0, 255), stop:0.333 rgba(0, 255, 0, 255), stop:0.5 rgba(0, 255, 255, 255), stop:0.666 rgba(0, 0, 255, 255), stop:0.833 rgba(255, 0, 255, 255), stop:1 rgba(255, 0, 0, 255));\n"
"border-radius:3px;"));

        horizontalLayout_7->addWidget(chooseIconColor);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        clearDataLabel = new QLabel(login);
        clearDataLabel->setObjectName(QString::fromUtf8("clearDataLabel"));

        horizontalLayout_8->addWidget(clearDataLabel);

        clearData = new QPushButton(login);
        clearData->setObjectName(QString::fromUtf8("clearData"));
        clearData->setMinimumSize(QSize(150, 40));
        clearData->setMaximumSize(QSize(150, 16777215));
        clearData->setStyleSheet(QString::fromUtf8("background-color: rgb(239, 41, 41);\n"
"border-radius:3px;"));

        horizontalLayout_8->addWidget(clearData);


        verticalLayout_2->addLayout(horizontalLayout_8);


        horizontalLayout_9->addLayout(verticalLayout_2);


        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QDialog *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "Dialog", nullptr));
        S->setText(QString());
        server_port->setText(QString());
        server_port->setPlaceholderText(QString());
        U->setText(QString());
        username->setText(QString());
        username->setPlaceholderText(QString());
        P->setText(QString());
        password->setText(QString());
        password->setPlaceholderText(QString());
        cancel->setText(QCoreApplication::translate("login", "Cancel", nullptr));
        creteNewUser->setText(QCoreApplication::translate("login", "Create", nullptr));
        Login->setText(QCoreApplication::translate("login", "Login", nullptr));
        transparentLabel->setText(QCoreApplication::translate("login", "Transparent", nullptr));
        fontColorLabel->setText(QCoreApplication::translate("login", "Font Color", nullptr));
        chooseFontColor->setText(QString());
        IconColorLabel->setText(QCoreApplication::translate("login", "Icon Color", nullptr));
        chooseIconColor->setText(QString());
        clearDataLabel->setText(QCoreApplication::translate("login", "Clean Data", nullptr));
        clearData->setText(QCoreApplication::translate("login", "Push", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
