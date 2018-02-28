cmake_minimum_required(VERSION 3.9)
project(GeniusNote1_0)

set(CMAKE_CXX_STANDARD 14)


add_executable(GeniusNote1_0 source/SocketServer.cpp include/SocketServer.h
        source/SqliteServer.cpp include/SqliteServer.h
        source/ThreadServer.cpp include/ThreadServer.h
        include/Log.h
        ServerSocetTest.cpp)
