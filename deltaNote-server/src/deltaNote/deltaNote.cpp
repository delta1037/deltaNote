#include <pthread.h>
#include <vector>

#include "../../include/deltaNote/connectCtrl.h"
#include "../../include/deltaNote/sqliteCtrl.h"

/**
 * 日志与黑名单位置信息
 */
static char logFilePath[PATH_SIZE];
static char dbFilePath[PATH_SIZE];
static char blacklistFilePath[PATH_SIZE];
static char blacklistTmpFilePath[PATH_SIZE];

/**
 * 服务器信息
 */
static char serverIP[32];
static int serverPort;

enum SystemStatus {
    SystemStopped = 0,
    SystemRunning = 1
};
SystemStatus systemStatus;

using namespace std;

void * clientHandler(void *pVoid){
    // detach self make standalone
    pthread_detach(pthread_self());
    LogCtrl::debug("new thread %d detach and running", (int)pthread_self());

    if(pVoid != nullptr){
        ConnectControl ConnectControlClass;
        auto *serverControl = (ConnectControl *)pVoid;
        if(typeid(ConnectControlClass) == typeid(*serverControl)){
            serverControl->processingClientRequest();
            delete serverControl;
            serverControl = nullptr;
        }else{
            serverControl = nullptr;
            LogCtrl::error("pointer is error");
        }
    }else{
        LogCtrl::error("thread null param");
    }

    LogCtrl::debug("thread is exited");
    pthread_exit(nullptr);
}
void RunAPP(const char server[], int port){
    LogCtrl::info("APP start: server:%s port:%d", server, port);

    // init system status
    systemStatus = SystemRunning;

    // database init
    if(!SqliteControl::initControlEnv(dbFilePath)){
        LogCtrl::error("server database init error, dbName: %s", dbFilePath);
        return;
    }
    // socket init
    if(!ConnectControl::initServerConnect(server, port)){
        LogCtrl::error("server socket init error");
        return;
    }

    // 存储已建立的连接
    vector<pair<long long, ConnectControl *>> controlQueue;
    while(true) {
        if (SystemStopped == systemStatus){
            LogCtrl::info("System break and ready stopped");
            break;
        }

        auto *serverControl = new ConnectControl();
        if(!serverControl->acceptNewConnect()){
            // 处理异常连接,添加到黑名单中
            std::string blackIP = serverControl->getClientIPAddr();
            BlacklistControl::instance()->addBlacklistItem(blackIP);
            LogCtrl::error("accept:%s error, add to blacklist", blackIP.c_str());
            delete serverControl;
            continue;
        }

        // 如果在黑名单中，拒绝连接
        std::string blackIP = serverControl->getClientIPAddr();
        if (BlacklistControl::instance()->inBlacklist(blackIP)){
            LogCtrl::info("ip %s is in black list", blackIP.c_str());
            delete serverControl;
            continue;
        }

        // 连接成功，开启线程进行处理
        // 存储到处理队列，之后对超时的连接进行销毁
        controlQueue.emplace_back(std::time(nullptr), serverControl);
        pthread_t thread_handles;
        int ret = pthread_create(&thread_handles, nullptr, clientHandler, (void *)serverControl);
        if(0 != ret){
            LogCtrl::error("create new thread fail");
        }

        // 清理超时连接 - 一分钟
        for(int i = (int)controlQueue.size() - 1; i >= 0; --i){
            // 超过六十秒清理掉
            if(std::time(nullptr) - controlQueue[i].first > 60){
                LogCtrl::debug("clean control queue index：%d", i);
                //if(controlQueue[i].second != nullptr){
                //    delete controlQueue[i].second;
                //}
                controlQueue.erase(controlQueue.begin() + i);
            }
        }
    }

    // clean all
    LogCtrl::info("APP is stopped");
}
int main(int argc, char* argv[]){
    if(argc > 1){
        strcpy(logFilePath, argv[1]);
        strcat(logFilePath, "/log/deltaNote");

        strcpy(dbFilePath, argv[1]);
        strcat(dbFilePath, "/data/deltaNoteServerDB");

        strcpy(blacklistFilePath, argv[1]);
        strcat(blacklistFilePath, "/data/.blacklist");

        
        strcpy(blacklistTmpFilePath, argv[1]);
        strcat(blacklistTmpFilePath, "/data/.blacklistTmp");

        strcpy(serverIP, argv[2]);
        serverPort = atoi(argv[3]);
    }else{
        strcpy(logFilePath, "defaultLog");
        strcpy(dbFilePath, "defaultDatabase");
        strcpy(blacklistFilePath, ".defaultBlacklist");
        strcpy(blacklistTmpFilePath, ".defaultBlacklistTmp");
        strcpy(serverIP, "0.0.0.0");
        serverPort = 8888;
    }

    BlacklistControl::instance()->setBlacklistPath(blacklistFilePath);
    BlacklistControl::instance()->setTmpBlacklistPath(blacklistTmpFilePath);
    BlacklistControl::instance()->loadAllBlacklist();

    std::string logFile(logFilePath);
    LogManage::instance()->setOutput(OUTPUT_FILE);
    LogManage::instance()->setLogFileName(logFile);
    LogManage::instance()->setLoglevel(LOG_DEBUG);

    LogCtrl::info("log pre path:%s", logFilePath);
    LogCtrl::info("database path:%s", dbFilePath);
    LogCtrl::info("blacklist path:%s", blacklistFilePath);
    LogCtrl::info("temp blacklist path:%s", blacklistTmpFilePath);

    RunAPP(serverIP, serverPort);

    BlacklistControl::instance()->saveAllBlacklist();
    return 0;
}