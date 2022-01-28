/********************************************************************************
** Form generated from reading UI file 'todo_detail.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TODO_DETAIL_H
#define UI_TODO_DETAIL_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TodoDetail
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *text_line;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout;
    QPushButton *i_b;
    QComboBox *tag;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *c_b;
    QDateTimeEdit *ddl;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *cancel;
    QPushButton *accept;

    void setupUi(QDialog *TodoDetail)
    {
        if (TodoDetail->objectName().isEmpty())
            TodoDetail->setObjectName(QString::fromUtf8("TodoDetail"));
        TodoDetail->resize(350, 240);
        verticalLayout = new QVBoxLayout(TodoDetail);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(20, 15, 20, 15);
        text_line = new QLineEdit(TodoDetail);
        text_line->setObjectName(QString::fromUtf8("text_line"));
        text_line->setMinimumSize(QSize(0, 35));
        text_line->setMaximumSize(QSize(16777215, 35));
        text_line->setAcceptDrops(false);
        text_line->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"border-style:dotted;\n"
"border-width:1px;\n"
"border-color: rgb(188, 250, 109);"));

        verticalLayout->addWidget(text_line);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(15);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        i_b = new QPushButton(TodoDetail);
        i_b->setObjectName(QString::fromUtf8("i_b"));
        i_b->setMinimumSize(QSize(35, 35));
        i_b->setMaximumSize(QSize(35, 35));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/resource/important.svg"), QSize(), QIcon::Normal, QIcon::Off);
        i_b->setIcon(icon);
        i_b->setIconSize(QSize(35, 35));
        i_b->setFlat(true);

        horizontalLayout->addWidget(i_b);

        tag = new QComboBox(TodoDetail);
        tag->setObjectName(QString::fromUtf8("tag"));
        tag->setMinimumSize(QSize(0, 35));
        tag->setMaximumSize(QSize(150, 35));
        tag->setAutoFillBackground(false);
        tag->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"	border-style:dotted;\n"
"	border-width:1px;\n"
"	border-color: rgb(188, 250, 109);\n"
"}\n"
""));
        tag->setFrame(false);

        horizontalLayout->addWidget(tag);


        horizontalLayout_4->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        c_b = new QPushButton(TodoDetail);
        c_b->setObjectName(QString::fromUtf8("c_b"));
        c_b->setMinimumSize(QSize(35, 35));
        c_b->setMaximumSize(QSize(35, 35));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/resource/clock.svg"), QSize(), QIcon::Normal, QIcon::Off);
        c_b->setIcon(icon1);
        c_b->setIconSize(QSize(35, 35));
        c_b->setFlat(true);

        horizontalLayout_3->addWidget(c_b);

        ddl = new QDateTimeEdit(TodoDetail);
        ddl->setObjectName(QString::fromUtf8("ddl"));
        ddl->setMinimumSize(QSize(0, 35));
        ddl->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"border-style:dotted;\n"
"border-width:1px;\n"
"border-color: rgb(188, 250, 109);"));

        horizontalLayout_3->addWidget(ddl);


        horizontalLayout_4->addLayout(horizontalLayout_3);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(50);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(30, -1, 30, -1);
        cancel = new QPushButton(TodoDetail);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setMinimumSize(QSize(100, 35));
        cancel->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));

        horizontalLayout_2->addWidget(cancel);

        accept = new QPushButton(TodoDetail);
        accept->setObjectName(QString::fromUtf8("accept"));
        accept->setMinimumSize(QSize(100, 35));
        accept->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));

        horizontalLayout_2->addWidget(accept);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(TodoDetail);

        QMetaObject::connectSlotsByName(TodoDetail);
    } // setupUi

    void retranslateUi(QDialog *TodoDetail)
    {
        TodoDetail->setWindowTitle(QCoreApplication::translate("TodoDetail", "Dialog", nullptr));
        i_b->setText(QString());
        c_b->setText(QString());
        cancel->setText(QCoreApplication::translate("TodoDetail", "Cancel", nullptr));
        accept->setText(QCoreApplication::translate("TodoDetail", "Accept", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TodoDetail: public Ui_TodoDetail {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TODO_DETAIL_H
