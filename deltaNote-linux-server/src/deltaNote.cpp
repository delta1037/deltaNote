#include "../include/socket.h"
#include "../include/sqlite.h"
#include "../include/Log.h"
#include <pthread.h>

using namespace deltaNote;

#define databaseName "data/deltaNoteServerDB"
#define SERVER_IP "0.0.0.0"
#define SERVER_PORT 1234

char serverIP[32];
int serverPort;

enum SystemStatus {
    SystemStopped = 0,
    SystemRunning = 1
};
SystemStatus systemStatus;

void * clientHandler(void *pVoid){
    auto *socket = (SocketServer *)pVoid;
    int ret = SqliteRunning;
    MSG_PACK recv{};
    char *buf = (char *)&recv;
    socket->recvMsg(buf, sizeof(recv));

    ServerSqlite sqlite = ServerSqlite(databaseName, recv.userName, recv.passwd);

    if (recv.msgOp == CreateUser) {
        ret = sqlite.addUser();

        MSG_PACK send{};
        send.msgState = sqlite.getSqliteOpState();

        socket->sendMsg(&send, sizeof(send));
        LOG_INFO("create user send back")
    } else if (recv.msgOp == Login) {
        ret = sqlite.loginRes();

        MSG_PACK send{};
        send.msgState = sqlite.getSqliteOpState();
        socket->sendMsg(&send, sizeof(send));
        LOG_INFO("login send back")
    } else if (recv.msgOp == Pull) {
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
    pthread_exit(nullptr);
    return nullptr;
}
void RunAPP(const char *server, int port){
    systemStatus = SystemRunning;

    // socket init
    SocketServer socket(server, port);

    while(true) {
        if (SystemStopped == systemStatus){
            break;
        }
        auto *new_socket = new SocketServer(socket);
        new_socket->acceptConn();

        pthread_t thread_handles;
        pthread_create(&thread_handles, nullptr, clientHandler, (void *)new_socket);
        pthread_detach(thread_handles);
    }

    // clean all
    socket.closeServer();
}
int main(int argc, char* argv[]){
    //test();
    if(argc > 1){
        strcpy(serverIP, argv[1]);
        serverPort = atoi(argv[2]);
        printf("Server:%s\nPort:%d\n", serverIP, serverPort);
        RunAPP(serverIP, serverPort);
    }else{
        printf("Server:%s\nPort:%d\n", SERVER_IP, SERVER_PORT);
        RunAPP(SERVER_IP, SERVER_PORT);
    }
    return 0;
}