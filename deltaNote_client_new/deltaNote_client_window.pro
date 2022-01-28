#-------------------------------------------------
#
# Project created by QtCreator 2019-11-18T19:11:49
#
#-------------------------------------------------

QT       += core gui svgwidgets xml core5compat
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = deltaNote
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES +=                                      \
    client/utils/c_inter_var.cpp                \
    client/ui/main/mainwindow.cpp               \
    client/ui/main/todo_detail.cpp              \
    client/ui/main/todolistwidget.cpp           \
    client/ui/main/todolistitem.cpp             \
    client/ui/setting/login.cpp                 \
    client/ui/setting/choosecolor.cpp           \
    client/ui/setting/newuser.cpp               \
    client/ui/info/notify.cpp                   \
    client/ui/utils/svg_color.cpp               \
    utils/inter_var.cpp                         \
    sqlite/sql_base.cpp                         \
    sqlite/sql_key_value.cpp                    \
    sqlite/sql_todo_list.cpp                    \
    http/http.cpp                               \
    client/logic/setting_ctrl.cpp               \
    client/logic/c_data_ctrl.cpp                \
    client/logic/c_sync_data.cpp                \
    client/logic/reminder.cpp                   \
    client/logic/main.cpp

HEADERS +=                                      \
    client/utils/c_language.h                   \
    client/utils/c_inter_var.h                  \
    client/ui/main/mainwindow.h                 \
    client/ui/main/todo_detail.h                \
    client/ui/main/todolistwidget.h             \
    client/ui/main/todolistitem.h               \
    client/ui/setting/login.h                   \
    client/ui/setting/choosecolor.h             \
    client/ui/setting/newuser.h                 \
    client/ui/info/notify.h                     \
    client/ui/utils/svg_color.h                 \
    utils/inter_var.h                           \
    sqlite/sql_base.h                           \
    sqlite/sql_key_value.h                      \
    sqlite/sql_todo_list.h                      \
    http/http.h                                 \
    client/logic/setting_ctrl.h                 \
    client/logic/c_data_ctrl.h                  \
    client/logic/reminder.h                     \
    client/logic/c_sync_data.h

INCLUDEPATH +=                                  \
    $$PWD/log                                   \
    $$PWD/utils                                 \
    $$PWD/sqlite                                \
    $$PWD/http                                  \
    $$PWD/client/utils                          \
    $$PWD/client/logic                          \
    $$PWD/client/ui/main                        \
    $$PWD/client/ui/setting                     \
    $$PWD/client/ui/info                        \
    $$PWD/client/ui/utils

FORMS +=                                        \
        client/ui/main/mainwindow.ui            \
        client/ui/main/todo_detail.ui           \
        client/ui/main/todolistitem.ui          \
        client/ui/setting/login.ui              \
        client/ui/setting/newuser.ui            \
        client/ui/setting/choosecolor.ui        \
        client/ui/info/notify.ui

UI_DIR =                                        \
        client/ui/main/                         \
        client/ui/setting/                      \
        client/ui/info/

RESOURCES +=                                    \
    client/ui/icon.qrc

RC_ICONS = client/ui/deltaNote.ico

DISTFILES +=

### 3rdparty
# sqlite3
INCLUDEPATH += $$PWD/3rdparty/sqlite/include
LIBS += -L$$PWD/3rdparty/sqlite/bin/ -lsqlite3
# dlog
INCLUDEPATH += $$PWD/3rdparty/dlog/include
LIBS += -L$$PWD/3rdparty/dlog/bin -ldlog
# jsoncpp
INCLUDEPATH += $$PWD/3rdparty/jsoncpp/include
LIBS += -L$$PWD/3rdparty/jsoncpp/bin -ljsoncpp
# base64
INCLUDEPATH += $$PWD/3rdparty/base64/include
LIBS += -L$$PWD/3rdparty/base64/bin -lbase64
# HTTP lib
INCLUDEPATH += $$PWD/3rdparty/httplib/include
LIBS += -lws2_32

DEFINES += WINDOW_CLIENT

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

