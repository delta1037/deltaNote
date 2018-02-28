//
// Created by geniusrabbit on 18-2-27.
//

#include <arpa/inet.h>
#include <cstring>
#include <zconf.h>

#include "../include/SocketClient.h"
#include "../include/Log.h"

namespace GeniusNoteSocket{
int SocketClient::init(int port) {
  LOG_INFO("Client Socket init")
  state=Running;
  in_addr_t Client_IP=inet_addr("0.0.0.0");
  int sockfd=socket(AF_INET,SOCK_STREAM,0);
  CHECK(sockfd,SOCKET_ERROR,{LOG_ERROR("Client Create socket error") state=Error;})

  struct sockaddr_in cliSockAddr;
  memset(&cliSockAddr,0, sizeof(cliSockAddr));

  cliSockAddr.sin_family=AF_INET;
  cliSockAddr.sin_port=htons(static_cast<uint16_t>(port));
  cliSockAddr.sin_addr.s_addr = Client_IP;

  LOG_INFO("Bind Socket...")

  int flag=bind(sockfd,(sockaddr*)&cliSockAddr, sizeof(cliSockAddr));
  CHECK(flag,SOCKET_ERROR,{LOG_ERROR("Bind Socket error")state=Error;})

  this->port=port;
  this->sockfd=sockfd;
  this->CliAddr=cliSockAddr;
  return state;
}
int SocketClient::ConSer(const std::string& serverIPStr, int serverPort) {
  LOG_INFO("Connect Server...")
  in_addr_t serverIP =inet_addr(serverIPStr.c_str());

  struct sockaddr_in server;
  memset(&server,0, sizeof(server));

  server.sin_family=AF_INET;
  server.sin_port=htons(static_cast<uint16_t>(serverPort));

  int flag=connect(this->sockfd,(const struct sockaddr*)&server,(socklen_t) sizeof(server));
  CHECK(flag,SOCKET_ERROR,{LOG_ERROR("Connect Error...") state=Error;})

  return state;
}
int SocketClient::Send(void *buf, size_t size) {
  LOG_INFO("Send message...")
  ssize_t sendSize=send(this->sockfd,buf,size,0);
  CHECK(sendSize,0,{LOG_ERROR("Send Size is NULL") state=Error;})
  return (int)sendSize;
}
int SocketClient::Recv(void *buf, size_t size) {
  LOG_INFO("Receive message...")
  ssize_t recvSize=recv(this->sockfd,buf,size,0);
  CHECK(recvSize,0,{LOG_ERROR("Receive Size is NULL") state=Error;})
  return (int)recvSize;
}
int SocketClient::Close() {
  LOG_INFO("Close socket~")
  close(this->sockfd);
  return 1;
}
}
