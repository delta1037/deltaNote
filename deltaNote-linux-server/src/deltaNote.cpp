#include "../include/socket.h"
#include "../include/sqlite.h"
#include "../include/Log.h"
#include <pthread.h>
#include <vector>

#define SERVER_IP "0.0.0.0"
#define SERVER_PORT 1234

char databaseName[PATH_SIZE];
char logFilePath[PATH_SIZE];

char serverIP[32];
int serverPort;

FILE *pLogFile;
time_t  curTime;

enum SystemStatus {
    SystemStopped = 0,
    SystemRunning = 1
};
SystemStatus systemStatus;

using namespace std;

void * clientHandler(void *pVoid){
    // detach self make standalone
    pthread_detach(pthread_self());
    LOG_INFO("new thread %d detach and running", (int)pthread_self())
    if(pVoid != nullptr){
        ServerConnectControl testClass;
        auto *serverControl = (ServerConnectControl *)pVoid;
        if(typeid(testClass) == typeid(*serverControl)){
            serverControl->processingClientRequest();
            LOG_INFO("thread is exiting");
            delete serverControl;
            serverControl = nullptr;
        }else{
            serverControl = nullptr;
            LOG_ERROR("pointer is error")
        }
    }else{
        LOG_ERROR("multi thread error")
    }
    pthread_exit(nullptr);
}
void RunAPP(const char *server, int port){
    LOG_INFO("APP start: server:%s port:%d", server, port)

    // init system status
    systemStatus = SystemRunning;

    // database init
    if(!ServerDataControl::initControlEnv(databaseName)){
        LOG_ERROR("server database init error, dbName: %s", databaseName)
        return;
    }
    // socket init
    if(!ServerConnectControl::initServerConnect(server, port)){
        LOG_ERROR("server socket init error")
        return;
    }

    // 存储已建立的连接
    vector<pair<long long, ServerConnectControl *>> controlQueue;
    while(true) {
        if (SystemStopped == systemStatus){
            LOG_INFO("System break and ready stopped")
            break;
        }

        auto *serverControl = new ServerConnectControl();
        if(!serverControl->acceptNewConnect()){
            delete serverControl;
            continue;
        }

        controlQueue.push_back(make_pair(std::time(nullptr), serverControl));
        pthread_t thread_handles;
        int ret = pthread_create(&thread_handles, nullptr, clientHandler, (void *)serverControl);
        if(0 == ret){
            LOG_INFO("Create new thread success")
        }else{
            LOG_ERROR("create new thread fail")
        }

        for(int i = controlQueue.size() - 1; i >= 0; --i){
            // 超过六十秒清理掉
            if(std::time(nullptr) - controlQueue[i].first > 60){
                controlQueue.erase(controlQueue.begin() + i);
            }
        }
    }
    // clean all
    LOG_INFO("APP is stopped")
}
int main(int argc, char* argv[]){
    if(argc > 1){
        strcpy(databaseName, argv[1]);
        strcat(databaseName, "/data/deltaNoteServerDB");

        strcpy(logFilePath, argv[1]);
        strcat(logFilePath, "/log/deltaNote.log");

        strcpy(serverIP, argv[2]);
        serverPort = atoi(argv[3]);
        LOG_INFO("log path:%s", logFilePath);
        LOG_INFO("db path:%s", databaseName);
        RunAPP(serverIP, serverPort);
    }else{
        strcpy(databaseName, "deltaNoteServerDB");
        strcpy(logFilePath, "deltaNote.log");
        RunAPP("0.0.0.0", 8888);
        LOG_ERROR("app start fail with wrong para")
    }
    return 0;
}