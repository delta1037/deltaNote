#include <QApplication>
#include <QGuiApplication>
#include <QCoreApplication>
#include "mainwindow.h"
#include "log.h"
#ifdef LINUX_CLIENT
/* 捕获Linux重启时的信号，用于保存数据 */
#include <signal.h>
#include <unistd.h>

void signalHandler(int sig){
    if(sig == SIGINT || sig == SIGTERM){
        // 保存设置
        DataCtrl dataCtrl;
        dataCtrl.setAllSetting();

        // 保存列表数据
        std::vector<SocketMsgOpPack> listBuffer;
        for(auto &it : ListControl::getDoneList()){
            listBuffer.emplace_back(SocketMsgOpPack(it));
        }
        for(auto &it : ListControl::getUndoList()){
            listBuffer.emplace_back(SocketMsgOpPack(it));
        }

        dataCtrl.cleanLocalList();
        dataCtrl.saveLocalList(listBuffer);

        delta_note_info("%s", "get quit signal, save all todo");
        qApp->quit();
    }
}
/* 捕获Linux重启时的信号，用于保存数据 */
#endif // LINUX_CLIENT

void initTodoList(){
    DataCtrl dataCtrl;
    // get data list
    // warning:会导致TODO重复发送
    std::vector<SocketMsgOpPack> listBuffer;
    dataCtrl.loadLocalList(listBuffer);
    delta_note_info("deltaNote start, load all todo, size:%d", listBuffer.size());
    if(listBuffer.size() > 0){
        for(unsigned int i = 0; i < listBuffer.size(); ++i){
            MsgOpPack tOpPack(listBuffer[i]);
            if(i == listBuffer.size() - 1){
                ListControl::opListAdd(tOpPack);
            }else{
                ListControl::opListAdd(tOpPack);
            }

            if(listBuffer[i].isCheck == Checked){
                ListControl::doneListAdd(tOpPack);
            }else{
                ListControl::undoListAdd(tOpPack);
            }
        }
    }
}

void initDatabaseSetting(char dbPath[]){
    strcpy(DataCtrl::databaseName, dbPath);
    strcpy(DataCtrl::userLocalListTableName, "localListTable");
    strcpy(DataCtrl::userLocalOpListTableName, "localOpListTable");
    DataCtrl::initDatabse();
}

void initSetting(){              
    DataCtrl dataCtrl;
    // get all setting
    dataCtrl.getAllSetting();
}

int main(int argc, char *argv[])
{
    delta_note_info("%s", "main program start");

    QApplication a(argc, argv);
    QApplication::addLibraryPath("./lib");

    desktopRect = QGuiApplication::screens()[0]->availableGeometry();

    QString currentPath = QCoreApplication::applicationDirPath();
    QString qDBPath = currentPath + "/data/";
    QString qDBFile = qDBPath + "localDB";
    QString qLogPath = currentPath + "/log/";
    QString qLogFile = qLogPath + "deltaNote.log";

    QDir dbFileDir(qDBPath);
    if(!dbFileDir.exists()){
        dbFileDir.mkpath(qDBPath);
    }

    QDir logFileDir(qLogPath);
    if(!logFileDir.exists()){
        logFileDir.mkpath(qLogPath);
    }

    char logFile[PATH_SIZE]{};
    char dbFile[PATH_SIZE]{};
    strcpy(dbFile, qDBFile.toLatin1().data());
    strcpy(logFile, qLogFile.toLatin1().data());

    /* 初始化数据库 */
    delta_note_info("%s", "init database");
    initDatabaseSetting(dbFile);

    /* 初始化设置 */
    delta_note_info("%s", "init setting");
    initSetting();

    /* 初始化显示列表 */
    delta_note_info("%s", "init list");
    initTodoList();

    MainWindow w;
#ifdef LINUX_CLIENT
    /* 注册Linux系统重启时退出信号 */
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    delta_note_error("%s", "why define signal handle")
#endif
    w.show();
    return QApplication::exec();
}
