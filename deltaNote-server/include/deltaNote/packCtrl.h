#ifndef PACK_CTRL
#define PACK_CTRL

#include <vector>

#include "../common/socket.h"

class PackCtrlFactory : public Socket {
public:
    virtual ~PackCtrlFactory();
    virtual bool msgSend(MsgPack &) = 0;
    virtual bool msgRecv(MsgPack &) = 0;
};

class FixedPackCtrl : public PackCtrlFactory {
public:
    bool msgSend(MsgPack &);
    bool msgRecv(MsgPack &);
};

class DynamicPackCtrl : public PackCtrlFactory {
public:
    bool msgSend(MsgPack &);
    bool msgRecv(MsgPack &);
};
#endif