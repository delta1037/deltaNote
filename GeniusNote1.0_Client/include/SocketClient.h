//
// Created by geniusrabbit on 18-2-27.
//

#ifndef GENIUSNOTE1_0_CLIENT_SOCKETCLIENT_H
#define GENIUSNOTE1_0_CLIENT_SOCKETCLIENT_H

#include <string>
#include <netdb.h>
#include <sys/socket.h>

#define CHECK(x,m,handle) if((x)==(m)){\
                           handle;\
                           return -1;\
                         }
#define SOCKET_ERROR (-1)

enum SocketState {
  Uninitialized=0,
  Running=1,
  Stopped=2,
  Error=3
};

namespace GeniusNoteSocket{
class SocketClient{
 public:
  int init(int port);
  int ConSer(const std::string& serverIPStr, int serverPort);
  int Send(void* buf,size_t size);
  int Recv(void* buf,size_t size);
  int Close();
 private:
  SocketState state;
  int port;
  int sockfd;
  sockaddr_in CliAddr;

};
}

#endif //GENIUSNOTE1_0_CLIENT_SOCKETCLIENT_H
