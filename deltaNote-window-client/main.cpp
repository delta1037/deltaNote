#include <QApplication>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QScreen>
#include "mainwindow.h"

#pragma comment( lib, "ws2_32.lib" ) // linker must use this lib for sockets

char dbPATH[PATH_SIZE];
char logPATH[PATH_SIZE];
FILE *pLogFile;
time_t  curTime;

void initClient(){
    ClientSqlite sqlite;
    sqlite.initSetting();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::addLibraryPath("./lib");

    memset(dbPATH, 0, PATH_SIZE);
    memset(logPATH, 0, PATH_SIZE);
    QString currentPath = QCoreApplication::applicationDirPath();
    QString dbPath = currentPath + "/data/localDB";
    QString logPath = currentPath + "/log/deltaNote.log";
    strcpy(dbPATH, dbPath.toLatin1().data());
    strcpy(logPATH, logPath.toLatin1().data());

    LOG_INFO("dbPath:%s", dbPATH)
    LOG_INFO("logPath:%s", logPATH)

    desktop = QGuiApplication::screens()[0]->availableGeometry();
    initClient();

    MainWindow w;
    w.show();
    return a.exec();
}
