#ifndef UNTILS_H
#define UNTILS_H

#include <QColor>
#include <QMessageBox>

#include <winsock.h>
#include <string.h>


#include "sqlite.h"
#include "socket.h"

extern char g_username[G_ARR_SIZE_USERNAME];
extern char g_passwd[G_ARR_SIZE_PASSWD];

extern char g_server[G_ARR_SIZE_SERVER];
extern int g_port;

extern bool isLogin;
extern bool isLocked;
extern bool isAutoStart;

extern QColor fontColor;
extern QColor iconColor;
extern int transparentPos;
extern bool cleanFlag;

extern int xPos;
extern int yPos;

extern int frameWigth;
extern int frameHeight;

extern QRect desktop;
extern QWidget *mainWindow;

bool doLogin();
void makeSocketPack(MSG_PACK &synPack, int msgSize, char msgSeg, char msgOp);
void makeDataPack(MSG_OP_PACK &opPack, char *opTimestamp, char *createTimestamp, char op, char isCheck, char *data);
void synLocalChange();
MSG_State synMsgToServer(MSG_PACK &synPack);
void parserServerPort(char *serverPort);

#endif // UNTILS_H
