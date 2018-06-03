//
// Created by geniusrabbit on 18-2-27.
//

#include <cstring>
#include "include/SocketClient.h"

using namespace std;
typedef struct{
  int id;
  char name[10];
}ID_Name;

using namespace GeniusNoteSocket;

int main(){
  SocketClient client{};
  int flag=0;
  size_t StructSize= sizeof(ID_Name);

  char buf[20]="teat";
  ID_Name* bufNode=(ID_Name*)malloc(sizeof(ID_Name));

  //生成结构体对象并拷贝到buf
  ID_Name* newNode=(ID_Name*)malloc(sizeof(ID_Name));
  newNode->id=666;
  strcpy(newNode->name,"liubo");

  memcpy(buf,&(*newNode), StructSize);
  client.Init("127.0.0.1",6666,"127.0.0.1",6667);

  int sendSize=client.Send(&buf, StructSize);
  int recvSize=client.Recv(buf, StructSize);
  memcpy(bufNode,buf,StructSize);
  printf("%d-%s\n",bufNode->id,bufNode->name);

  client.Close();
}


