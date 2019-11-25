#ifndef LOG_H
#define LOG_H
#include <stdio.h>
#include <stdlib.h>

#define LOG_INFO_TAG   "[INFO]"
#define LOG_ERROR_TAG  "[ERROR]"
#define NEW_LINE "\n"

#ifdef DEBUG
#define LOG_ERROR(...)  {\
                            printf(" ");  \
                            printf(LOG_ERROR_TAG); \
                            printf(" ");  \
                            printf(__VA_ARGS__);   \
                            printf("\n");  \
                        }

#define LOG_INFO(...)  {\
                            printf(" ");  \
                            printf(LOG_INFO_TAG); \
                            printf(" ");  \
                            printf(__VA_ARGS__);   \
                            printf("\n");  \
                        }


#else
#define LOG_ERROR(...)  {\
                        }

#define LOG_INFO(...)  {\
                        }
#endif

#define CHECK(x,m,handle) if((x) == (m)){\
                           handle;\
                         }

#endif // LOG_H
