#include <QApplication>

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

        d_logic_info("%s", "get quit signal, save all todo");
        qApp->quit();
    }
}
/* 捕获Linux重启时的信号，用于保存数据 */
#endif // LINUX_CLIENT

#if 0
void initTodoList(){
    DataCtrl dataCtrl;
    // get data list
    // warning:会导致TODO重复发送
    std::vector<SocketMsgOpPack> listBuffer;
    dataCtrl.loadLocalList(listBuffer);
    d_logic_info("deltaNote start, load all todo, size:%d", listBuffer.size());
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
#endif

int main(int argc, char *argv[])
{
    d_logic_info("%s", "main program start");

    QApplication a(argc, argv);
    QApplication::addLibraryPath("./lib");
#if 0
    /* 初始化设置 */
    d_logic_info("%s", "init setting");
    initSetting();

    /* 初始化显示列表 */
    d_logic_info("%s", "init list");
    initTodoList();
#endif
    MainWindow w;
#ifdef LINUX_CLIENT
    /* 注册Linux系统重启时退出信号 */
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    d_logic_error("%s", "why define signal handle")
#endif
    w.show();
    return QApplication::exec();
}
