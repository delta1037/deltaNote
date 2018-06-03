//
// Created by geniusrabbit on 18-2-27.
//

#ifndef GENIUSNOTE1_0_CLIENT_SOCKETCLIENT_H
#define GENIUSNOTE1_0_CLIENT_SOCKETCLIENT_H

#include <string>
#include <netdb.h>
#include <sys/socket.h>

#define SOCKET_ERROR (-1)

enum SocketState {
  Running=1,
  Error=3
};

namespace GeniusNoteSocket{
class SocketClient{
 public:
  int Init(char* ClientIP,int ClientPort,char* ServerIP,int ServerPort);
  int Send(void* buf,size_t size);
  int Recv(void* buf,size_t size);
  int Close();
 private:
  SocketState state;
  int serSock;
  int cliSock;
};
}

#endif //GENIUSNOTE1_0_CLIENT_SOCKETCLIENT_H
