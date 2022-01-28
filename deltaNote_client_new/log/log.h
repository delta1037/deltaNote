#ifndef DELTA_NOTE_LOG_H
#define DELTA_NOTE_LOG_H

#include "dlog.h"

#define FORMAT_PREFIX "<%s, %s, %d> "
#define __FILENAME__ ( __builtin_strrchr(__FILE__, '\\') ? __builtin_strrchr(__FILE__, '\\') + 1 : __FILE__ )
#define d_logic_error(format, ...) log(LOG_MODULE_INIT(d_logic), LOG_ERROR, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define d_logic_info(format, ...)  log(LOG_MODULE_INIT(d_logic), LOG_INFO, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define d_logic_warn(format, ...) log(LOG_MODULE_INIT(d_logic), LOG_WARN, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define d_logic_debug(format, ...) log(LOG_MODULE_INIT(d_logic), LOG_DEBUG, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);

#define d_sql_error(format, ...) log(LOG_MODULE_INIT(d_sql), LOG_ERROR, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define d_sql_info(format, ...)  log(LOG_MODULE_INIT(d_sql), LOG_INFO, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define d_sql_warn(format, ...) log(LOG_MODULE_INIT(d_sql), LOG_WARN, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define d_sql_debug(format, ...) log(LOG_MODULE_INIT(d_sql), LOG_DEBUG, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);

#define d_ui_error(format, ...) log(LOG_MODULE_INIT(d_ui), LOG_ERROR, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define d_ui_info(format, ...)  log(LOG_MODULE_INIT(d_ui), LOG_INFO, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define d_ui_debug(format, ...) log(LOG_MODULE_INIT(d_ui), LOG_DEBUG, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);

#define d_net_error(format, ...) log(LOG_MODULE_INIT(d_net), LOG_ERROR, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define d_net_info(format, ...)  log(LOG_MODULE_INIT(d_net), LOG_INFO, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define d_net_warn(format, ...) log(LOG_MODULE_INIT(d_net), LOG_WARN, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define d_net_debug(format, ...) log(LOG_MODULE_INIT(d_net), LOG_DEBUG, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);

#define d_test_error(format, ...) log(LOG_MODULE_INIT(d_test), LOG_ERROR, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define d_test_info(format, ...)  log(LOG_MODULE_INIT(d_test), LOG_INFO, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define d_test_warn(format, ...) log(LOG_MODULE_INIT(d_test), LOG_WARN, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);
#define d_test_debug(format, ...) log(LOG_MODULE_INIT(d_test), LOG_DEBUG, FORMAT_PREFIX#format, __FILENAME__, __FUNCTION__ , __LINE__, __VA_ARGS__);

#endif //DELTA_NOTE_LOG_H
