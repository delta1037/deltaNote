#include "untils.h"

void makeSocketPack(MSG &synPack, int msgSize, char msgSeg, char msgOp){
    strcpy(synPack.userName, g_username);
    strcpy(synPack.passwd, g_passdw);

    synPack.msgSize = msgSize;
    synPack.msg_seg = msgSeg;
    synPack.msgOp = msgOp;
}

void makeDataPack(MSG_OP_PACK &opPack, char *createTimestamp, char *opTimestamp, char *data, char op, char isCheck){
    strcpy(opPack.createTimestamp, createTimestamp);
    strcpy(opPack.opTimestamp, opTimestamp);
    strcpy(opPack.data, data);
    opPack.op = op;
    opPack.isCheck = isCheck;
}

void parserServerPort(char *serverPort){
    bool isIP = true;
    char server[G_ARR_SIZE_SERVER] = {0};
    int port = 0;
    for(int i = 0; serverPort[i] != '\0'; ++i){
        if(serverPort[i] == ':'){
            isIP = false;
            server[i] = '\0';
            continue;
        }

        if(isIP){
            server[i] = serverPort[i];
        } else {
            port = port * 10 + serverPort[i] - '0';
        }
    }

    //check
    if(server[0] == '\0' || port == 0){
        LOG_ERROR("server and port parser error!")
        return;
    }

    // store log info
    strcpy(g_server, server);
    g_port = port;
}
