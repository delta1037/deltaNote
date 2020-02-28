/********************************************************************************
** Form generated from reading UI file 'choosecolor.ui'
**
** Created by: Qt User Interface Compiler version 5.14.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHOOSECOLOR_H
#define UI_CHOOSECOLOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_chooseColor
{
public:
    QPushButton *color_1_1;
    QPushButton *color_1_2;
    QPushButton *color_1_3;
    QPushButton *color_1_4;
    QPushButton *color_2_1;
    QPushButton *color_2_2;
    QPushButton *color_2_3;
    QPushButton *color_2_4;

    void setupUi(QDialog *chooseColor)
    {
        if (chooseColor->objectName().isEmpty())
            chooseColor->setObjectName(QString::fromUtf8("chooseColor"));
        chooseColor->resize(360, 120);
        chooseColor->setMinimumSize(QSize(360, 120));
        chooseColor->setMaximumSize(QSize(360, 120));
        chooseColor->setSizeGripEnabled(false);
        color_1_1 = new QPushButton(chooseColor);
        color_1_1->setObjectName(QString::fromUtf8("color_1_1"));
        color_1_1->setGeometry(QRect(10, 20, 71, 31));
        color_1_1->setStyleSheet(QString::fromUtf8("border-radius:6px;\n"
"background-color: rgb(239, 41, 41);\n"
""));
        color_1_2 = new QPushButton(chooseColor);
        color_1_2->setObjectName(QString::fromUtf8("color_1_2"));
        color_1_2->setGeometry(QRect(100, 20, 71, 31));
        color_1_2->setStyleSheet(QString::fromUtf8("border-radius:6px;\n"
"background-color: rgb(252, 175, 62);\n"
""));
        color_1_3 = new QPushButton(chooseColor);
        color_1_3->setObjectName(QString::fromUtf8("color_1_3"));
        color_1_3->setGeometry(QRect(190, 20, 71, 31));
        color_1_3->setStyleSheet(QString::fromUtf8("border-radius:6px;\n"
"background-color: rgb(252, 233, 79);\n"
""));
        color_1_4 = new QPushButton(chooseColor);
        color_1_4->setObjectName(QString::fromUtf8("color_1_4"));
        color_1_4->setGeometry(QRect(280, 20, 71, 31));
        color_1_4->setStyleSheet(QString::fromUtf8("border-radius:6px;\n"
"background-color: rgb(255, 255, 255);\n"
""));
        color_2_1 = new QPushButton(chooseColor);
        color_2_1->setObjectName(QString::fromUtf8("color_2_1"));
        color_2_1->setGeometry(QRect(10, 70, 71, 31));
        color_2_1->setStyleSheet(QString::fromUtf8("border-radius:6px;\n"
"background-color: rgb(188, 250, 109);\n"
""));
        color_2_2 = new QPushButton(chooseColor);
        color_2_2->setObjectName(QString::fromUtf8("color_2_2"));
        color_2_2->setGeometry(QRect(100, 70, 71, 31));
        color_2_2->setStyleSheet(QString::fromUtf8("border-radius:6px;\n"
"background-color: rgb(114, 159, 207);\n"
""));
        color_2_3 = new QPushButton(chooseColor);
        color_2_3->setObjectName(QString::fromUtf8("color_2_3"));
        color_2_3->setGeometry(QRect(190, 70, 71, 31));
        color_2_3->setStyleSheet(QString::fromUtf8("border-radius:6px;\n"
"background-color: rgb(173, 127, 168);\n"
"\n"
""));
        color_2_4 = new QPushButton(chooseColor);
        color_2_4->setObjectName(QString::fromUtf8("color_2_4"));
        color_2_4->setGeometry(QRect(280, 70, 71, 31));
        color_2_4->setStyleSheet(QString::fromUtf8("border-radius:6px;\n"
"background-color: rgb(0, 0, 0);\n"
""));

        retranslateUi(chooseColor);

        QMetaObject::connectSlotsByName(chooseColor);
    } // setupUi

    void retranslateUi(QDialog *chooseColor)
    {
        chooseColor->setWindowTitle(QCoreApplication::translate("chooseColor", "Dialog", nullptr));
        color_1_1->setText(QString());
        color_1_2->setText(QString());
        color_1_3->setText(QString());
        color_1_4->setText(QString());
        color_2_1->setText(QString());
        color_2_2->setText(QString());
        color_2_3->setText(QString());
        color_2_4->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class chooseColor: public Ui_chooseColor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHOOSECOLOR_H
