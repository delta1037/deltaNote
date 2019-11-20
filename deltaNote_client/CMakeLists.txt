cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 14)

project(deltaNote_client)

set (CMAKE_PREFIX_PATH "/install/Qt5.13.0/5.13.0/gcc_64")

set(SOURCE_FILES
        main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
        login.cpp login.h login.ui
        socket.cpp socket.h
        pack.h log.h)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files

find_package( Qt5LinguistTools )
find_package( Qt5Core 5.13 REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )
find_package( Qt5Sql REQUIRED )
find_package( Qt5Svg REQUIRED )
find_package( Qt5Network REQUIRED )
find_package( Qt5Xml REQUIRED )
find_package( Qt5XmlPatterns REQUIRED )
find_package( Qt5PrintSupport REQUIRED )
find_package( Qt5WebSockets REQUIRED )
find_package( Qt5Qml REQUIRED )

qt5_wrap_ui(ui_mainwindow.h mainwindow.ui)
qt5_wrap_ui(ui_login.h login.ui)

add_executable(deltaNote_client ${SOURCE_FILES})

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

# Use the Widgets module from Qt 5
target_link_libraries(deltaNote_client Qt5::Core Qt5::Widgets Qt5::Gui)