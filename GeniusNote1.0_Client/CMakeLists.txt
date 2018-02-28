cmake_minimum_required(VERSION 3.9)
project(GeniusNote1_0_Client)

set(CMAKE_CXX_STANDARD 14)

add_executable(GeniusNote1_0_Client source/SocketClient.cpp include/SocketClient.h include/Log.h ClientSocketTest.cpp)