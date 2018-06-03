//
// Created by geniusrabbit on 18-2-27.
//

#include "../include/SocketServer.h"
#include "../include/Log.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

namespace GeniusNote {
int SocketServer::Init(char* ClientIP,int ClientPort,char* ServerIP,int ServerPort) {
  LOG_INFO("Server start success~")
  int flag;
  int serSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  CHECK(serSock, SOCKET_ERROR, { LOG_ERROR("Server socket init failed")state = Error; })
  this->serSock=serSock;

  struct sockaddr_in servAddr;
  memset(&servAddr,0, sizeof(servAddr));

  servAddr.sin_family=AF_INET;
  servAddr.sin_port=htons(static_cast<uint16_t>(ServerPort));
  servAddr.sin_addr.s_addr=inet_addr(ServerIP);

  flag=bind(serSock,(struct sockaddr*)&servAddr, sizeof(servAddr));
  CHECK(flag,SOCKET_ERROR,{LOG_ERROR("Server bind error") state=Error;})

  flag=listen(serSock,20);
  CHECK(flag,SOCKET_ERROR,{LOG_ERROR("Start(Listen) error") state=Error;})

  struct sockaddr_in cliAddr;
  socklen_t clnt_addr_size= sizeof(cliAddr);

  int cliSock=accept(serSock,(struct sockaddr*)&cliAddr,&clnt_addr_size);
  this->cliSock=cliSock;

  CHECK(flag,SOCKET_ERROR,{LOG_ERROR("Accept fault") state=Error;})

  return state;
}
int SocketServer::Send(void *buf, size_t size) {
  LOG_INFO("Send message...")
  ssize_t sendSize=send(this->serSock,buf,size,0);
  CHECK(sendSize,0,{LOG_ERROR("Send Size is NULL") state=Error;})
  return (int)sendSize;
}
int SocketServer::Recv(void *buf, size_t size) {
  LOG_INFO("Receive message...")
  ssize_t recvSize=read(this->cliSock,buf,size);
  CHECK(recvSize,0,{LOG_ERROR("Receive Size is NULL") state=Error;})
  return (int)recvSize;
}
int SocketServer::Close() {
  LOG_INFO("Close Socket~")
  close(this->cliSock);
  close(this->serSock);
  return 1;
}
}
