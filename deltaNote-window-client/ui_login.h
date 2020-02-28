/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_0;
    QHBoxLayout *horizontalLayout_0;
    QPushButton *S;
    QLineEdit *server_port;
    QHBoxLayout *horizontalLayout_1;
    QPushButton *U;
    QLineEdit *username;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *P;
    QLineEdit *password;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *cancel;
    QPushButton *creteNewUser;
    QPushButton *Login;
    QFrame *line;
    QVBoxLayout *verticalLayout_1;
    QHBoxLayout *horizontalLayout_4;
    QLabel *transparentLabel;
    QSlider *transparent;
    QHBoxLayout *horizontalLayout_5;
    QLabel *fontColorLabel;
    QPushButton *chooseFontColor;
    QHBoxLayout *horizontalLayout_6;
    QLabel *IconColorLabel;
    QPushButton *chooseIconColor;
    QHBoxLayout *horizontalLayout_78;
    QHBoxLayout *horizontalLayout_7;
    QLabel *clearDataLabel;
    QPushButton *clearData;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label;
    QRadioButton *radioButton;
    QPushButton *exit;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName(QString::fromUtf8("login"));
        login->resize(560, 360);
        login->setMinimumSize(QSize(560, 360));
        login->setMaximumSize(QSize(560, 360));
        login->setWindowOpacity(1.000000000000000);
        login->setAutoFillBackground(true);
        login->setStyleSheet(QString::fromUtf8(""));
        login->setSizeGripEnabled(false);
        gridLayout = new QGridLayout(login);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(40, 0, 0, 40);
        horizontalSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_0 = new QVBoxLayout();
        verticalLayout_0->setSpacing(6);
        verticalLayout_0->setObjectName(QString::fromUtf8("verticalLayout_0"));
        horizontalLayout_0 = new QHBoxLayout();
        horizontalLayout_0->setObjectName(QString::fromUtf8("horizontalLayout_0"));
        horizontalLayout_0->setContentsMargins(-1, -1, 10, -1);
        S = new QPushButton(login);
        S->setObjectName(QString::fromUtf8("S"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/server.svg"), QSize(), QIcon::Normal, QIcon::Off);
        S->setIcon(icon);
        S->setIconSize(QSize(32, 32));
        S->setFlat(true);

        horizontalLayout_0->addWidget(S);

        server_port = new QLineEdit(login);
        server_port->setObjectName(QString::fromUtf8("server_port"));
        server_port->setMinimumSize(QSize(0, 32));
        server_port->setMaximumSize(QSize(16777215, 32));
        server_port->setInputMethodHints(Qt::ImhNone);
        server_port->setMaxLength(22);
        server_port->setClearButtonEnabled(true);

        horizontalLayout_0->addWidget(server_port);


        verticalLayout_0->addLayout(horizontalLayout_0);

        horizontalLayout_1 = new QHBoxLayout();
        horizontalLayout_1->setObjectName(QString::fromUtf8("horizontalLayout_1"));
        horizontalLayout_1->setContentsMargins(-1, -1, 10, -1);
        U = new QPushButton(login);
        U->setObjectName(QString::fromUtf8("U"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resource/user.svg"), QSize(), QIcon::Normal, QIcon::Off);
        U->setIcon(icon1);
        U->setIconSize(QSize(32, 32));
        U->setFlat(true);

        horizontalLayout_1->addWidget(U);

        username = new QLineEdit(login);
        username->setObjectName(QString::fromUtf8("username"));
        username->setMinimumSize(QSize(0, 32));
        username->setMaximumSize(QSize(16777215, 32));
        username->setInputMethodHints(Qt::ImhNone);
        username->setMaxLength(8);
        username->setClearButtonEnabled(true);

        horizontalLayout_1->addWidget(username);


        verticalLayout_0->addLayout(horizontalLayout_1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, -1, 10, -1);
        P = new QPushButton(login);
        P->setObjectName(QString::fromUtf8("P"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resource/passwd.svg"), QSize(), QIcon::Normal, QIcon::Off);
        P->setIcon(icon2);
        P->setIconSize(QSize(32, 32));
        P->setFlat(true);

        horizontalLayout_2->addWidget(P);

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

        horizontalLayout_2->addWidget(password);


        verticalLayout_0->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(20);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, -1, 0, -1);
        cancel = new QPushButton(login);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setMinimumSize(QSize(0, 32));
        cancel->setMaximumSize(QSize(64, 32));
        cancel->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));
        cancel->setFlat(false);

        horizontalLayout_3->addWidget(cancel);

        creteNewUser = new QPushButton(login);
        creteNewUser->setObjectName(QString::fromUtf8("creteNewUser"));
        creteNewUser->setMinimumSize(QSize(0, 32));
        creteNewUser->setMaximumSize(QSize(64, 32));
        creteNewUser->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));
        creteNewUser->setFlat(false);

        horizontalLayout_3->addWidget(creteNewUser);

        Login = new QPushButton(login);
        Login->setObjectName(QString::fromUtf8("Login"));
        Login->setMinimumSize(QSize(0, 32));
        Login->setMaximumSize(QSize(64, 32));
        Login->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));
        Login->setFlat(false);

        horizontalLayout_3->addWidget(Login);


        verticalLayout_0->addLayout(horizontalLayout_3);


        horizontalLayout->addLayout(verticalLayout_0);

        line = new QFrame(login);
        line->setObjectName(QString::fromUtf8("line"));
        line->setMinimumSize(QSize(0, 0));
        line->setStyleSheet(QString::fromUtf8("background-color: rgb(32, 74, 135);"));
        line->setLineWidth(2);
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        verticalLayout_1 = new QVBoxLayout();
        verticalLayout_1->setObjectName(QString::fromUtf8("verticalLayout_1"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        transparentLabel = new QLabel(login);
        transparentLabel->setObjectName(QString::fromUtf8("transparentLabel"));

        horizontalLayout_4->addWidget(transparentLabel);

        transparent = new QSlider(login);
        transparent->setObjectName(QString::fromUtf8("transparent"));
        transparent->setMaximum(255);
        transparent->setSingleStep(5);
        transparent->setPageStep(25);
        transparent->setValue(100);
        transparent->setSliderPosition(100);
        transparent->setTracking(false);
        transparent->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(transparent);


        verticalLayout_1->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        fontColorLabel = new QLabel(login);
        fontColorLabel->setObjectName(QString::fromUtf8("fontColorLabel"));

        horizontalLayout_5->addWidget(fontColorLabel);

        chooseFontColor = new QPushButton(login);
        chooseFontColor->setObjectName(QString::fromUtf8("chooseFontColor"));
        chooseFontColor->setMinimumSize(QSize(150, 40));
        chooseFontColor->setMaximumSize(QSize(150, 16777215));
        chooseFontColor->setStyleSheet(QString::fromUtf8("border-radius:3px;\n"
""));

        horizontalLayout_5->addWidget(chooseFontColor);


        verticalLayout_1->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        IconColorLabel = new QLabel(login);
        IconColorLabel->setObjectName(QString::fromUtf8("IconColorLabel"));

        horizontalLayout_6->addWidget(IconColorLabel);

        chooseIconColor = new QPushButton(login);
        chooseIconColor->setObjectName(QString::fromUtf8("chooseIconColor"));
        chooseIconColor->setMinimumSize(QSize(150, 40));
        chooseIconColor->setMaximumSize(QSize(150, 16777215));
        chooseIconColor->setStyleSheet(QString::fromUtf8("border-radius:3px;"));

        horizontalLayout_6->addWidget(chooseIconColor);


        verticalLayout_1->addLayout(horizontalLayout_6);

        horizontalLayout_78 = new QHBoxLayout();
        horizontalLayout_78->setSpacing(20);
        horizontalLayout_78->setObjectName(QString::fromUtf8("horizontalLayout_78"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        clearDataLabel = new QLabel(login);
        clearDataLabel->setObjectName(QString::fromUtf8("clearDataLabel"));

        horizontalLayout_7->addWidget(clearDataLabel);

        clearData = new QPushButton(login);
        clearData->setObjectName(QString::fromUtf8("clearData"));
        clearData->setMinimumSize(QSize(0, 40));
        clearData->setMaximumSize(QSize(150, 16777215));
        clearData->setStyleSheet(QString::fromUtf8("background-color: rgb(239, 41, 41);\n"
"border-radius:3px;"));

        horizontalLayout_7->addWidget(clearData);


        horizontalLayout_78->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(0, -1, 25, -1);
        label = new QLabel(login);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_8->addWidget(label);

        radioButton = new QRadioButton(login);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout_8->addWidget(radioButton);


        horizontalLayout_78->addLayout(horizontalLayout_8);


        verticalLayout_1->addLayout(horizontalLayout_78);


        horizontalLayout->addLayout(verticalLayout_1);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        exit = new QPushButton(login);
        exit->setObjectName(QString::fromUtf8("exit"));
        exit->setMinimumSize(QSize(40, 40));
        exit->setMaximumSize(QSize(40, 40));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resource/exit.svg"), QSize(), QIcon::Normal, QIcon::Off);
        exit->setIcon(icon3);
        exit->setIconSize(QSize(40, 40));

        gridLayout->addWidget(exit, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 1, 1, 1);

        gridLayout->setRowStretch(0, 99);
        gridLayout->setColumnStretch(0, 99);

        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QDialog *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "Setting", nullptr));
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
        clearData->setText(QCoreApplication::translate("login", "Clean", nullptr));
        label->setText(QCoreApplication::translate("login", "Auto Start", nullptr));
        radioButton->setText(QString());
        exit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
