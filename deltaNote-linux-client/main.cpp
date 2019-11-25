#include <QApplication>
#include "mainwindow.h"

void initClient(){
    ClientSqlite sqlite;
    sqlite.initSetting();
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    initClient();

    MainWindow w;
    w.show();
    return a.exec();
}
