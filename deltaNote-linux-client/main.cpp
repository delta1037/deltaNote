#include <QApplication>
#include "mainwindow.h"

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
    if(argc > 1){
        memset(dbPATH, 0, PATH_SIZE);
        memset(logPATH, 0, PATH_SIZE);
        strcpy(dbPATH, argv[1]);
        strcat(dbPATH, "/data/localDB");

        strcpy(logPATH, argv[1]);
        strcat(logPATH, "/log/deltaNote.log");

        LOG_INFO("dbPath:%s", dbPATH)
        LOG_INFO("logPath:%s", logPATH)
    }

    QApplication a(argc, argv);

    initClient();

    MainWindow w;
    w.show();
    return a.exec();
}
