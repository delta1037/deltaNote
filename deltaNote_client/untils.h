#ifndef UNTILS_H
#define UNTILS_H

#include <string.h>

#include "pack.h"
#include "log.h"

#define G_ARR_SIZE_SERVER 16
#define G_ARR_SIZE_USERNAME 8
#define G_ARR_SIZE_PASSWD 12

static char g_server[G_ARR_SIZE_SERVER];
static int g_port;

static char g_username[G_ARR_SIZE_USERNAME];
static char g_passdw[G_ARR_SIZE_PASSWD];

static bool isLogin;
static bool isLocked;

void makeSocketPack(MSG &synPack, int msgSize, char msgSeg, char msgOp);
void makeDataPack(MSG_OP_PACK &opPack, char *createTimestamp, char *opTimestamp, char *data, char op, char isCheck);
void parserServerPort(char *serverPort);

#endif // UNTILS_H
