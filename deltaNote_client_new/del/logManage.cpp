//
// Created by delta on 2020/4/9.
//

#include <fstream>
#include <cstring>
#include "log/logManage.h"


LogManage* LogManage::_instance = nullptr;
LogManage *LogManage::instance() {
    if(_instance == nullptr){
        _instance = new LogManage();
    }
    return _instance;
}

LogManage::LogManage() {
    outputLogLevel = LOG_DEBUG_O;
    outputLocation = OUTPUT_FILE_O;

    logFileName = "local_log.log";
}

void LogManage::logMessage(short level, std::string &message) {
    if(!isGreaterThanLevel(level)){
        return;
    }

    switch (outputLocation){
        case OUTPUT_FILE_O:
            logMsgToFile(level, message);
            break;
        case OUTPUT_SCREEN_O:
            logMsgToScreen(level, message);
            break;
    }
}

void LogManage::logMsgToFile(short level, std::string &message) {
    std::fstream outFileFd;
    outFileFd.open(logFileName, std::ios_base::app);

    time_t now = time(nullptr);
    char *ct = ctime(&now);
    ct[strcspn(ct, "\n")] = '\0';
    outFileFd << ct << " " << getLevelStr(level) << " " << message << std::endl;
}

void LogManage::logMsgToScreen(short level, std::string &message) {
    time_t now = time(nullptr);
    char *ct = ctime(&now);
    ct[strcspn(ct, "\n")] = '\0';
    std::cout << ct << " " << getLevelStr(level) << " " << message << std::endl;
}

bool LogManage::isGreaterThanLevel(short level) {
    return outputLogLevel <= level;
}

void LogManage::setOutput(OUTPUT output) {
    this->outputLocation = output;
}

void LogManage::setLoglevel(LOG_LEVEL_OLD logLevel) {
    this->outputLogLevel = logLevel;
}

void LogManage::setLogFileName(std::string &fileName) {
    this->logFileName = fileName;
}

std::string LogManage::getLevelStr(short level) {
    if(level == LOG_ERROR_O){
        return "[ERROR]";
    }else if(level == LOG_WARN_O){
        return "[WARN] ";
    }else if(level == LOG_INFO_O){
        return "[INFO] ";
    }else if(level == LOG_DEBUG_O){
        return "[DEBUG]";
    }
    return "[NONE] ";
}
