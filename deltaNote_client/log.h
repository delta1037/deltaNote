#ifndef LOG_H
#define LOG_H
#include <stdio.h>
#include <stdlib.h>

#define RED    ""
#define GREEN  ""

#define INFO   "[INFO]"
#define ERROR  "[ERROR]"
#define NEW_LINE "\n"

#define LOG_ERROR(...)  {\
                            printf(" ");  \
                            printf(ERROR); \
                            printf(" ");  \
                            printf(__VA_ARGS__);   \
                            printf("\n");  \
                        }

#define LOG_INFO(...)  {\
                            printf(" ");  \
                            printf(INFO); \
                            printf(" ");  \
                            printf(__VA_ARGS__);   \
                            printf("\n");  \
                        }

#define CHECK(x,m,handle) if((x) == (m)){\
                           handle;\
                         }
#endif // LOG_H
