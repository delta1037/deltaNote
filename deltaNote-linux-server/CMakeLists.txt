cmake_minimum_required(VERSION 3.9)
project(deltaNote)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/include)
find_library(Sqlite sqlite3 ./lib)

find_package(Threads REQUIRED)

add_executable(deltaNote
        src/socket.cpp include/socket.h
        src/sqlite.cpp include/sqlite.h
        include/Log.h
        include/sqlite3.h
        src/deltaNote.cpp
        include/untils.h)

target_link_libraries(deltaNote ${Sqlite})
target_link_libraries(deltaNote Threads::Threads)