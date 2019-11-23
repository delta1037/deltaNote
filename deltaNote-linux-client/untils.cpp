#include "untils.h"

char g_server[G_ARR_SIZE_SERVER];
int g_port;

char g_username[G_ARR_SIZE_USERNAME];
char g_passdw[G_ARR_SIZE_PASSWD];

bool isLogin;
bool isLocked;

QColor fontColor;
QColor iconColor;
int transparentPos;
bool cleanFlag;

void synLocalChange(){
    vector<MSG_OP_PACK> retDataPack;

    ClientSqlite sqlite;
    int ret = sqlite.getLocalChange(retDataPack);

    SocketClient socket;
    int sendSize = int(retDataPack.size());
    MSG synPack{};
    for (int index = 0; index < sendSize; ++index) {
        int left = min(5, sendSize - index);
        makeSocketPack(synPack, left, ((left == 5) && (sendSize - index != 5))? MSG_FULL:MSG_HALF, RET);
        for (int i = 0; i < 5 && index < sendSize; ++i, ++index) {
            makeDataPack(synPack.msgQueue[i], retDataPack[index].opTimestamp, retDataPack[index].createTimestamp, retDataPack[index].op, retDataPack[index].isCheck, retDataPack[index].data);
        }
        socket.sendMsg(&synPack, sizeof(synPack));
    }
    LOG_INFO("send local change")
}

MSG_State synMsgToServer(MSG &synPack){
    SocketClient socketClient = SocketClient(g_server, g_port);
    socketClient.sendMsg(&synPack, sizeof(synPack));

    MSG recvPack{};
    socketClient.recvMsg(&recvPack, sizeof (recvPack));

    if(PushError == recvPack.msgState || CleanError == recvPack.msgState){
        QMessageBox::warning(nullptr, "Warning", "push msg to server error!", QMessageBox::Yes);
    } else {
        socketClient.sendMsg(&synPack, sizeof(synPack));

        MSG recvPack{};
        socketClient.recvMsg(&recvPack, sizeof (recvPack));
    }
    return MSG_State(recvPack.msgState);
}
void makeSocketPack(MSG &synPack, int msgSize, char msgSeg, char msgOp){
    strcpy(synPack.userName, g_username);
    strcpy(synPack.passwd, g_passdw);
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

int isIPAddr(const char* pStr)
{
    int bRet = 1;
    if (nullptr == pStr) return -1;
        const char* p = pStr;
    for (; *p != '\0'; p++)
    {
        if ((isalpha(*p)) && (*p != '.'))
        {
            bRet = 0;
            break;
        }
    }
    return bRet;
}

int getIPbyDomain(const char* domain, char* ip)
{
    struct hostent *answer;
    answer = gethostbyname(domain);
    if (nullptr == answer)
    {
        herror("gethostbyname");//the error function of itself
        return -1;
    }
    if (answer->h_addr_list[0])
        inet_ntop(AF_INET, (answer->h_addr_list)[0], ip, 16);
    else
        return -1;
    return 0;
}

void parserServerPort(char *serverPort){
    bool isServer = true;
    char server[G_ARR_SIZE_SERVER] = {0};
    int port = 0;
    for(int i = 0; serverPort[i] != '\0'; ++i){
        if(serverPort[i] == ':'){
            isServer = false;
            server[i] = '\0';
            continue;
        }

        if(isServer){
            server[i] = serverPort[i];
        } else {
            port = port * 10 + serverPort[i] - '0';
        }
    }

    //check
    if(server[0] == '\0' || port == 0){
        QMessageBox::warning(nullptr, "Error", "server_port format error", QMessageBox::Yes);
        LOG_ERROR("server and port parser error!")
        return;
    }

    if(!isIPAddr(server)){
        char serverIP[G_ARR_SIZE_SERVER] = {0};
        int ret = getIPbyDomain(server, serverIP);

        if(ret != 0){
            QMessageBox::warning(nullptr, "Error", "host name unreachable", QMessageBox::Yes);
            LOG_ERROR("host name error");
            return;
        }
    }

    // store log info
    strcpy(g_server, server);
    g_port = port;
}
