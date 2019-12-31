#ifndef LOG_H
#define LOG_H
#include <cstdio>
#include <cerrno>
#include <ctime>

#define INFO   " [INFO] "
#define ERROR  " [ERROR] "
#define NEW_LINE "\n"
#define DEBUG
extern FILE *pLogFile;
extern char logPATH[1024];
extern time_t  curTime;
#ifdef DEBUG
#define LOG_ERROR(...)  { time_t  curTime;time(&curTime);                           \
                          struct tm *localTime = localtime(&curTime);\
                          char timeValue[100];\
                          sprintf(timeValue, "%d-%d-%d %d:%d:%d", \
                                          localTime->tm_year + 1900,\
                                          localTime->tm_mon,\
                                          localTime->tm_mday,\
                                          localTime->tm_hour,\
                                          localTime->tm_min,\
                                          localTime->tm_sec);\
                          printf("%s", timeValue);                  \
                          printf(ERROR);                            \
                          printf(__VA_ARGS__);                      \
                          printf(NEW_LINE);                         \
                                                                    \
                          pLogFile = fopen(logPATH, "a+");      \
                          fprintf(pLogFile, "%s", timeValue);       \
                          fprintf(pLogFile, ERROR);                 \
                          fprintf(pLogFile, __VA_ARGS__);           \
                          fprintf(pLogFile, NEW_LINE);              \
                          fclose(pLogFile);                         \
                        }

#define LOG_INFO(...)   { time_t  curTime;time(&curTime);                           \
                          struct tm *localTime = localtime(&curTime);\
                          char timeValue[100];\
                          sprintf(timeValue, "%d-%d-%d %d:%d:%d", \
                                          localTime->tm_year + 1900,\
                                          localTime->tm_mon,\
                                          localTime->tm_mday,\
                                          localTime->tm_hour,\
                                          localTime->tm_min,\
                                          localTime->tm_sec);\
                          printf("%s", timeValue);                  \
                          printf(INFO);                             \
                          printf(__VA_ARGS__);                      \
                          printf(NEW_LINE);                         \
                                                                    \
                          pLogFile = fopen(logPATH, "a+");      \
                          fprintf(pLogFile, "%s", timeValue);       \
                          fprintf(pLogFile, INFO);                  \
                          fprintf(pLogFile, __VA_ARGS__);           \
                          fprintf(pLogFile, NEW_LINE);              \
                          fclose(pLogFile);                         \
                        }
#else
#define LOG_ERROR(...)  { printf(__TIME__); \
                          printf(ERROR);\
                          printf(__VA_ARGS__);   \
                          printf(NEW_LINE); \
                        }

#define LOG_INFO(...)   { printf(__TIME__); \
                          printf(INFO); \
                          printf(__VA_ARGS__);  \
                          printf(NEW_LINE); \
                        }
#endif

#define CHECK(x,m,handle) if((x) == (m)){\
                           handle;\
                         }
#endif // LOG_H
