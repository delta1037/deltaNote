#-------------------------------------------------
#
# Project created by QtCreator 2019-11-18T19:11:49
#
#-------------------------------------------------

QT       += core gui svg xml

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

SOURCES +=                                      \
        source/deltaNote/connectctrl.cpp        \
        source/deltaNote/listCtrl.cpp           \
        source/log/logCtrl.cpp                  \
        source/log/logManage.cpp                \
        source/common/untils.cpp                \
        source/common/socket.cpp                \
        source/common/sqlite.cpp                \
        source/common/graphicscolorsvgitem.cpp  \
        source/widget/todolistwidget.cpp        \
        source/ui/checkupdate.cpp               \
        source/ui/choosecolor.cpp               \
        source/ui/notify.cpp                    \
        source/ui/newuser.cpp                   \
        source/ui/login.cpp                     \
        source/ui/todolistitem.cpp              \
        source/ui/mainwindow.cpp                \
        source/deltaNote/packCtrl.cpp           \
        source/deltaNote/datactrl.cpp           \
        source/deltaNote/main.cpp

HEADERS +=                                      \
        include/common/language.h               \
        include/deltaNote/connectctrl.h         \
        include/deltaNote/listCtrl.h            \
        include/log/logCtrl.h                   \
        include/log/logManage.h                 \
        include/sqlite/sqlite3.h                \
        include/common/untils.h                 \
        include/common/socket.h                 \
        include/common/sqlite.h                 \
        include/common/graphicscolorsvgitem.h   \
        include/widget/todolistwidget.h         \
        include/ui/checkupdate.h                \
        include/ui/choosecolor.h                \
        include/ui/notify.h                     \
        include/ui/newuser.h                    \
        include/ui/login.h                      \
        include/ui/todolistitem.h               \
        include/ui/mainwindow.h                 \
        include/deltaNote/packCtrl.h            \
        include/deltaNote/datactrl.h


LIBS += -LH:\deltaNote\deltaNote_client\lib -lsqlite3 -lws2_32

DEFINES += WINDOW_CLIENT

INCLUDEPATH += \
        include/ \
        include/widget \
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
