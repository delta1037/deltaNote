//
// Created by geniusrabbit on 18-3-4.
//

#include "include/SqliteServer.h"
#include "include/SocketServer.h"
#include "include/Log.h"
#include <pthread.h>
#include <iostream>
#include <cstring>

#define NUM_THREADS 5

using namespace GeniusNote;

typedef struct {
  int id;
  char name[8];
}ID_Name;
typedef struct {
  char* UserName;
  char* Paswd;
  char* Type;
}User_Paswd;

void* buf;


int callback(void *NotUsed,int argc,char **argv,char **azColName){
  for(int i=0;i<argc;i++){
    printf("%s\n",argv[i]);
  }
  return 0;
}

void * SoSqTh(void *pVoid){
  SocketServer socketServer{};
  User_Paswd* user_paswd;
  int rc;
  size_t UserPaswdSize= sizeof(User_Paswd);
  if(socketServer.AcCon()==1){
    LOG_INFO("Socket is accept connection...")
    socketServer.Recv(buf, UserPaswdSize);
    memcpy(&user_paswd, buf,UserPaswdSize);

    if(strcmp(user_paswd->Type,"register")){

    }else if(strcmp(user_paswd->Type,"client")||strcmp(user_paswd->Type,"android")){

    }else{
      LOG_ERROR("身份验证失败。。。")
    }

  }else{
    LOG_ERROR("Connection fault!!!")
  }
}
/**
 * 接收到新的连接请求
 * 建立新的线程
 * 与客户端交换数据
 * 交换完毕断开连接终止线程
 * @return
 */

int main(){
  SocketServer socketServer{};

  pthread_t threads[NUM_THREADS];
  int indexes[NUM_THREADS];// 用数组来保存i的值
  int rc;
  int i;
  for( i=0; i < NUM_THREADS; i++ ){
    LOG_INFO("main() : 创建线程, ")
    indexes[i] = i; //先保存i的值
    // 传入的时候必须强制转换为void* 类型，即无类型指针
    rc = pthread_create(&threads[i], NULL,SoSqTh,(void *)&(indexes[i]));
    if (rc){
      LOG_INFO("Error:无法创建线程,")
      exit(-1);
    }
  }
  pthread_exit(NULL);
  return 1;
}