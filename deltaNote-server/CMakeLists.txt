cmake_minimum_required(VERSION 3.9)
project(deltaNote)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/include)
find_library(Sqlite sqlite3 ./lib)

find_package(Threads REQUIRED)

add_executable(deltaNote
        include/common/untils.h src/common/untils.cpp
        include/common/socket.h src/common/socket.cpp
        include/common/blackList.h src/common/blackList.cpp
        include/common/sqlite3.h include/common/sqlite.h src/common/sqlite.cpp
        include/deltaNote/connectCtrl.h src/deltaNote/connectCtrl.cpp
        include/deltaNote/sqliteCtrl.h src/deltaNote/sqliteCtrl.cpp
        include/deltaNote/packCtrl.h src/deltaNote/packCtrl.cpp
        src/deltaNote/deltaNote.cpp
        src/logCtrl/logCtrl.cpp include/logCtrl/logCtrl.h
        src/logCtrl/logManage.cpp include/logCtrl/logManage.h)

target_link_libraries(deltaNote ${Sqlite})
target_link_libraries(deltaNote Threads::Threads)
