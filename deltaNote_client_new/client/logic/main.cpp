#include <QApplication>

#include <QCoreApplication>
#include "mainwindow.h"
#include "log.h"

int main(int argc, char *argv[])
{
    d_logic_info("%s", "main program start");

    QApplication a(argc, argv);
    QApplication::addLibraryPath("./lib");

    MainWindow w;
    w.show();
    return QApplication::exec();
}
