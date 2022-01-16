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
    outputLogLevel = LOG_DEBUG;
    outputLocation = OUTPUT_FILE;

    logFileName = "local_log.log";
}

void LogManage::logMessage(short level, std::string &message) {
    if(!isGreaterThanLevel(level)){
        return;
    }

    switch (outputLocation){
        case OUTPUT_FILE:
            logMsgToFile(level, message);
            break;
        case OUTPUT_SCREEN:
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

void LogManage::setLoglevel(LOG_LEVEL logLevel) {
    this->outputLogLevel = logLevel;
}

void LogManage::setLogFileName(std::string &fileName) {
    this->logFileName = fileName;
}

std::string LogManage::getLevelStr(short level) {
    if(level == LOG_ERROR){
        return "[ERROR]";
    }else if(level == LOG_WARN){
        return "[WARN] ";
    }else if(level == LOG_INFO){
        return "[INFO] ";
    }else if(level == LOG_DEBUG){
        return "[DEBUG]";
    }
    return "[NONE] ";
}
