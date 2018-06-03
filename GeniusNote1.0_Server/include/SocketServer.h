//
// Created by geniusrabbit on 18-2-27.
//

#ifndef GENIUSNOTE1_0_SOCKETSERVER_H
#define GENIUSNOTE1_0_SOCKETSERVER_H

#include <string>
#include <netdb.h>
#include <sys/socket.h>


#define SOCKET_ERROR (-1)

enum SocketState {
  Uninitialized=3,
  Running=1,
  Stopped=2,
  Error=0
};

namespace GeniusNote{
class SocketServer{
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




#endif //GENIUSNOTE1_0_SOCKETSERVER_H

