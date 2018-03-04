//
// Created by geniusrabbit on 18-2-27.
//

#include "../include/SocketServer.h"
#include "../include/Log.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

namespace GeniusNote {
int SocketServer::init(int port) {
  LOG_INFO("Server init")
  int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  CHECK(sockfd, SOCKET_ERROR, { LOG_ERROR("Server socket init failed")state = Error; })

  struct sockaddr_in serv_addr;
  memset(&serv_addr,0, sizeof(serv_addr));

  serv_addr.sin_family=AF_INET;
  serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
  serv_addr.sin_port=htons(static_cast<uint16_t>(port));

  this->port=port;
  this->sockfd=sockfd;
  this->ServerAddr=serv_addr;

  int flag=bind(sockfd,(
  struct sockaddr*)&serv_addr, sizeof(sockaddr));

  CHECK(flag,SOCKET_ERROR,{LOG_ERROR("Server bind error") state=Error;})

  state=Running;

  return state;
}
int SocketServer::StartSocket() {
  state=Running;
  LOG_INFO("Start Listen")
  int flag=listen(this->sockfd,this->port);
  CHECK(flag,SOCKET_ERROR,{LOG_ERROR("Start(Listen) error") state=Error;})
  return state;
}

int SocketServer::AcCon() {
  state=Running;
  LOG_INFO("Connect...")
  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_size= sizeof(clnt_addr);

  int clientfd=accept(this->sockfd,(
  struct sockaddr*)&clnt_addr,&clnt_addr_size);

  CHECK(clientfd,SOCKET_ERROR,{LOG_ERROR("Accept fault") state=Error;})

  return state;

}

int SocketServer::Send(void *buf, size_t size) {
  LOG_INFO("Send message...")
  ssize_t sendSize=send(this->sockfd,buf,size,0);
  CHECK(sendSize,0,{LOG_ERROR("Send Size is NULL") state=Error;})

  return (int)sendSize;
}
int SocketServer::Recv(void *buf, size_t size) {
  LOG_INFO("Receive message...")
  ssize_t recvSize=recv(this->sockfd,buf,size,0);
  CHECK(recvSize,0,{LOG_ERROR("Receive Size is NULL") state=Error;})

  return (int)recvSize;
}
int SocketServer::Close() {
  LOG_INFO("Close Socket")
  close(this->sockfd);
  return 1;
}
}
