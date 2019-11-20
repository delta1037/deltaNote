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
/*
int test(){
    ServerSqlite sqlite = ServerSqlite("deltaNoteDB", "noteUserTable", "root", "root");

    sqlite.addUser();

    char *s_note1 = (char *)"999999";
    char *op_add = (char *)"0";
    char *note1_val = (char *)"this is a note";
    vector<OpChange *> test;
    OpChange note1 = OpChange(s_note1, s_note1, op_add, note1_val);
    test.push_back(&note1);

    sqlite.addChange(test);

    vector<OpChange*> res;

    sqlite.synChange((char *)"000000", res);

    printf("%s, %d", res[0]->data, res.size());

    return 0;
}
*/

void * clientHandler(void *pVoid){
    auto *socket = (SocketServer *)pVoid;
    int ret = SqliteRunning;
    MSG recv{};
    char *buf = (char *)&recv;
    socket->recvMsg(buf, sizeof(recv));

    ServerSqlite sqlite = ServerSqlite(databaseName, usersTableName, recv.userName, recv.passwd);

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
            vector<OpChange *> selectRes;
            ret = sqlite.synChange(recv.timestamp, selectRes);

            // send result to client
            int sendSize = selectRes.size();
            MSG send{};
            for(int index = 0; index < sendSize; ++index){
                int i = 0;
                for(; i < 5 && index < sendSize; ++i, ++index){
                    strcpy(send.msgQueue[i].opTimestamp, selectRes[index]->opTimestamp);
                    strcpy(send.msgQueue[i].createTimestamp, selectRes[index]->createTimestamp);
                    send.msgQueue[i].op = selectRes[index]->op;
                    send.msgQueue[i].isCheck = selectRes[index]->isCheck;
                    strcpy(send.msgQueue[i].data, selectRes[index]->data);
                }
                if(index < sendSize){
                    send.msgSize = 5;
                }else{
                    send.msgSize = i;
                }

                socket->sendMsg((void *)&send, sizeof(send));
            }
            LOG_INFO("send message")
        }
    } else if (recv.msgOp == Push) {
        ret = sqlite.loginRes();
        if (SqliteRunning == ret){
            do {
                vector<OpChange *> changePush;
                for (int i = 0; i < recv.msgSize; ++i) {
                    auto *t = new OpChange(recv.msgQueue[i].opTimestamp, recv.msgQueue[i].createTimestamp,
                                           recv.msgQueue[i].op, recv.msgQueue[i].isCheck, recv.msgQueue[i].data);
                    changePush.push_back(t);
                }
                ret = sqlite.addChange(changePush);

                if (recv.msg_seg != MSG_FULL) {
                    socket->recvMsg(buf, sizeof(MSG));
                } else {
                    break;
                }
            } while (true);
        }
        MSG send{};
        if (SqliteRunning == ret){
            send.msgState = PushSuccess;
        } else {
            send.msgState = PushError;
        }
        socket->sendMsg((void *)&send, sizeof(send));
    } else {
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