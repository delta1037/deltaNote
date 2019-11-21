#include "../include/socket.h"
#include "../include/sqlite.h"
#include "../include/Log.h"
#include <pthread.h>

using namespace deltaNote;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

#define databaseName "deltaNoteDB"
#define usersTableName "user_passwd"

enum SystemStatus {
    SystemStopped = 0,
    SystemRunning = 1
};
SystemStatus systemStatus;

void * clientHandler(void *pVoid){
    auto *socket = (SocketServer *)pVoid;
    int ret = SqliteRunning;
    MSG recv{};
    char *buf = (char *)&recv;
    socket->recvMsg(buf, sizeof(recv));

    ServerSqlite sqlite = ServerSqlite(databaseName, recv.userName, recv.passwd);

    if (recv.msgOp == CreateUser) {
        ret = sqlite.addUser();

        MSG send{};
        send.msgState = sqlite.getSqliteOpState();

        socket->sendMsg(&send, sizeof(send));
        LOG_INFO("create user send back")
    } else if (recv.msgOp == Login) {
        ret = sqlite.loginRes();

        MSG send{};
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
            MSG synPack{};
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
            LOG_INFO("send message")
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
                    socket->recvMsg(buf, sizeof(MSG));
                } else {
                    break;
                }
            } while (true);
        }

        // add change to sql
        ret = sqlite.addChange(changePush);

        MSG send{};
        if (SqliteRunning == ret){
            send.msgState = PushSuccess;
        } else {
            send.msgState = PushError;
        }
        socket->sendMsg((void *)&send, sizeof(send));
    } else if (recv.msgOp == Delete){
        ret = sqlite.loginRes();
        if (SqliteRunning == ret){
            ret = sqlite.cleanSqlite();
        }

        MSG send{};
        if (SqliteRunning == ret){
            send.msgState = CleanSuccess;
        } else {
            send.msgState = CleanError;
        }
        socket->sendMsg((void *)&send, sizeof(send));
    }else {
        LOG_ERROR("Wrong Msg OP %d received", recv.msgOp)
    }

    socket->closeClient();
    pthread_exit(nullptr);
    return nullptr;
}
void RunAPP(){
    systemStatus = SystemRunning;

    // socket init
    SocketServer socket(SERVER_IP, SERVER_PORT);

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
int main(){
    //test();

    RunAPP();
    return 0;
}