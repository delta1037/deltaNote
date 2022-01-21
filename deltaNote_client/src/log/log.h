#ifndef LOG_H
#define LOG_H

#include "dlog.h"

#define FORMAT_PREFIX "<%s, %s, %d> "
#define __FILENAME__ ( __builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__ )
#define delta_note_error(format, ...) log(LOG_MODULE_INIT(delta_note), LOG_ERROR, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define delta_note_info(format, ...)  log(LOG_MODULE_INIT(delta_note), LOG_INFO, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define delta_note_debug(format, ...) log(LOG_MODULE_INIT(delta_note), LOG_DEBUG, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);

#define sqlite_error(format, ...) log(LOG_MODULE_INIT(sqlite), LOG_ERROR, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define sqlite_info(format, ...)  log(LOG_MODULE_INIT(sqlite), LOG_INFO, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define sqlite_debug(format, ...) log(LOG_MODULE_INIT(sqlite), LOG_DEBUG, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);

#endif
