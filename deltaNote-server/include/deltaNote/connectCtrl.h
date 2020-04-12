#ifndef DELTA_NOTE_CONNECT_CONTROL
#define DELTA_NOTE_CONNECT_CONTROL

#include <string>

#include "../common/socket.h"
#include "../common/blackList.h"

#include "sqliteCtrl.h"
#include "packCtrl.h"

class ConnectControl {
public:
    // init server
    static bool initServerConnect(const char *serverIP, int serverPort);

public:
    ConnectControl();
    ~ConnectControl();

    bool acceptNewConnect();            // recv a new connect
    void processingClientRequest();     // processing client request
    
    void loginToServer();               // processing : client login to server 
    void createNewUser();               // processing : client create new user
    void loadFromServer();              // processing : client load data from server
    void uploadToServer();              // processing : client upload data to server
    void getUpdateLink();

    std::string getClientIPAddr();

private:
    char clientIPAddr[CLIENT_IP_ADDR_SIZE];

    MsgPack msgPack;
    PackCtrlFactory *socketServer;      // socket control

    SqliteControl   *dataControl;       // database control
};
#endif // DELTA_NOTE_CONNECT_CONTROL