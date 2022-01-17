#-------------------------------------------------
#
# Project created by QtCreator 2019-11-18T19:11:49
#
#-------------------------------------------------

QT       += core gui svgwidgets xml core5compat
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = deltaNote_client
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

SOURCES +=                                   \
        src/deltaNote/connectctrl.cpp        \
        src/deltaNote/listCtrl.cpp           \
        src/log/logCtrl.cpp                  \
        src/log/logManage.cpp                \
        src/common/untils.cpp                \
        src/common/socket.cpp                \
        src/common/sqlite.cpp                \
        src/common/graphicscolorsvgitem.cpp  \
        src/widget/todolistwidget.cpp        \
        src/ui/checkupdate.cpp               \
        src/ui/choosecolor.cpp               \
        src/ui/notify.cpp                    \
        src/ui/newuser.cpp                   \
        src/ui/login.cpp                     \
        src/ui/todolistitem.cpp              \
        src/ui/mainwindow.cpp                \
        src/deltaNote/packCtrl.cpp           \
        src/deltaNote/datactrl.cpp           \
        src/deltaNote/main.cpp

HEADERS +=                                  \
        src/common/language.h               \
        src/deltaNote/connectctrl.h         \
        src/deltaNote/listCtrl.h            \
        src/log/logCtrl.h                   \
        src/log/logManage.h                 \
        src/sqlite/sqlite3.h                \
        src/common/untils.h                 \
        src/common/socket.h                 \
        src/common/sqlite.h                 \
        src/common/graphicscolorsvgitem.h   \
        src/widget/todolistwidget.h         \
        src/ui/checkupdate.h                \
        src/ui/choosecolor.h                \
        src/ui/notify.h                     \
        src/ui/newuser.h                    \
        src/ui/login.h                      \
        src/ui/todolistitem.h               \
        src/ui/mainwindow.h                 \
        src/deltaNote/packCtrl.h            \
        src/deltaNote/datactrl.h


LIBS += -LH:\GitHubRepo\deltaNote\deltaNote_client\lib -lsqlite3 -lws2_32

DEFINES += WINDOW_CLIENT

INCLUDEPATH += \
        src/ \
        src/widget \
        ui/

FORMS += \
        ui/checkupdate.ui       \
        ui/choosecolor.ui       \
        ui/login.ui             \
        ui/mainwindow.ui        \
        ui/newuser.ui           \
        ui/notify.ui            \
        ui/todolistitem.ui

UI_DIR = ui/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ui/icon.qrc

RC_ICONS = ui/deltaNote.ico

DISTFILES +=
