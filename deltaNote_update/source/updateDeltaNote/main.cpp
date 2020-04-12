#include "mainwindow.h"
#include <QApplication>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QString currentPath = QCoreApplication::applicationDirPath();
    QString qLogPath = currentPath + "/log/deltaNote.log";

    char logPath[1024]{};
    strcpy(logPath, qLogPath.toLatin1().data());

    /* 设置日志显示配置 */
    std::string logFile(logPath);
    LogManage::instance()->setOutput(OUTPUT_FILE);
    LogManage::instance()->setLogFileName(logFile);
    LogManage::instance()->setLoglevel(LOG_INFO);

    LogCtrl::info("logPath:%s", logPath);

    QRect desktopRect = QGuiApplication::screens()[0]->availableGeometry();

    w.move(desktopRect.width()/2 - 90, desktopRect.height()/2 - 50);
    w.show();

    return a.exec();
}
