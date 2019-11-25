#ifndef UNTILS_H
#define UNTILS_H

#include <QColor>
#include <QMessageBox>

#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "sqlite.h"
#include "socket.h"

void makeSocketPack(MSG_PACK &synPack, int msgSize, char msgSeg, char msgOp);
void makeDataPack(MSG_OP_PACK &opPack, char *opTimestamp, char *createTimestamp, char op, char isCheck, char *data);
void synLocalChange();
MSG_State synMsgToServer(MSG_PACK &synPack);
void parserServerPort(char *serverPort);
#endif // UNTILS_H
