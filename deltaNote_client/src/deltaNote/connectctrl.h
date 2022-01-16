#ifndef CONNECTCTRL_H
#define CONNECTCTRL_H

#include "deltaNote/packCtrl.h"
#include "ui/notify.h"

class ConnectCtrl {
public:
    ConnectCtrl();
    ~ConnectCtrl();

    MsgStatus loginToServer();               // processing : client login to server
    MsgStatus createNewUser();               // processing : client create new user
    MsgStatus loadFromServer(std::vector<struct SocketMsgOpPack>&);              // processing : client load data from server
    MsgStatus uploadToServer(std::vector<struct SocketMsgOpPack>&);              // processing : client upload data to server
    MsgStatus checkUpdate(std::string&, std::string&);
    void parserServerPort(char *);
private:
    bool isIPAddr(const char*);
    bool initSocket();
    bool checkUserIDValied();

    bool inited;
    MsgPack msgPack;
    PackCtrlFactory *socketClient;      // socket control
};

#endif // CONNECTCTRL_H
