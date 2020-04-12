#include <cstring>
#include <string>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <algorithm>

#include "sqlite.h"
#include "../../include/sqliteTools/cmdProcess.h"
#include "../../include/sqliteTools/sqliteCtrl.h"

// show users;
// show table user;
// exit

char *convertTime(char time[]){
    time_t timeValue = atoll(time);
    struct tm *localTime = localtime(&timeValue);
    char *timeString = new char[100]();
    sprintf(timeString, "%d-%02d-%02d %02d:%02d:%02d",  \
                    localTime->tm_year + 1900,          \
                    localTime->tm_mon + 1,              \
                    localTime->tm_mday,                 \
                    localTime->tm_hour,                 \
                    localTime->tm_min,                  \
                    localTime->tm_sec);                 \
    return timeString;
}

char *getOperation(char operation){
    char *operate = new char[10];
    if(operation == '1'){
        strcpy(operate, "ADD");
    }else if(operation == '2'){
        strcpy(operate, "DEL");
    }else if(operation == '3'){
        strcpy(operate, "ALTER");
    }else if(operation == '4'){
        strcpy(operate, "CHECK");
    }else{
        strcpy(operate, "NULL");
    }
    return operate;
}

static bool recordSort(const SocketMsgOpPack &a, const SocketMsgOpPack &b){
    return strcmp(a.operateTimestamp, b.operateTimestamp) > 0;
}

static bool opRecordSort(const SocketMsgOpPack &a, const SocketMsgOpPack &b){
    int ret = strcmp(a.createTimestamp, b.createTimestamp);
    if(ret > 0){
        return true;
    }else if(ret == 0){
        return strcmp(a.operateTimestamp, b.operateTimestamp) > 0;
    }else{
        return false;
    }
}

int processCmd(){
    SqliteControl sqliteControl;
    do{
        std::cin.clear();
        std::cout << "> ";
        std::string first_cmd;
        std::cin >> first_cmd;
        if(isExit(first_cmd)){
            break;
        }else if(isShow(first_cmd)){
            std::string second_cmd;
            std::cin >> second_cmd;
            if(isUsers(second_cmd)){
                // show users
                std::vector<struct UserID> userInfos;
                sqliteControl.findAllUser(userInfos);

                printf("| %12s | %32s |\n", "username", "password");
                for(const auto &it : userInfos){
                    printf("| %12s | %32s |\n", it.userName, it.passWord);
                }
            }else if(isTable(second_cmd)){
                // show user table
                std::string username;
                std::cin >> username;

                std::vector<struct SocketMsgOpPack> todoRecords;
                sqliteControl.loadTodoRecords(username.c_str(), todoRecords, false);
                std::sort(todoRecords.begin(), todoRecords.end(), recordSort);
                
                printf("| %19s | %19s | %7s | %s\n", "createTime", "operateTime", "isCheck", "data");
                for(auto &it : todoRecords){
                    char *createTime = convertTime(it.createTimestamp);
                    char *operateTime = convertTime(it.operateTimestamp);
                    //printf("c:%s o:%s\n", it.createTimestamp, it.operateTimestamp);
                    
                    printf("| %19s | %19s | %7s | %s\n", createTime, operateTime, it.isCheck == '1'? "Checked" : "UnCheck", it.data);
                }
                printf("data record size:%d\n", todoRecords.size());

                std::vector<struct SocketMsgOpPack> todoOpRecords;
                sqliteControl.loadTodoRecords(username.c_str(), todoOpRecords, true);
                std::sort(todoOpRecords.begin(), todoOpRecords.end(), opRecordSort);
                
                printf("| %19s | %19s | %7s | %5s | %s\n", "createTime", "operateTime", "isCheck", "op", "data");
                for(auto &it : todoOpRecords){
                    char *createTime = convertTime(it.createTimestamp);
                    char *operateTime = convertTime(it.operateTimestamp);
                    char *operation = getOperation(it.operation);

                    //printf("c:%s o:%s\n", it.createTimestamp, it.operateTimestamp);
                    
                    printf("| %19s | %19s | %7s | %5s | %s\n", createTime, operateTime, it.isCheck == '1'? "Checked" : "UnCheck", operation, it.data);
                }
                printf("op record size:%d\n", todoOpRecords.size());
            }else{
                //LOG_ERROR("error cmd : %d", second_cmd.c_str())
            }
        }else{
            //LOG_ERROR("error cmd : %d", first_cmd.c_str())
        }
        std::cout << std::endl;
    }while(true);

    //LOG_INFO("quit.")
    return 0;
}

int main(int argc, char* argv[]){
    char databaseName[1024];
    if(argc > 1){
        strcpy(databaseName, argv[1]);
        //LOG_INFO("db path:%s", databaseName)
    }else{
        //LOG_ERROR("need databse path")
        return -1;
    }

    if(!SqliteControl::initControlEnv(databaseName)){
        //LOG_ERROR("server database init error, dbName: %s", databaseName)
        return -1;
    }

    processCmd();
}
