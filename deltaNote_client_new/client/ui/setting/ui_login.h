/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
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
    QPushButton *exit;
    QHBoxLayout *horizontalLayout_8;
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
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *transparentLabel;
    QSlider *transparent;
    QHBoxLayout *horizontalLayout_5;
    QLabel *fontSizeLabel;
    QSlider *fontSizeSlider;
    QHBoxLayout *horizontalLayout;
    QLabel *mainWinWidthLabel;
    QSlider *mainWinWidthSlider;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *chooseFontColor;
    QPushButton *choose_bg_color;
    QPushButton *chooseIconColor;
    QHBoxLayout *horizontalLayout_7;
    QLabel *auto_start;
    QRadioButton *radioButton;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName(QString::fromUtf8("login"));
        login->resize(560, 360);
        login->setMinimumSize(QSize(560, 360));
        login->setMaximumSize(QSize(560, 360));
        login->setCursor(QCursor(Qt::SizeAllCursor));
        login->setWindowOpacity(1.000000000000000);
        login->setAutoFillBackground(true);
        login->setStyleSheet(QString::fromUtf8(""));
        login->setSizeGripEnabled(false);
        gridLayout = new QGridLayout(login);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        exit = new QPushButton(login);
        exit->setObjectName(QString::fromUtf8("exit"));
        exit->setMinimumSize(QSize(40, 40));
        exit->setMaximumSize(QSize(40, 40));
        exit->setCursor(QCursor(Qt::PointingHandCursor));
        exit->setStyleSheet(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/exit.svg"), QSize(), QIcon::Normal, QIcon::Off);
        exit->setIcon(icon);
        exit->setIconSize(QSize(40, 40));
        exit->setFlat(true);

        gridLayout->addWidget(exit, 0, 1, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(0);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setContentsMargins(40, 0, -1, 40);
        verticalLayout_0 = new QVBoxLayout();
        verticalLayout_0->setSpacing(6);
        verticalLayout_0->setObjectName(QString::fromUtf8("verticalLayout_0"));
        verticalLayout_0->setContentsMargins(-1, -1, 0, -1);
        horizontalLayout_0 = new QHBoxLayout();
        horizontalLayout_0->setObjectName(QString::fromUtf8("horizontalLayout_0"));
        horizontalLayout_0->setContentsMargins(-1, -1, 10, -1);
        S = new QPushButton(login);
        S->setObjectName(QString::fromUtf8("S"));
        S->setCursor(QCursor(Qt::ForbiddenCursor));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resource/server.svg"), QSize(), QIcon::Normal, QIcon::Off);
        S->setIcon(icon1);
        S->setIconSize(QSize(32, 32));
        S->setFlat(true);

        horizontalLayout_0->addWidget(S);

        server_port = new QLineEdit(login);
        server_port->setObjectName(QString::fromUtf8("server_port"));
        server_port->setMinimumSize(QSize(175, 32));
        server_port->setMaximumSize(QSize(175, 32));
        QFont font;
        font.setPointSize(8);
        server_port->setFont(font);
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
        U->setCursor(QCursor(Qt::ForbiddenCursor));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/resource/user.svg"), QSize(), QIcon::Normal, QIcon::Off);
        U->setIcon(icon2);
        U->setIconSize(QSize(32, 32));
        U->setFlat(true);

        horizontalLayout_1->addWidget(U);

        username = new QLineEdit(login);
        username->setObjectName(QString::fromUtf8("username"));
        username->setMinimumSize(QSize(175, 32));
        username->setMaximumSize(QSize(175, 32));
        username->setFont(font);
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
        QFont font1;
        P->setFont(font1);
        P->setCursor(QCursor(Qt::ForbiddenCursor));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/resource/passwd.svg"), QSize(), QIcon::Normal, QIcon::Off);
        P->setIcon(icon3);
        P->setIconSize(QSize(32, 32));
        P->setFlat(true);

        horizontalLayout_2->addWidget(P);

        password = new QLineEdit(login);
        password->setObjectName(QString::fromUtf8("password"));
        password->setMinimumSize(QSize(175, 32));
        password->setMaximumSize(QSize(175, 32));
        password->setFont(font);
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
        cancel->setFont(font);
        cancel->setCursor(QCursor(Qt::PointingHandCursor));
        cancel->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));
        cancel->setFlat(false);

        horizontalLayout_3->addWidget(cancel);

        creteNewUser = new QPushButton(login);
        creteNewUser->setObjectName(QString::fromUtf8("creteNewUser"));
        creteNewUser->setMinimumSize(QSize(0, 32));
        creteNewUser->setMaximumSize(QSize(64, 32));
        creteNewUser->setFont(font);
        creteNewUser->setCursor(QCursor(Qt::PointingHandCursor));
        creteNewUser->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));
        creteNewUser->setFlat(false);

        horizontalLayout_3->addWidget(creteNewUser);

        Login = new QPushButton(login);
        Login->setObjectName(QString::fromUtf8("Login"));
        Login->setMinimumSize(QSize(0, 32));
        Login->setMaximumSize(QSize(64, 32));
        Login->setFont(font);
        Login->setCursor(QCursor(Qt::PointingHandCursor));
        Login->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));
        Login->setFlat(false);

        horizontalLayout_3->addWidget(Login);


        verticalLayout_0->addLayout(horizontalLayout_3);


        horizontalLayout_8->addLayout(verticalLayout_0);

        line = new QFrame(login);
        line->setObjectName(QString::fromUtf8("line"));
        line->setMinimumSize(QSize(0, 0));
        line->setStyleSheet(QString::fromUtf8("background-color: rgb(32, 74, 135);"));
        line->setLineWidth(2);
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_8->addWidget(line);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(4, -1, -1, -1);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(0);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(15, -1, -1, -1);
        transparentLabel = new QLabel(login);
        transparentLabel->setObjectName(QString::fromUtf8("transparentLabel"));
        QFont font2;
        font2.setPointSize(9);
        transparentLabel->setFont(font2);

        horizontalLayout_4->addWidget(transparentLabel);

        transparent = new QSlider(login);
        transparent->setObjectName(QString::fromUtf8("transparent"));
        transparent->setCursor(QCursor(Qt::PointingHandCursor));
        transparent->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal {\n"
"background: rgba(0, 0, 0, 0);\n"
"height: 4px;\n"
"border-radius: 2px;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,\n"
"    stop: 0 #66e, stop: 1 #bbf);\n"
"background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,\n"
"    stop: 0 #bbf, stop: 1 #55f);\n"
"border: 1px solid #777;\n"
"height: 10px;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::add-page:horizontal {\n"
"background: #fff;\n"
"border: 1px solid #777;\n"
"height: 5px;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"    stop:0 #eee, stop:1 #ccc);\n"
"border: 1px solid #777;\n"
"width: 11px;\n"
"margin-top: -3px;\n"
"margin-bottom: -3px;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:hover {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"    stop:0 #fff, stop:1 #ddd);\n"
"border: 1px solid #444;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::s"
                        "ub-page:horizontal:disabled {\n"
"background: #bbb;\n"
"border-color: #999;\n"
"}\n"
"\n"
"QSlider::add-page:horizontal:disabled {\n"
"background: #eee;\n"
"border-color: #999;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:disabled {\n"
"background: #eee;\n"
"border: 1px solid #aaa;\n"
"border-radius: 4px;\n"
"}"));
        transparent->setMaximum(255);
        transparent->setSingleStep(5);
        transparent->setPageStep(25);
        transparent->setValue(100);
        transparent->setSliderPosition(100);
        transparent->setTracking(false);
        transparent->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(transparent);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        fontSizeLabel = new QLabel(login);
        fontSizeLabel->setObjectName(QString::fromUtf8("fontSizeLabel"));
        fontSizeLabel->setFont(font2);

        horizontalLayout_5->addWidget(fontSizeLabel);

        fontSizeSlider = new QSlider(login);
        fontSizeSlider->setObjectName(QString::fromUtf8("fontSizeSlider"));
        fontSizeSlider->setCursor(QCursor(Qt::PointingHandCursor));
        fontSizeSlider->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal {\n"
"background: rgba(0, 0, 0, 0);\n"
"height: 4px;\n"
"border-radius: 2px;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,\n"
"    stop: 0 #66e, stop: 1 #bbf);\n"
"background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,\n"
"    stop: 0 #bbf, stop: 1 #55f);\n"
"border: 1px solid #777;\n"
"height: 10px;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::add-page:horizontal {\n"
"background: #fff;\n"
"border: 1px solid #777;\n"
"height: 5px;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"    stop:0 #eee, stop:1 #ccc);\n"
"border: 1px solid #777;\n"
"width: 11px;\n"
"margin-top: -3px;\n"
"margin-bottom: -3px;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:hover {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"    stop:0 #fff, stop:1 #ddd);\n"
"border: 1px solid #444;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::s"
                        "ub-page:horizontal:disabled {\n"
"background: #bbb;\n"
"border-color: #999;\n"
"}\n"
"\n"
"QSlider::add-page:horizontal:disabled {\n"
"background: #eee;\n"
"border-color: #999;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:disabled {\n"
"background: #eee;\n"
"border: 1px solid #aaa;\n"
"border-radius: 4px;\n"
"}"));
        fontSizeSlider->setMinimum(9);
        fontSizeSlider->setMaximum(16);
        fontSizeSlider->setPageStep(1);
        fontSizeSlider->setValue(11);
        fontSizeSlider->setSliderPosition(11);
        fontSizeSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_5->addWidget(fontSizeSlider);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        mainWinWidthLabel = new QLabel(login);
        mainWinWidthLabel->setObjectName(QString::fromUtf8("mainWinWidthLabel"));
        mainWinWidthLabel->setFont(font2);

        horizontalLayout->addWidget(mainWinWidthLabel);

        mainWinWidthSlider = new QSlider(login);
        mainWinWidthSlider->setObjectName(QString::fromUtf8("mainWinWidthSlider"));
        mainWinWidthSlider->setCursor(QCursor(Qt::PointingHandCursor));
        mainWinWidthSlider->setStyleSheet(QString::fromUtf8("QSlider::groove:horizontal {\n"
"background: rgba(0, 0, 0, 0);\n"
"height: 4px;\n"
"border-radius: 2px;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1,\n"
"    stop: 0 #66e, stop: 1 #bbf);\n"
"background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,\n"
"    stop: 0 #bbf, stop: 1 #55f);\n"
"border: 1px solid #777;\n"
"height: 10px;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::add-page:horizontal {\n"
"background: #fff;\n"
"border: 1px solid #777;\n"
"height: 5px;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"    stop:0 #eee, stop:1 #ccc);\n"
"border: 1px solid #777;\n"
"width: 11px;\n"
"margin-top: -3px;\n"
"margin-bottom: -3px;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:hover {\n"
"background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"    stop:0 #fff, stop:1 #ddd);\n"
"border: 1px solid #444;\n"
"border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::s"
                        "ub-page:horizontal:disabled {\n"
"background: #bbb;\n"
"border-color: #999;\n"
"}\n"
"\n"
"QSlider::add-page:horizontal:disabled {\n"
"background: #eee;\n"
"border-color: #999;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:disabled {\n"
"background: #eee;\n"
"border: 1px solid #aaa;\n"
"border-radius: 4px;\n"
"}"));
        mainWinWidthSlider->setMinimum(280);
        mainWinWidthSlider->setMaximum(560);
        mainWinWidthSlider->setValue(300);
        mainWinWidthSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(mainWinWidthSlider);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        chooseFontColor = new QPushButton(login);
        chooseFontColor->setObjectName(QString::fromUtf8("chooseFontColor"));
        chooseFontColor->setMinimumSize(QSize(60, 40));
        chooseFontColor->setMaximumSize(QSize(60, 40));
        QFont font3;
        font3.setPointSize(11);
        chooseFontColor->setFont(font3);
        chooseFontColor->setCursor(QCursor(Qt::PointingHandCursor));
        chooseFontColor->setStyleSheet(QString::fromUtf8("border-radius:3px;\n"
""));
        chooseFontColor->setFlat(false);

        horizontalLayout_6->addWidget(chooseFontColor);

        choose_bg_color = new QPushButton(login);
        choose_bg_color->setObjectName(QString::fromUtf8("choose_bg_color"));
        choose_bg_color->setMinimumSize(QSize(60, 40));
        choose_bg_color->setMaximumSize(QSize(60, 40));
        choose_bg_color->setFlat(false);

        horizontalLayout_6->addWidget(choose_bg_color);

        chooseIconColor = new QPushButton(login);
        chooseIconColor->setObjectName(QString::fromUtf8("chooseIconColor"));
        chooseIconColor->setMinimumSize(QSize(60, 40));
        chooseIconColor->setMaximumSize(QSize(60, 40));
        chooseIconColor->setFont(font3);
        chooseIconColor->setCursor(QCursor(Qt::PointingHandCursor));
        chooseIconColor->setStyleSheet(QString::fromUtf8("border-radius:3px;"));
        chooseIconColor->setFlat(false);

        horizontalLayout_6->addWidget(chooseIconColor);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(0);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(40, -1, 80, -1);
        auto_start = new QLabel(login);
        auto_start->setObjectName(QString::fromUtf8("auto_start"));
        auto_start->setFont(font1);

        horizontalLayout_7->addWidget(auto_start);

        radioButton = new QRadioButton(login);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout_7->addWidget(radioButton);


        verticalLayout->addLayout(horizontalLayout_7);


        horizontalLayout_8->addLayout(verticalLayout);


        gridLayout->addLayout(horizontalLayout_8, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);


        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QDialog *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "Setting", nullptr));
        exit->setText(QString());
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
        transparentLabel->setText(QCoreApplication::translate("login", "\351\200\217\346\230\216\345\272\246 ", nullptr));
        fontSizeLabel->setText(QCoreApplication::translate("login", "\345\255\227\344\275\223\345\244\247\345\260\217", nullptr));
        mainWinWidthLabel->setText(QCoreApplication::translate("login", "\344\270\273\347\225\214\351\235\242\345\256\275", nullptr));
        chooseFontColor->setText(QCoreApplication::translate("login", "\345\255\227\344\275\223", nullptr));
        choose_bg_color->setText(QCoreApplication::translate("login", "\350\203\214\346\231\257", nullptr));
        chooseIconColor->setText(QCoreApplication::translate("login", "\345\233\276\346\240\207", nullptr));
        auto_start->setText(QCoreApplication::translate("login", "\345\274\200\346\234\272\345\220\257\345\212\250", nullptr));
        radioButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
