cmake_minimum_required(VERSION 3.17)
project(note_client_clion)
project(note_server_clion)
# project(sqlite_test)

set(CMAKE_CXX_STANDARD 14)
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
        log
        utils
        sqlite
        http
        client/logic
        client/ui/main
        client/ui/setting
        client/ui/info
        client/ui/utils
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
        client/ui/main/mainwindow.cpp
        client/ui/main/mainwindow.ui
        client/ui/main/todo_detail.cpp
        client/ui/main/todo_detail.ui
        client/ui/main/todolistwidget.cpp
        client/ui/main/todolistitem.cpp
        client/ui/main/todolistitem.ui
        client/ui/setting/login.cpp
        client/ui/setting/login.ui
        client/ui/setting/choosecolor.cpp
        client/ui/setting/choosecolor.ui
        client/ui/setting/newuser.cpp
        client/ui/setting/newuser.ui
        client/ui/info/notify.cpp
        client/ui/info/notify.ui
        client/ui/deltaNote_client_resource.rc
        client/ui/icon.qrc
        client/ui/utils/svg_color.cpp
        utils/inter_var.cpp
        sqlite/sql_base.cpp
        sqlite/sql_key_value.cpp
        sqlite/sql_todo_list.cpp
        http/http.cpp
        client/logic/setting_ctrl.cpp
        client/logic/c_data_ctrl.cpp
        client/logic/c_sync_data.cpp
        client/logic/main.cpp
        client/logic/reminder.cpp client/logic/reminder.h)

target_link_libraries(note_client_clion
        Qt::Core
        Qt::Widgets
        Qt::Gui
        Qt::SvgWidgets
        Qt::Xml
        Qt::Network
        Qt::Core5Compat
        sqlite3
        dlog
        jsoncpp
        base64
        )

add_executable(note_server_clion
        utils/inter_var.cpp
        sqlite/sql_base.cpp
        sqlite/sql_user.cpp
        sqlite/sql_todo_list.cpp
        http/http.cpp
        server/logic/s_sync_data.cpp
        server/logic/s_user_ctrl.cpp
        server/logic/main.cpp
        )
target_link_libraries(note_server_clion
        ws2_32
        sqlite3
        dlog
        jsoncpp
        base64
        )
#add_executable(sqlite_test
#        test/sql_test.cpp
#        client/sqlite/sql_base.cpp
#        client/sqlite/sql_setting.cpp
#        client/sqlite/sql_todo_list.cpp
#        client/utils/inter_var.cpp
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
