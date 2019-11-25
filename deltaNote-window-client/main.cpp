#include <QApplication>
#include "mainwindow.h"

#pragma comment( lib, "ws2_32.lib" ) // linker must use this lib for sockets

void initClient(){
    ClientSqlite sqlite;
    sqlite.initSetting();
}
int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./lib");
    QApplication a(argc, argv);

    initClient();

    MainWindow w;
    w.show();
    return a.exec();
}
