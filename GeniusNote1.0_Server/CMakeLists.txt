cmake_minimum_required(VERSION 3.9)
project(GeniusNote1_0)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/include)
find_library(Sqlite sqlite3 /usr/lib)

add_executable(GeniusNote1_0
        source/SocketServer.cpp include/SocketServer.h
        source/SqliteServer.cpp include/SqliteServer.h
        include/Log.h
        include/sqlite3.h
        #ServerSocetTest.cpp
        #test/ServerSqliteTest.cpp
        test/main.cpp
        )

target_link_libraries(GeniusNote1_0 ${Sqlite})