#include <QApplication>
#include "mainwindow.h"

// main
//  mainwindow
//   login
//    newuser
//     untils
//      pack
//   userinfo
//    untils
//      pack

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
