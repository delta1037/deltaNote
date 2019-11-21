//
// Created by delta on 11/20/19.
//

#include "../include/untils.h"

void makeSocketPack(MSG &synPack, int msgSize, char msgSeg, char msgOp){
    synPack.msgSize = msgSize;
    synPack.msg_seg = msgSeg;
    synPack.msgOp = msgOp;
}

void makeDataPack(MSG_OP_PACK &opPack, char *opTimestamp, char *createTimestamp, char op, char isCheck, char *data){
    strcpy(opPack.createTimestamp, createTimestamp);
    strcpy(opPack.opTimestamp, opTimestamp);
    strcpy(opPack.data, data);
    opPack.op = op;
    opPack.isCheck = isCheck;
}