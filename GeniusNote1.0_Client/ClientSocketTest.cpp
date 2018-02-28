//
// Created by geniusrabbit on 18-2-27.
//

#include <cstring>
#include "include/SocketClient.h"
//#include "include/Log.h"
//#include <iostream>

typedef struct {
  int id;
  char name[8];
}ID_Name;

using namespace GeniusNoteSocket;

int main(){
  SocketClient client{};
  int flag=0;
  size_t StructSize= sizeof(ID_Name);

  char* buf=(char*)malloc(sizeof(ID_Name));

  ID_Name* newNode=(ID_Name*)malloc(sizeof(ID_Name));
  newNode->id=666;
  strcpy(newNode->name,"lll");

  memcpy(buf,newNode, StructSize);

  /*
  ID_Name* newNode1=(ID_Name*)malloc(sizeof(ID_Name));
  memcpy(newNode1,buf, sizeof(ID_Name));
  printf("%d--%s\n",newNode1->id,newNode1->name);

  */
  //fgets((void*)&buf, sizeof(stdin),stdin);

  int init=client.init(2336);
  int con=client.ConSer("127.0.0.1",2332);

  int sendSize=client.Send(buf, StructSize);
  memset(buf,0, StructSize);
  int recvSize=client.Recv(buf, StructSize);
  memcpy(newNode,buf, StructSize);

  printf("%d--%s\n",newNode->id,newNode->name);

  int clo=client.Close();

}


