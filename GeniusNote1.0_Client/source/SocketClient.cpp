//
// Created by geniusrabbit on 18-2-27.
//

#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#include "../include/SocketClient.h"
#include "../include/Log.h"

namespace GeniusNoteSocket{
int SocketClient::Init(char* ClientIP,int ClientPort,char* ServerIP,int ServerPort){

  LOG_INFO("Client socket start success~")
  state=Running;
  int flag;

  int cliSock=socket(AF_INET,SOCK_STREAM,0);
  this->cliSock=cliSock;
  CHECK(cliSock,SOCKET_ERROR,{LOG_ERROR("Client Create socket error") state=Error;})

  struct sockaddr_in servAddr;
  memset(&servAddr,0, sizeof(servAddr));

  servAddr.sin_family=AF_INET;
  servAddr.sin_port=htons(static_cast<uint16_t>(ServerPort));
  servAddr.sin_addr.s_addr = inet_addr(ServerIP);

  flag=connect(cliSock,(const struct sockaddr*)&servAddr,(socklen_t) sizeof(servAddr));
  CHECK(flag,SOCKET_ERROR,{LOG_ERROR("Connect Error...") state=Error;})

  LOG_INFO("Client init success~")
  return state;
}
int SocketClient::Send(void *buf, size_t size) {
  LOG_INFO("Send message...")
  ssize_t sendSize=write(this->cliSock,buf,size);
  CHECK(sendSize,0,{LOG_ERROR("Send Size is NULL") state=Error;})
  return (int)sendSize;
}
int SocketClient::Recv(void *buf, size_t size) {
  LOG_INFO("Receive message...")
  ssize_t recvSize=recv(this->serSock,buf,size,0);
  CHECK(recvSize,0,{LOG_ERROR("Receive Size is NULL") state=Error;})
  return (int)recvSize;
}
int SocketClient::Close() {
  LOG_INFO("Close socket~")
  close(this->serSock);
  close(this->cliSock);
  return 1;
}
}
