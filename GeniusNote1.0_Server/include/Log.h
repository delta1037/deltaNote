//
// Created by geniusrabbit on 18-2-27.
//

#ifndef GENIUSNOTE1_0_LOG_H
#define GENIUSNOTE1_0_LOG_H

#include <cstdio>
#include <cerrno>

//#define None   "\e[0m"
#define RED    "\e[0;31m"
#define GREEN  "\e[0;32m"

#define INFO   "[INFO]"
#define ERROR  "[ERROR]"
#define NEW_LINE "\n"

#define LOG_ERROR(...)  { printf(RED); \
                          printf(__TIME__); \
                          printf(ERROR);\
                          printf(__VA_ARGS__);   \
                          printf(NEW_LINE); \
                        }

#define LOG_INFO(...)   { printf(GREEN); \
                          printf(__TIME__); \
                          printf(INFO); \
                          printf(__VA_ARGS__);  \
                          printf(NEW_LINE); \
                        }
#endif //GENIUSNOTE1_0_LOG_H
