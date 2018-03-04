cmake_minimum_required(VERSION 3.9)
project(GeniusNote1_0)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/include)
find_library(Sqlite sqlite3 /lib)

add_executable(GeniusNote1_0 source/SocketServer.cpp include/SocketServer.h
        source/SqliteServer.cpp include/SqliteServer.h
        source/ThreadServer.cpp include/ThreadServer.h
        include/Log.h
        include/sqlite3.h
        test/ServerSocetTest.cpp test/ServerSqliteTest.cpp main.cpp)

target_link_libraries(GeniusNote1_0 sqlite3)