#include "../include/socket.h"
#include "../include/sqlite.h"
#include "../include/Log.h"
#include <pthread.h>

using namespace deltaNote;

#define SERVER_IP "0.0.0.0"
#define SERVER_PORT 1234

char databaseName[128];
char logFilePath[128];

char serverIP[32];
int serverPort;

FILE *pLogFile;
time_t  curTime;

enum SystemStatus {
    SystemStopped = 0,
    SystemRunning = 1
};
SystemStatus systemStatus;

void * clientHandler(void *pVoid){
    // detach self make standalone
    pthread_detach(pthread_self());
    LOG_INFO("new thread %d detach and running", (int)pthread_self())

    // define ret val
    int ret;

    // get socket fd
    auto *socket = (SocketServer *)pVoid;

    // receive pack from client
    MSG_PACK recv{};
    char *buf = (char *)&recv;
    socket->recvMsg(buf, sizeof(recv));

    // init db
    ServerSqlite sqlite = ServerSqlite(databaseName, recv.userName, recv.passwd);

    if (recv.msgOp == CreateUser) {
        // create new user
        ret = sqlite.addUser();

        MSG_PACK send{};
        send.msgState = sqlite.getSqliteOpState();

        socket->sendMsg(&send, sizeof(send));
        LOG_INFO("create user send back")
    } else if (recv.msgOp == Login) {
        // login
        ret = sqlite.loginRes();

        MSG_PACK send{};
        send.msgState = sqlite.getSqliteOpState();
        socket->sendMsg(&send, sizeof(send));
        LOG_INFO("login send back")
    } else if (recv.msgOp == Pull) {
        // get msg from server
        // do login
        ret = sqlite.loginRes();
        if (SqliteRunning == ret) {
            // select from database
            vector<MSG_OP_PACK> retDataPack;
            ret = sqlite.returnDataSet(retDataPack);

            // send result to client
            int sendSize = retDataPack.size();
            MSG_PACK synPack{};
            for (int index = 0; index < sendSize;) {
                int left = min(5, sendSize - index);
                makeSocketPack(synPack,
                        left,
                        ((left == 5) && (sendSize - index != 5))? MSG_HALF:MSG_FULL,
                        RET);
                for (int i = 0; i < 5 && index < sendSize; ++i, ++index) {
                    makeDataPack(synPack.msgQueue[i],
                                 retDataPack[index].opTimestamp,
                                 retDataPack[index].createTimestamp,
                                 retDataPack[index].op,
                                 retDataPack[index].isCheck,
                                 retDataPack[index].data);
                }
                socket->sendMsg(&synPack, sizeof(synPack));
            }
            LOG_INFO("send server data back")
        }
    } else if (recv.msgOp == Push) {
        // push msg to server
        ret = sqlite.loginRes();
        vector<MSG_OP_PACK> changePush;
        if (SqliteRunning == ret){
            do {
                for (int i = 0; i < recv.msgSize; ++i) {
                    MSG_OP_PACK opPack{};
                    makeDataPack(opPack,
                                 recv.msgQueue[i].opTimestamp,
                                 recv.msgQueue[i].createTimestamp,
                                 recv.msgQueue[i].op,
                                 recv.msgQueue[i].isCheck,
                                 recv.msgQueue[i].data);
                    changePush.push_back(opPack);
                }

                if (recv.msg_seg != MSG_FULL) {
                    socket->recvMsg(buf, sizeof(MSG_PACK));
                } else {
                    break;
                }
            } while (true);
        }

        // add change to sql
        ret = sqlite.addChange(changePush);

        MSG_PACK send{};
        if (SqliteRunning == ret){
            send.msgState = PushSuccess;
        } else {
            send.msgState = PushError;
        }
        socket->sendMsg((void *)&send, sizeof(send));
        LOG_INFO("push to server change success")
    } else if (recv.msgOp == Delete){
        // clean user's db
        ret = sqlite.loginRes();
        if (SqliteRunning == ret){
            ret = sqlite.cleanSqlite();
        }

        MSG_PACK send{};
        if (SqliteRunning == ret){
            send.msgState = CleanSuccess;
        } else {
            send.msgState = CleanError;
        }
        socket->sendMsg((void *)&send, sizeof(send));
        LOG_INFO("clear server data success")
    }else {
        LOG_ERROR("Wrong Msg OP %d received", recv.msgOp)
    }

    delete socket;
    LOG_INFO("thread is exiting");
    pthread_exit(0);
}
void RunAPP(const char *server, int port){
    LOG_INFO("APP start: server:%s port:%d", server, port)

    // init system status
    systemStatus = SystemRunning;
    int ret;

    // socket init
    SocketServer socket(server, port);

    while(true) {
        if (SystemStopped == systemStatus){
            LOG_INFO("System break and ready stopped")
            break;
        }
        auto *new_socket = new SocketServer(socket);
        new_socket->acceptConn();

        pthread_t thread_handles;
        ret = pthread_create(&thread_handles, nullptr, clientHandler, (void *)new_socket);
        if(0 == ret){
            LOG_INFO("Create new thread success")
        }else{
            LOG_ERROR("create new thread fail")
        }
    }
    // clean all
    socket.closeServer();

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