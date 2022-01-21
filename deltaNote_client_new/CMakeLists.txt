cmake_minimum_required(VERSION 3.17)
project(note_client_clion)
project(sqlite_test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "E:/Qt/6.2.2/mingw_64")

# third lib and include
link_directories(
        ${CMAKE_SOURCE_DIR}/3rdparty/sqlite/bin
        ${CMAKE_SOURCE_DIR}/3rdparty/dlog/bin
)
include_directories(
        ${CMAKE_SOURCE_DIR}/3rdparty/sqlite/include
        ${CMAKE_SOURCE_DIR}/3rdparty/dlog/include
)

# project include
include_directories(
        src/common
        src/deltaNote
        src/ui
        src/widget
        src/log
        src/sqlite
        src
        ui/
)


find_package(Qt6 COMPONENTS
        Core
        Widgets
        Gui
        SvgWidgets
        Xml
        Network
        Core5Compat
        REQUIRED)

find_library(sqlite_lib NAMES sqlite PATHS ./3rdparty/sqlite/bin)
find_library(dlog_lib NAMES dlog PATHS ${CMAKE_SOURCE_DIR}/3rdparty/dlog/bin)

add_executable(note_client_clion
        src/deltaNote/main.cpp
        src/deltaNote/packCtrl.cpp
        src/deltaNote/listCtrl.cpp
        src/deltaNote/datactrl.cpp
        src/deltaNote/connectctrl.cpp
        src/common/graphicscolorsvgitem.cpp
        src/common/socket.cpp
        src/sqlite/sqlite.cpp
        src/common/untils.cpp
        src/ui/checkupdate.cpp
        src/ui/choosecolor.cpp
        src/ui/login.cpp
        src/ui/mainwindow.cpp
        src/ui/newuser.cpp
        src/ui/notify.cpp
        src/ui/todolistitem.cpp
        src/widget/todolistwidget.cpp
        ui/deltaNote_client_resource.rc
        ui/icon.qrc
        ui/checkupdate.ui
        ui/choosecolor.ui
        ui/login.ui
        ui/mainwindow.ui
        ui/newuser.ui
        ui/notify.ui
        ui/todolistitem.ui
        src/sqlite/sql_base.cpp src/sqlite/sql_base.h src/common/inter_var.h src/sqlite/sql_setting.cpp src/sqlite/sql_setting.h src/sqlite/sql_todo_list.cpp src/sqlite/sql_todo_list.h src/common/inter_var.cpp)

target_link_libraries(note_client_clion
        Qt::Core
        Qt::Widgets
        Qt::Gui
        Qt::SvgWidgets
        Qt::Xml
        Qt::Network
        Qt::Core5Compat
        ws2_32
        sqlite3
        dlog
        )

add_executable(sqlite_test
        test/sql_test.cpp
        src/sqlite/sql_base.cpp
        src/sqlite/sql_setting.cpp
        src/sqlite/sql_todo_list.cpp
        src/common/inter_var.cpp
        )
target_link_libraries(sqlite_test
        Qt::Core
        sqlite3
        dlog
        )


ADD_DEFINITIONS(-DWINDOW_CLIENT)


if (WIN32)
    set(DEBUG_SUFFIX)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
