#include <QApplication>
#include "mainwindow.h"

#pragma comment( lib, "ws2_32.lib" ) // linker must use this lib for sockets

int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./lib");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
