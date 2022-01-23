cmake_minimum_required(VERSION 3.17)
project(note_client_clion)
# project(sqlite_test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "E:/Qt/6.2.2/mingw_64")

# third lib and include
link_directories(
        ${CMAKE_SOURCE_DIR}/3rdparty/sqlite/bin
        ${CMAKE_SOURCE_DIR}/3rdparty/dlog/bin
        ${CMAKE_SOURCE_DIR}/3rdparty/jsoncpp/bin
        ${CMAKE_SOURCE_DIR}/3rdparty/base64/bin
)
include_directories(
        ${CMAKE_SOURCE_DIR}/3rdparty/sqlite/include
        ${CMAKE_SOURCE_DIR}/3rdparty/dlog/include
        ${CMAKE_SOURCE_DIR}/3rdparty/httplib/include
        ${CMAKE_SOURCE_DIR}/3rdparty/jsoncpp/include
        ${CMAKE_SOURCE_DIR}/3rdparty/base64/include
)

# project include
include_directories(
        src/log
        src/utils
        src/sqlite
        src/network
        src/logic
        src/ui/main
        src/ui/setting
        src/ui/info
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
find_library(jsoncpp_lib NAMES jsoncpp PATHS ${CMAKE_SOURCE_DIR}/3rdparty/jsoncpp/bin)
find_library(base64_lib NAMES base64 PATHS ${CMAKE_SOURCE_DIR}/3rdparty/base64/bin)

add_executable(note_client_clion
        src/ui/main/mainwindow.cpp
        src/ui/main/mainwindow.ui
        src/ui/main/todolistwidget.cpp
        src/ui/main/todolistitem.cpp
        src/ui/main/todolistitem.ui
        src/ui/setting/login.cpp
        src/ui/setting/login.ui
        src/ui/setting/choosecolor.cpp
        src/ui/setting/choosecolor.ui
        src/ui/setting/newuser.cpp
        src/ui/setting/newuser.ui
        src/ui/info/notify.cpp
        src/ui/info/notify.ui
        src/ui/deltaNote_client_resource.rc
        src/ui/icon.qrc
        src/utils/svg_color.cpp
        src/utils/inter_var.cpp
        src/sqlite/sql_base.cpp
        src/sqlite/sql_setting.cpp
        src/sqlite/sql_todo_list.cpp
        src/network/network.cpp
        src/logic/setting_ctrl.cpp
        src/logic/data_ctrl.cpp
        src/logic/sync_data.cpp
        src/logic/main.cpp
        )

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
        jsoncpp
        base64
        )

#add_executable(sqlite_test
#        test/sql_test.cpp
#        src/sqlite/sql_base.cpp
#        src/sqlite/sql_setting.cpp
#        src/sqlite/sql_todo_list.cpp
#        src/utils/inter_var.cpp
#        )
#target_link_libraries(sqlite_test
#        Qt::Core
#        sqlite3
#        dlog
#        )


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
