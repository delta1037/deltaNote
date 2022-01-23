#ifndef NOTE_CLIENT_CLION_NETWORK_H
#define NOTE_CLIENT_CLION_NETWORK_H

#include "httplib.h"

enum NetType{
    Net_unknown = 0,
    Net_client,
    Net_server,
};

class Network {
public:
    Network(NetType net_type);

// TODO 服务端和网络端合一
private:

private:

};


#endif //NOTE_CLIENT_CLION_NETWORK_H
