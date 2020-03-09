#include "untils.h"

char g_server[G_ARR_SIZE_SERVER];
int g_port;

char g_username[G_ARR_SIZE_USERNAME];
char g_passwd[G_ARR_SIZE_PASSWD];

bool isLogin;
bool isLocked;
bool isAutoStart;

QColor fontColor;
QColor iconColor;
int transparentPos;

int xPos;
int yPos;

int frameHeight;
int frameWidth;

int fontSize;

QRect desktopRect;
QWidget *mainWindowWidget;
