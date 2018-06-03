//
// Created by geniusrabbit on 18-2-27.
//

#include <cstring>
#include "include/SocketServer.h"
using namespace std;
typedef struct{
  int id;
  char name[10];
}ID_Name;
using namespace GeniusNote;

int main(){

  char buf[20];
  size_t structSize= sizeof(ID_Name);
  ID_Name* newNode=(ID_Name*)malloc(sizeof(ID_Name));

  SocketServer server{};
  int init=server.Init(const_cast<char *>("127.0.0.1"), 6666, const_cast<char *>("127.0.0.1"), 6667);

  int recvSize=server.Recv(buf, 20);
  memcpy(newNode,buf,structSize);
  printf("%d-%s\n",newNode->id,newNode->name);
  int sendSize=server.Send(buf, 20);
  server.Close();

}