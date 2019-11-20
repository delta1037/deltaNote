#include "mainwindow.h"
#include "socket.h"
#include <QApplication>

using namespace deltaNote;

char g_server[16]{};
int g_port = 0;

char g_username[8]{};
char g_passdw[12]{};

bool isLogin = false;
bool isLocked = false;

// main
//  mainwindow
//   login
//    newuser
//   userinfo

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
