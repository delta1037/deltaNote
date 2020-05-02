#include "../../include/common/blacklist.h"

BlacklistControl *BlacklistControl::_instance = nullptr;

BlacklistControl::BlacklistControl() {

}

BlacklistControl *BlacklistControl::instance() {
    if(_instance == nullptr){
        _instance = new BlacklistControl();
    }
    return _instance;
}

void BlacklistControl::setBlacklistPath(const char path[]) {
    strcpy(blacklistPath, path);
}

void BlacklistControl::setTmpBlacklistPath(const char path[]) {
    strcpy(blacklistPathTmp, path);
}


long BlacklistControl::ipStrToNum(char ipStr[]) {
    std::string sIPStr(ipStr);
    return ipStrToNum(sIPStr);
}

long BlacklistControl::ipStrToNum(std::string &ipStr) {
    std::vector<unsigned int> ipNums = std::vector<unsigned int>(4, 0);
    unsigned int ipSegIndex = 0, ipSegNum = 0;
    for(char i : ipStr){
        if(i == '.'){
            ipNums[ipSegIndex] = ipSegNum;
            ++ipSegIndex;
            ipSegNum = 0;
            continue;
        } else if (i >= '0' && i <= '9'){
            ipSegNum = ipSegNum * 10 + i - '0';
            if(ipSegNum > 255){
                return 0;
            }
        } else {
            return 0;
        }
    }
    if(ipSegIndex != 3){
        return 0;
    }else{
        ipNums[ipSegIndex] = ipSegNum;
    }
    return (ipNums[0] << 24) + (ipNums[1] << 16) + (ipNums[2] << 8) + ipNums[3];
}

bool BlacklistControl::loadAllBlacklist() {
    return loadBlacklist(blacklistPath, s_blacklistMap) && loadBlacklist(blacklistPathTmp, s_tmpBlacklistMap);
}

bool BlacklistControl::loadBlacklist(char filePath[], std::unordered_map<std::string, int> &blacklistMap) {
    FILE *fileFd = fopen(filePath, "r");
    if(fileFd == nullptr){
        LogCtrl::error("black list file open to read fail");
        return false;
    }

    int blacklistSize = 0;
    fscanf(fileFd, "blacklistSize:%d\n", &blacklistSize);
    for(int i = 0; i < blacklistSize; ++i){
        long ipNum;
        char ipBuffer[64];
        fscanf(fileFd, "%s %d\n", ipBuffer, &ipNum);
        std::string ipStr(ipBuffer);

        blacklistMap.insert(std::make_pair(ipStr, ipNum));
    }

    fclose(fileFd);
    return true;
}

bool BlacklistControl::saveTmpBlacklist() {
    return saveBlacklist(blacklistPathTmp, s_tmpBlacklistMap);
}

bool BlacklistControl::saveBlacklist(char filePath[], std::unordered_map<std::string, int> &blacklistMap) {
    FILE *fileFd = fopen(filePath, "w");
    if(fileFd == nullptr){
        LogCtrl::error("black list file open to write fail");
        return false;
    }

    fprintf(fileFd, "blacklistSize:%d\n", (int)blacklistMap.size());
    for(const auto& it : blacklistMap){
        fprintf(fileFd, "%s %d\n", it.first.c_str(), it.second);
    }

    fclose(fileFd);
    return true;
}

bool BlacklistControl::inBlacklist(char *ipStr) {
    std::string sIPStr(ipStr);
    return inBlacklist(sIPStr);
}

//在黑名单中返回true，不在返回false
bool BlacklistControl::inBlacklist(std::string &ipStr) {
    return s_blacklistMap.find(ipStr) != s_blacklistMap.end();
}

bool BlacklistControl::addBlacklistItem(char ipStr[]) {
    std::string sIPStr(ipStr);
    return addBlacklistItem(sIPStr);
}

bool BlacklistControl::addBlacklistItem(std::string &ipStr) {
    auto bt = s_blacklistMap.find(ipStr);
    if(bt != s_blacklistMap.end()){
        // 黑名单中已存在
        return true;
    }

    auto it = s_tmpBlacklistMap.find(ipStr);
    if(it == s_tmpBlacklistMap.end()){
        //LOG_INFO("add new black ip")
        s_tmpBlacklistMap.insert(make_pair(ipStr, 1));
    }else{
        //LOG_INFO("black ip time add 1")
        it->second++;
        /*
        if(it->second > 5){
            if(!inBlacklist(ipStr)){
                LogCtrl::info("add new black ip:%s", ipStr.c_str());
                s_blacklistMap.insert(make_pair(ipStrToNum(ipStr), ipStr));
            }
            s_tmpBlacklistMap.erase(s_tmpBlacklistMap.find(ipStr));
        }
        */
    }

    // 永久存储
    saveTmpBlacklist();
    return true;
}