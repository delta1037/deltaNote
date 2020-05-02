#ifndef DELTA_NOTE_BLACKLIST
#define DELTA_NOTE_BLACKLIST

#include <string>
#include <unordered_map>

#include "untils.h"
#include "../logCtrl/logCtrl.h"

class BlacklistControl {
private:
    static BlacklistControl *_instance;

    char blacklistPath[PATH_SIZE];
    char blacklistPathTmp[PATH_SIZE];
    std::unordered_map<std::string, int> s_blacklistMap;
    std::unordered_map<std::string, int> s_tmpBlacklistMap;

private:
    static long ipStrToNum(char ipStr[]);
    static long ipStrToNum(std::string &ipStr);

protected:
    BlacklistControl();

public:
    static BlacklistControl *instance();

    void setBlacklistPath(const char []);
    void setTmpBlacklistPath(const char []);

    bool addBlacklistItem(char ipStr[]);
    bool addBlacklistItem(std::string &ipStr);
    bool addBlacklist();

    bool loadAllBlacklist();
    bool loadBlacklist(char filePath[], std::unordered_map<std::string, int> &);

    bool saveTmpBlacklist();
    bool saveBlacklist(char filePath[], std::unordered_map<std::string, int> &);

    bool inBlacklist(char ipStr[]);
    bool inBlacklist(std::string &ipStr);
};

#endif //DELTA_NOTE_BLACKLIST