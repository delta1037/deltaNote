//
// Created by geniusrabbit on 18-3-4.
//

#include "../include/SqliteServer.h"
#include "../include/SocketServer.h"
#include "../include/Log.h"
#include <pthread.h>
#include <iostream>
#include <cstring>

#define NUM_THREADS 1024

using namespace GeniusNote;

typedef struct {
  int id;
  char name[8];
}ID_Name;
ID_Name Node[10];
typedef struct {
  char* UserName;
  char* Paswd;
  char* Type;
}User_Paswd;

void* buf;

int callback(void *NotUsed,int argc,char **argv,char **azColName){
  memcpy(buf,*argv, sizeof(*argv));
  return 0;
}

static void * DataHandle() {
  SocketServer Socket{};
  Socket
}
/**
 * 接收到新的连接请求
 * 建立新的线程
 * 与客户端交换数据
 * 交换完毕断开连接终止线程
 * @return
 */

int main(){
  int i=0;
  SocketServer socketServer{};

  return 0;
}