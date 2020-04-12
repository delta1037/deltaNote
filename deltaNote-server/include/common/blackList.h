#ifndef DELTA_NOTE_BLACKLIST
#define DELTA_NOTE_BLACKLIST

#include <string>
#include <unordered_map>

#include "untils.h"
#include "../logCtrl/logCtrl.h"

class ServerBlackListControl {
private:
    static ServerBlackListControl *_instance;

    char blacklistPath[PATH_SIZE];
    std::unordered_map<long, std::string> s_blackListMap;
    std::unordered_map<std::string, int> s_tmpBlackListMap;

private:
    static long ipStrToNum(char ipStr[]);
    static long ipStrToNum(std::string &ipStr);

protected:
    ServerBlackListControl();

public:
    static ServerBlackListControl *instance();

    void setBlacklistPath(const char []);
    bool addBlackListItem(char ipStr[]);
    bool addBlackListItem(std::string &ipStr);
    bool addBlackList();

    bool loadBlackList(char filePath[]);
    bool saveBlackList(char filePath[]);
    bool loadBlackList();
    bool saveBlackList();

    bool inBlackList(char ipStr[]);
    bool inBlackList(std::string &ipStr);
};

#endif //DELTA_NOTE_BLACKLIST