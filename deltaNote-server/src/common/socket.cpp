#include "../../include/common/socket.h"

#include <cstdio>

int Socket::serverSocketFd;
Socket::Socket(){
    clientSocketFd = -1;
}

Socket::~Socket(){
    if(clientSocketFd > 0) {
        LogCtrl::debug("close client socket");
        close(clientSocketFd);
    }
}

bool Socket::initSocketServer(const char initServerIP[], int initServerPort) {
    LogCtrl::info("start socket server");

    int flag;
    serverSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    CHECK(serverSocketFd, SOCKET_ERROR, { LogCtrl::error("Server socket init failed"); return false; })

    struct sockaddr_in serverSocketAddr{};
    memset(&serverSocketAddr,0, sizeof(serverSocketAddr));

    serverSocketAddr.sin_family = AF_INET;
    serverSocketAddr.sin_port = htons(static_cast<uint16_t>(initServerPort));
    serverSocketAddr.sin_addr.s_addr = inet_addr(initServerIP);
    flag = bind(serverSocketFd, (struct sockaddr*)&serverSocketAddr, sizeof(serverSocketAddr));
    CHECK(flag, SOCKET_ERROR, {LogCtrl::error("Server bind error"); return false;})

    flag = listen(serverSocketFd, 20);
    CHECK(flag, SOCKET_ERROR, {LogCtrl::error("Start(Listen) error"); return false;})

    return true;
}

void Socket::getClientIPAddr(char r_clientIPAddr[]){
    strcpy(r_clientIPAddr, this->clientIPAddr);
}

bool Socket::acceptConn(){
    struct sockaddr_in clientAddr{};
    socklen_t clientAddrSize = sizeof(clientAddr);
    clientSocketFd = accept(serverSocketFd, (struct sockaddr*)&clientAddr, &clientAddrSize);
    sprintf(clientIPAddr, "%s", inet_ntoa(clientAddr.sin_addr));
    return clientSocketFd > 0;
}

bool Socket::sendMsg(void *buf, size_t size) {
    // 检查连接
    if(clientSocketFd == -1){
        return false;
    }

    // 发送数据
    int needSendSize = size;
    bool bigDataFlag = false;
    while(size > 0){
        ssize_t sendSize = write(clientSocketFd, buf, size);

        if(sendSize == -1){
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

char numToHex(int num){
    if(num >= 0 && num < 10){
        return num + '0';
    }else if(num >= 10 && num < 16){
        return num + 'a' - 10;
    }else{
        return '\0';
    }
}

void printMemery(char *buf, int size){
    if(size <= 0){
        return;
    }

    char *newbuf = (char*)malloc(2 * size + 1);
    for(int i = 0; i < size; ++i){
        int bufNum = (int)(buf[i] & 0xff);
        newbuf[2*i] = numToHex(bufNum & 0x0f);
        newbuf[2*i+1] = numToHex((bufNum >> 4) & 0x0f);
    }
    newbuf[2*size] = '\0';

    FILE *pLogFile = fopen("pack.log", "a+");
    fprintf(pLogFile, "%s\n", newbuf);
    fclose(pLogFile);

    free(newbuf);
}

bool Socket::recvMsg(void *buf, size_t size) {
    // 检查连接
    if(clientSocketFd == -1){
        return false;
    }

    // 接收数据
    int needRecvSize = size;
    bool bigDataFlag = false;
    while(size > 0){
        ssize_t recvSize=read(clientSocketFd, buf, size);

        if(recvSize == -1){
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