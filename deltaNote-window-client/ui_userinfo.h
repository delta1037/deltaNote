/********************************************************************************
** Form generated from reading UI file 'userinfo.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERINFO_H
#define UI_USERINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_userInfo
{
public:
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QLabel *tip_user;
    QLineEdit *username;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *cancel;
    QPushButton *logout;
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
    QHBoxLayout *horizontalLayout_10;
    QHBoxLayout *horizontalLayout_8;
    QLabel *clearDataLabel;
    QPushButton *clearData;
    QHBoxLayout *horizontalLayout_9;
    QLabel *clearDataLabel_2;
    QRadioButton *radioButton;

    void setupUi(QDialog *userInfo)
    {
        if (userInfo->objectName().isEmpty())
            userInfo->setObjectName(QString::fromUtf8("userInfo"));
        userInfo->resize(560, 360);
        userInfo->setMinimumSize(QSize(560, 360));
        userInfo->setMaximumSize(QSize(560, 360));
        horizontalLayout_3 = new QHBoxLayout(userInfo);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 20, -1, 60);
        label = new QLabel(userInfo);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(14);
        label->setFont(font);

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, 20, -1);
        tip_user = new QLabel(userInfo);
        tip_user->setObjectName(QString::fromUtf8("tip_user"));

        horizontalLayout->addWidget(tip_user);

        username = new QLineEdit(userInfo);
        username->setObjectName(QString::fromUtf8("username"));
        username->setFrame(false);
        username->setReadOnly(true);

        horizontalLayout->addWidget(username);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        cancel = new QPushButton(userInfo);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setMinimumSize(QSize(64, 32));
        cancel->setMaximumSize(QSize(64, 32));
        cancel->setStyleSheet(QString::fromUtf8("background-color: rgb(188, 250, 109);\n"
"border-radius:10px;"));

        horizontalLayout_2->addWidget(cancel);

        logout = new QPushButton(userInfo);
        logout->setObjectName(QString::fromUtf8("logout"));
        logout->setMinimumSize(QSize(64, 32));
        logout->setMaximumSize(QSize(64, 32));
        logout->setStyleSheet(QString::fromUtf8("background-color: rgb(188, 250, 109);\n"
"border-radius:10px;"));

        horizontalLayout_2->addWidget(logout);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout);

        line = new QFrame(userInfo);
        line->setObjectName(QString::fromUtf8("line"));
        line->setMinimumSize(QSize(0, 0));
        line->setStyleSheet(QString::fromUtf8("background-color: rgb(32, 74, 135);"));
        line->setLineWidth(2);
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_3->addWidget(line);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        transparentLabel = new QLabel(userInfo);
        transparentLabel->setObjectName(QString::fromUtf8("transparentLabel"));

        horizontalLayout_5->addWidget(transparentLabel);

        transparent = new QSlider(userInfo);
        transparent->setObjectName(QString::fromUtf8("transparent"));
        transparent->setMaximum(255);
        transparent->setSingleStep(5);
        transparent->setPageStep(25);
        transparent->setValue(100);
        transparent->setSliderPosition(100);
        transparent->setTracking(false);
        transparent->setOrientation(Qt::Horizontal);

        horizontalLayout_5->addWidget(transparent);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        fontColorLabel = new QLabel(userInfo);
        fontColorLabel->setObjectName(QString::fromUtf8("fontColorLabel"));

        horizontalLayout_6->addWidget(fontColorLabel);

        chooseFontColor = new QPushButton(userInfo);
        chooseFontColor->setObjectName(QString::fromUtf8("chooseFontColor"));
        chooseFontColor->setMinimumSize(QSize(150, 40));
        chooseFontColor->setMaximumSize(QSize(150, 16777215));
        chooseFontColor->setStyleSheet(QString::fromUtf8("border-radius:3px;\n"
""));

        horizontalLayout_6->addWidget(chooseFontColor);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        IconColorLabel = new QLabel(userInfo);
        IconColorLabel->setObjectName(QString::fromUtf8("IconColorLabel"));

        horizontalLayout_7->addWidget(IconColorLabel);

        chooseIconColor = new QPushButton(userInfo);
        chooseIconColor->setObjectName(QString::fromUtf8("chooseIconColor"));
        chooseIconColor->setMinimumSize(QSize(150, 40));
        chooseIconColor->setMaximumSize(QSize(150, 16777215));
        chooseIconColor->setStyleSheet(QString::fromUtf8("border-radius:3px;"));

        horizontalLayout_7->addWidget(chooseIconColor);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(20);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        clearDataLabel = new QLabel(userInfo);
        clearDataLabel->setObjectName(QString::fromUtf8("clearDataLabel"));

        horizontalLayout_8->addWidget(clearDataLabel);

        clearData = new QPushButton(userInfo);
        clearData->setObjectName(QString::fromUtf8("clearData"));
        clearData->setMinimumSize(QSize(0, 40));
        clearData->setMaximumSize(QSize(150, 16777215));
        clearData->setStyleSheet(QString::fromUtf8("background-color: rgb(239, 41, 41);\n"
"border-radius:3px;"));

        horizontalLayout_8->addWidget(clearData);


        horizontalLayout_10->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(-1, -1, 20, -1);
        clearDataLabel_2 = new QLabel(userInfo);
        clearDataLabel_2->setObjectName(QString::fromUtf8("clearDataLabel_2"));

        horizontalLayout_9->addWidget(clearDataLabel_2);

        radioButton = new QRadioButton(userInfo);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout_9->addWidget(radioButton);


        horizontalLayout_10->addLayout(horizontalLayout_9);


        verticalLayout_2->addLayout(horizontalLayout_10);


        horizontalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(userInfo);
        QObject::connect(logout, SIGNAL(clicked()), userInfo, SLOT(accept()));

        QMetaObject::connectSlotsByName(userInfo);
    } // setupUi

    void retranslateUi(QDialog *userInfo)
    {
        userInfo->setWindowTitle(QCoreApplication::translate("userInfo", "Setting", nullptr));
        label->setText(QCoreApplication::translate("userInfo", "    Login Success", nullptr));
        tip_user->setText(QCoreApplication::translate("userInfo", "Username:", nullptr));
        cancel->setText(QCoreApplication::translate("userInfo", "Cancel", nullptr));
        logout->setText(QCoreApplication::translate("userInfo", "Logout", nullptr));
        transparentLabel->setText(QCoreApplication::translate("userInfo", "Transparent", nullptr));
        fontColorLabel->setText(QCoreApplication::translate("userInfo", "Font Color", nullptr));
        chooseFontColor->setText(QString());
        IconColorLabel->setText(QCoreApplication::translate("userInfo", "Icon Color", nullptr));
        chooseIconColor->setText(QString());
        clearDataLabel->setText(QCoreApplication::translate("userInfo", "Clean Data", nullptr));
        clearData->setText(QCoreApplication::translate("userInfo", "Clean", nullptr));
        clearDataLabel_2->setText(QCoreApplication::translate("userInfo", "AutoStart", nullptr));
        radioButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class userInfo: public Ui_userInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERINFO_H
