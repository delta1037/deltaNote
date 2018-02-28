//
// Created by geniusrabbit on 18-2-27.
//

#include <cstring>
#include "include/SocketServer.h"
typedef struct{
  int id;
  char name[8];
}ID_Name;
using namespace GeniusNoteSocket;

int main(){

  char* buf=(char*)malloc(sizeof(ID_Name));
  ID_Name* newNode=(ID_Name*)malloc(sizeof(ID_Name));

  SocketServer server{};

  size_t StructSize= sizeof(ID_Name);
  int init=server.init(2332);
  int start=server.StartSocket();
  int Ac=server.AcCon();
  int recvSize=server.Recv(buf, StructSize);

  memcpy(&newNode, buf,StructSize);
  printf("%d--%s\n",newNode->id,newNode->name);
  memcpy(buf,newNode, sizeof(ID_Name)+1);

  int sendSize=server.Send(buf, StructSize);
  server.Close();

}