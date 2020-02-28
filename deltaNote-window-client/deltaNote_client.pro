#-------------------------------------------------
#
# Project created by QtCreator 2019-11-18T19:11:49
#
#-------------------------------------------------

QT       += core gui svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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

SOURCES += \
        choosecolor.cpp \
        graphicscolorsvgitem.cpp \
        login.cpp \
        main.cpp \
        mainwindow.cpp \
        newuser.cpp \
        notify.cpp \
        socket.cpp \
        sqlite.cpp \
        todolistitem.cpp \
        untils.cpp

HEADERS += \
        choosecolor.h \
        graphicscolorsvgitem.h \
        log.h \
        login.h \
        mainwindow.h \
        newuser.h \
        notify.h \
        pack.h \
        socket.h \
        sqlite.h \
        stringTable.h \
        todolistitem.h \
        untils.h \
        sqlite3.h

LIBS += -LH:\deltaNote\deltaNote-window-client\lib -lsqlite3 -lws2_32

FORMS += \
        choosecolor.ui \
        login.ui \
        mainwindow.ui \
        newuser.ui \
        notify.ui \
        todolistitem.ui

UI_DIR = H:\deltaNote\deltaNote-window-client

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc

RC_ICONS = deltaNote.ico

DISTFILES +=
