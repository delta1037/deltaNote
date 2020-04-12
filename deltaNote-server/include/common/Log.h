//
// Created by geniusrabbit on 18-2-27.
//

#ifndef GENIUSNOTE1_0_LOG_H
#define GENIUSNOTE1_0_LOG_H

#include <cstdio>
#include <cerrno>
#include <ctime>
#include "untils.h"

#define INFO   " [INFO] "
#define ERROR  " [ERROR] "
#define NEW_LINE "\n"
#define DEBUG

#ifdef DEBUG
#define LOG_ERROR(...)  { time(&curTime);                           \
                          struct tm *localTime = localtime(&curTime);\
                          char timeValue[100];\
                          sprintf(timeValue, "%d-%02d-%02d %02d:%02d:%02d", \
                                          localTime->tm_year + 1900,\
                                          localTime->tm_mon + 1,\
                                          localTime->tm_mday,\
                                          localTime->tm_hour,\
                                          localTime->tm_min,\
                                          localTime->tm_sec);\
                          printf("%s", timeValue);                  \
                          printf(ERROR);                            \
                          printf(__VA_ARGS__);                      \
                          printf(NEW_LINE);                         \
                                                                    \
                          pLogFile = fopen(logFilePath, "a+");      \
                          fprintf(pLogFile, "%s", timeValue);       \
                          fprintf(pLogFile, ERROR);                 \
                          fprintf(pLogFile, __VA_ARGS__);           \
                          fprintf(pLogFile, NEW_LINE);              \
                          fclose(pLogFile);                         \
                        }

#define LOG_INFO(...)   { time(&curTime);                           \
                          struct tm *localTime = localtime(&curTime);\
                          char timeValue[100];\
                          sprintf(timeValue, "%d-%02d-%02d %02d:%02d:%02d", \
                                          localTime->tm_year + 1900,\
                                          localTime->tm_mon + 1,\
                                          localTime->tm_mday,\
                                          localTime->tm_hour,\
                                          localTime->tm_min,\
                                          localTime->tm_sec);\
                          printf("%s", timeValue);                  \
                          printf(INFO);                             \
                          printf(__VA_ARGS__);                      \
                          printf(NEW_LINE);                         \
                                                                    \
                          pLogFile = fopen(logFilePath, "a+");      \
                          fprintf(pLogFile, "%s", timeValue);       \
                          fprintf(pLogFile, INFO);                  \
                          fprintf(pLogFile, __VA_ARGS__);           \
                          fprintf(pLogFile, NEW_LINE);              \
                          fclose(pLogFile);                         \
                        }
#else
#define LOG_ERROR(...)  { fprintf(pLogFile, __TIME__); \
                          fprintf(pLogFile, " "); \
                          fprintf(pLogFile, ERROR);\
                          fprintf(pLogFile, " "); \
                          fprintf(pLogFile, __VA_ARGS__);   \
                          fprintf(pLogFile, NEW_LINE); \
                        }

#define LOG_INFO(...)   { fprintf(pLogFile, __TIME__); \
                          fprintf(pLogFile, " "); \
                          fprintf(pLogFile, INFO); \
                          fprintf(pLogFile, " "); \
                          fprintf(pLogFile, __VA_ARGS__);  \
                          fprintf(pLogFile, NEW_LINE); \
                        }
#endif

#define CHECK(x,m,handle) if((x) == (m)){\
                           handle;\
                         }

#endif //GENIUSNOTE1_0_LOG_H
