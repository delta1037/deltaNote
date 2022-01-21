/********************************************************************************
** Form generated from reading UI file 'checkupdate.ui'
**
** Created by: Qt User Interface Compiler version 6.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHECKUPDATE_H
#define UI_CHECKUPDATE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CheckUpdate
{
public:
    QVBoxLayout *verticalLayout;
    QProgressBar *progressBar;
    QTextBrowser *message;
    QHBoxLayout *horizontalLayout;
    QPushButton *cancel;
    QPushButton *abort;
    QPushButton *yes;

    void setupUi(QDialog *CheckUpdate)
    {
        if (CheckUpdate->objectName().isEmpty())
            CheckUpdate->setObjectName(QString::fromUtf8("CheckUpdate"));
        CheckUpdate->resize(372, 271);
        CheckUpdate->setMinimumSize(QSize(372, 271));
        CheckUpdate->setMaximumSize(QSize(372, 271));
        verticalLayout = new QVBoxLayout(CheckUpdate);
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(10, 25, 10, 10);
        progressBar = new QProgressBar(CheckUpdate);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setCursor(QCursor(Qt::ArrowCursor));
        progressBar->setAutoFillBackground(false);
        progressBar->setStyleSheet(QString::fromUtf8("\n"
"\n"
" QProgressBar::chunk {\n"
"     background-color: #05B8CC;\n"
"     width: 20px;\n"
"}\n"
"\n"
"QProgressBar {\n"
"	 background-color: rgba(255, 255, 255, 0);\n"
"     border: 1px solid #50ff3d;\n"
"     border-radius: 5px;\n"
"     text-align: center;\n"
"}"));
        progressBar->setValue(0);
        progressBar->setInvertedAppearance(false);

        verticalLayout->addWidget(progressBar);

        message = new QTextBrowser(CheckUpdate);
        message->setObjectName(QString::fromUtf8("message"));
        message->setAutoFillBackground(true);
        message->setStyleSheet(QString::fromUtf8(""));
        message->setFrameShape(QFrame::Panel);
        message->setFrameShadow(QFrame::Plain);
        message->setLineWidth(0);

        verticalLayout->addWidget(message);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cancel = new QPushButton(CheckUpdate);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setMinimumSize(QSize(61, 31));
        cancel->setMaximumSize(QSize(61, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Adobe \351\273\221\344\275\223 Std R"));
        cancel->setFont(font);
        cancel->setCursor(QCursor(Qt::PointingHandCursor));
        cancel->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));
        cancel->setFlat(true);

        horizontalLayout->addWidget(cancel);

        abort = new QPushButton(CheckUpdate);
        abort->setObjectName(QString::fromUtf8("abort"));
        abort->setMinimumSize(QSize(61, 31));
        abort->setMaximumSize(QSize(61, 31));
        abort->setFont(font);
        abort->setCursor(QCursor(Qt::PointingHandCursor));
        abort->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));
        abort->setFlat(true);

        horizontalLayout->addWidget(abort);

        yes = new QPushButton(CheckUpdate);
        yes->setObjectName(QString::fromUtf8("yes"));
        yes->setMinimumSize(QSize(61, 31));
        yes->setMaximumSize(QSize(61, 31));
        yes->setFont(font);
        yes->setCursor(QCursor(Qt::PointingHandCursor));
        yes->setStyleSheet(QString::fromUtf8("border-radius:10px;\n"
"background-color: rgb(188, 250, 109);"));
        yes->setFlat(true);

        horizontalLayout->addWidget(yes);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CheckUpdate);

        QMetaObject::connectSlotsByName(CheckUpdate);
    } // setupUi

    void retranslateUi(QDialog *CheckUpdate)
    {
        CheckUpdate->setWindowTitle(QCoreApplication::translate("CheckUpdate", "Dialog", nullptr));
        cancel->setText(QCoreApplication::translate("CheckUpdate", "\345\217\226\346\266\210", nullptr));
        abort->setText(QCoreApplication::translate("CheckUpdate", "\347\273\210\346\255\242", nullptr));
        yes->setText(QCoreApplication::translate("CheckUpdate", "\346\233\264\346\226\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CheckUpdate: public Ui_CheckUpdate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHECKUPDATE_H
