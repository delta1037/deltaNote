#ifndef PACK_CTRL
#define PACK_CTRL

#include <vector>
#include <string>
#include <cstdint>

#include "../common/socket.h"

class PackCtrlFactory : public SocketClient {
public:
    virtual ~PackCtrlFactory();
    virtual bool msgSend(MsgPack &) = 0;
    virtual bool msgRecv(MsgPack &) = 0;
};

class DynamicPackCtrl : public PackCtrlFactory {
public:
    bool msgSend(MsgPack &);
    bool msgRecv(MsgPack &);
};
#endif
