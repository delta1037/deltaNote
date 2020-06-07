//
// Created by geniusrabbit on 18-2-27.
//

#include "common/socket.h"

SocketClient::SocketClient(){
#ifdef WINDOW_CLIENT
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
#ifdef LINUX_CLIENT
    clientSocketFd = -1;
#endif
    ret = 0;
}

SocketClient::~SocketClient(){
    if(clientSocketFd > 0){
        close(clientSocketFd);
        clientSocketFd = -1;
    }
#ifdef WINDOW_CLIENT
    WSACleanup();
#endif
}

bool SocketClient::initSocket(char *initServerIP, int initServerPort){
    clientSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSocketFd == INVALID_SOCKET){
        LogCtrl::error("Client socket init failed");
        return false;
    }

    SOCKADDR_IN serverSocketAddr;
    memset(&serverSocketAddr,0, sizeof(serverSocketAddr));
    serverSocketAddr.sin_family = AF_INET;
    serverSocketAddr.sin_port = htons(static_cast<uint16_t>(initServerPort));
    serverSocketAddr.sin_addr.s_addr = inet_addr(initServerIP);

    ret = connect(clientSocketFd, (SOCKADDR*)&serverSocketAddr, sizeof(serverSocketAddr));
    if(ret == SOCKET_ERROR){
        LogCtrl::error("connect server failed");
        return false;
    }
    return true;
}

bool SocketClient::sendMsg(void *buf, size_t size) {
    // 检查连接
    if(INVALID_SOCKET == clientSocketFd){
        return false;
    }

    // 发送数据
    int needSendSize = size;
    bool bigDataFlag = false;
    while(size > 0){
        long sendSize = send(clientSocketFd, (char *)buf, size, 0);
        //ssize_t sendSize = write(clientSocketFd, buf, size);

        if(sendSize <= 0){
            LogCtrl::info("client socket closed, stop send");
            close(clientSocketFd);
            clientSocketFd = -1;
            return false;
        }

        size -= sendSize;
        buf = (uint8_t *)buf + sendSize;

        if(size != 0 || (size == 0 && bigDataFlag == true)){
            bigDataFlag = true;
            LogCtrl::info("big data sending : %d / %d", (int)(needSendSize - size), (int)needSendSize);
        }
    }
    return true;
}

bool SocketClient::recvMsg(void *buf, size_t size) {
    // 检查连接
    if(INVALID_SOCKET == clientSocketFd){
        return false;
    }

    // 接收数据
    int needRecvSize = size;
    bool bigDataFlag = false;
    while(size > 0){
        long recvSize = recv(clientSocketFd, (char *)buf, size, 0);
        //ssize_t recvSize=read(clientSocketFd, buf, size);

        if(recvSize <= 0){
            //  当接收到的大小小于等于0时判断为对端关闭连接
            LogCtrl::info("client socket closed, stop receive");
            close(clientSocketFd);
            clientSocketFd = -1;
            return false;
        }

        size -= recvSize;
        buf = (uint8_t *)buf + recvSize;
        if(size != 0 || (size == 0 && bigDataFlag == true)){
            bigDataFlag = true;
            LogCtrl::info("big data receiving : %d / %d", (int)(needRecvSize - size), (int)needRecvSize);
        }
    }
    return true;
}

