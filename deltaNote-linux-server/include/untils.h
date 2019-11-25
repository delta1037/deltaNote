//
// Created by delta on 11/20/19.
//

#ifndef DELTANOTE_UNTILS_H
#define DELTANOTE_UNTILS_H

#include <cstring>

#include "pack.h"

void makeSocketPack(MSG_PACK &synPack, int msgSize, char msgSeg, char msgOp);
void makeDataPack(MSG_OP_PACK &opPack, char *opTimestamp, char *createTimestamp, char op, char isCheck, char *data);
#endif //DELTANOTE_UNTILS_H
