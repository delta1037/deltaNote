#include "../../include/common/blackList.h"

ServerBlackListControl *ServerBlackListControl::_instance = nullptr;

ServerBlackListControl::ServerBlackListControl() {

}

ServerBlackListControl *ServerBlackListControl::instance() {
    if(_instance == nullptr){
        _instance = new ServerBlackListControl();
    }
    return _instance;
}

void ServerBlackListControl::setBlacklistPath(const char path[]) {
    strcpy(blacklistPath, path);
}


long ServerBlackListControl::ipStrToNum(char ipStr[]) {
    std::string sIPStr(ipStr);
    return ipStrToNum(sIPStr);
}

long ServerBlackListControl::ipStrToNum(std::string &ipStr) {
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

bool ServerBlackListControl::loadBlackList() {
    return loadBlackList(blacklistPath);
}

bool ServerBlackListControl::loadBlackList(char filePath[]) {
    FILE *fileFd = fopen(filePath, "r");
    if(fileFd == nullptr){
        LogCtrl::error("black list file open to read fail");
        return false;
    }

    // 黑名单列表
    int blackListSize = 0;
    fscanf(fileFd, "blackListSize:%d\n", &blackListSize);
    for(int i = 0; i < blackListSize; ++i){
        long ipNum;
        char ipBuffer[64];
        fscanf(fileFd, "%ld %s\n", &ipNum, ipBuffer);
        std::string ipStr(ipBuffer);

        s_blackListMap.insert(std::make_pair(ipNum, ipStr));
    }
    
    // 黑名单临时列表
    int tmpBlackListSize = 0;
    fscanf(fileFd, "tmpBlackListSize:%d\n", &tmpBlackListSize);
    for(int i = 0; i < tmpBlackListSize; ++i){
        int ipNum;
        char ipBuffer[64];
        fscanf(fileFd, "%s %d\n", ipBuffer, &ipNum);
        std::string ipStr(ipBuffer);

        s_tmpBlackListMap.insert(std::make_pair(ipStr, ipNum));
    }

    fclose(fileFd);
    return true;
}
bool ServerBlackListControl::saveBlackList() {
    return saveBlackList(blacklistPath);
}
bool ServerBlackListControl::saveBlackList(char filePath[]) {
    FILE *fileFd = fopen(filePath, "w");
    if(fileFd == nullptr){
        LogCtrl::error("black list file open to write fail");
        return false;
    }

    // 黑名单列表
    fprintf(fileFd, "blackListSize:%d\n", (int)s_blackListMap.size());
    for(const auto& it : s_blackListMap){
        fprintf(fileFd, "%ld %s\n", it.first, it.second.c_str());
    }
    
    // 黑名单那临时列表
    fprintf(fileFd, "tmpBlackListSize:%d\n", (int)s_tmpBlackListMap.size());
    for(const auto& it : s_tmpBlackListMap){
        fprintf(fileFd, "%s %d\n", it.first.c_str(), it.second);
    }

    fclose(fileFd);
    return true;
}

bool ServerBlackListControl::inBlackList(char *ipStr) {
    std::string sIPStr(ipStr);
    return inBlackList(sIPStr);
}

//在黑名单中返回true，不在返回false
bool ServerBlackListControl::inBlackList(std::string &ipStr) {
    return s_blackListMap.find(ipStrToNum(ipStr)) != s_blackListMap.end();
}

bool ServerBlackListControl::addBlackListItem(char ipStr[]) {
    std::string sIPStr(ipStr);
    return addBlackListItem(sIPStr);
}

bool ServerBlackListControl::addBlackListItem(std::string &ipStr) {
    auto bt = s_blackListMap.find(ipStrToNum(ipStr));
    if(bt != s_blackListMap.end()){
        // 黑名单中已存在
        return true;
    }

    auto it = s_tmpBlackListMap.find(ipStr);
    if(it == s_tmpBlackListMap.end()){
        //LOG_INFO("add new black ip")
        s_tmpBlackListMap.insert(make_pair(ipStr, 1));
    }else{
        //LOG_INFO("black ip time add 1")
        it->second++;
        if(it->second > 5){
            if(!inBlackList(ipStr)){
                LogCtrl::info("add new black ip:%s", ipStr.c_str());
                s_blackListMap.insert(make_pair(ipStrToNum(ipStr), ipStr));
            }
            s_tmpBlackListMap.erase(s_tmpBlackListMap.find(ipStr));
        }
    }

    // 永久存储
    saveBlackList();
    return true;
}

bool ServerBlackListControl::addBlackList() {
    bool tag = false;
    for(const auto& it : s_tmpBlackListMap){
        // 访问次数错误大于等于3则加入永久黑名单
        if(it.second <= 5){
            continue;
        }
        tag = true;
        std::string t_ip = it.first;
        if(!inBlackList(t_ip)){
            s_blackListMap.insert(std::make_pair(ipStrToNum(t_ip), t_ip));
        }

        s_tmpBlackListMap.erase(s_tmpBlackListMap.find(it.first));
    }
    if(tag){
        saveBlackList();
    }
    return true;
}