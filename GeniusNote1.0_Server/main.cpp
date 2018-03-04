//
// Created by geniusrabbit on 18-3-4.
//

#include "include/SqliteServer.h"
#include "include/SocketServer.h"

#include <iostream>

using namespace GeniusNote;

typedef struct{
  int id;
  char name[8];
}ID_Name;

int callback(void *NotUsed,int argc,char **argv,char **azColName){
  for(int i=0;i<argc;i++){
    printf("%s\n",argv[i]);
  }
  return 0;
}

int Sqlite(char* UserName,char* Paswd,char* Type){

}
/**
 * 接收到新的连接请求
 * 建立新的线程
 * 与客户端交换数据
 * 交换完毕断开连接终止线程
 * @return
 */

int main(){





  return 1;
}