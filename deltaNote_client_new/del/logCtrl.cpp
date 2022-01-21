//
// Created by delta on 2020/4/9.
//

#include "log/logCtrl.h"

void LogCtrl::error(const char *format, ...) {
    va_list vaList;
    va_start(vaList, format);
    std::string message = getFormatStr(format, vaList);
    va_end(vaList);
    LogManage *logManage = LogManage::instance();
    logManage->logMessage(LOG_ERROR_O, message);
}

void LogCtrl::warn(const char *format, ...) {
    va_list vaList;
    va_start(vaList, format);
    std::string message = getFormatStr(format, vaList);
    va_end(vaList);
    LogManage *logManage = LogManage::instance();
    logManage->logMessage(LOG_WARN_O, message);
}

void LogCtrl::info(const char *format, ...) {
    va_list vaList;
    va_start(vaList, format);
    std::string message = getFormatStr(format, vaList);
    va_end(vaList);
    LogManage *logManage = LogManage::instance();
    logManage->logMessage(LOG_INFO_O, message);
}

void LogCtrl::debug(const char *format, ...) {
    va_list vaList;
    va_start(vaList, format);
    std::string message = getFormatStr(format, vaList);
    va_end(vaList);
    LogManage *logManage = LogManage::instance();
    logManage->logMessage(LOG_DEBUG_O, message);
}

std::string LogCtrl::getFormatStr(const char *format, va_list args) {
    char buffer[MAX_BUFFER]{};
    vsprintf(buffer, format, args);
    return std::string(buffer);
}
