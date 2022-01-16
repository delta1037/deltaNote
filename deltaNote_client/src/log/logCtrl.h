#ifndef DELTANOTE_LOGCTRL_H
#define DELTANOTE_LOGCTRL_H

#include <cstdarg>
#include <ctime>

#include <iostream>
#include <fstream>
#include <string>

#define MAX_BUFFER 2048

#include "logManage.h"

class LogCtrl {
public:
    static void error(const char *format, ...);
    static void warn(const char *format, ...);
    static void info(const char *format, ...);
    static void debug(const char *format, ...);

private:
    static std::string  getFormatStr(const char * format, va_list args );
};

#define CHECK(x,m,handle) if((x) == (m)){   \
                           handle;          \
                         }
#endif //DELTANOTE_LOGCTRL_H
