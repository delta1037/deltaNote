//
// Created by delta on 2020/4/9.
//

#include <fstream>
#include <cstring>
#include "../../include/logCtrl/logManage.h"


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

    logFileName = "default";
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
    time_t now = time(nullptr);
    char *ct = ctime(&now);

    std::fstream outFileFd;
    outFileFd.open(getLogFilePath(), std::ios_base::app);

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

std::string LogManage::getLogFilePath(){
    time_t now = time(nullptr);
    struct tm *nowTime = localtime(&now);
    return this->logFileName + 
        "_" + std::to_string(nowTime->tm_year + 1900) + 
        "-" + getTwoBitsNumber(nowTime->tm_mon + 1) + 
        "-" + getTwoBitsNumber(nowTime->tm_mday) + ".log";
}

std::string LogManage::getTwoBitsNumber(int i){
    if(i < 10){
        return "0" + std::to_string(i);
    }else{
        return std::to_string(i);
    }
}