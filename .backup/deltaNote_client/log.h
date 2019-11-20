#ifndef LOG_H
#define LOG_H
#include <iostream>
#include <cerrno>

#define RED    ""
#define GREEN  ""

#define INFO   "[INFO]"
#define ERROR  "[ERROR]"
#define NEW_LINE "\n"

#define LOG_ERROR(...)  { std::cout << RED \
                          << " "  \
                          << ERROR \
                          << " " \
                          << __VA_ARGS__   \
                          << "\n"; \
                        }

#define LOG_INFO(...)   { std::cout << RED \
                            << " " \
                            << INFO \
                            << " " \
                            << __VA_ARGS__  \
                            << "\n"; \
                          }

#define CHECK(x,m,handle) if((x) == (m)){\
                           handle;\
                         }
#endif // LOG_H
