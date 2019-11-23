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
        graphicscolorsvgitem.cpp \
        login.cpp \
        main.cpp \
        mainwindow.cpp \
        newuser.cpp \
        socket.cpp \
        sqlite.cpp \
        todolistitem.cpp \
        untils.cpp \
        userinfo.cpp

HEADERS += \
        graphicscolorsvgitem.h \
        log.h \
        login.h \
        mainwindow.h \
        newuser.h \
        pack.h \
        socket.h \
        sqlite.h \
        todolistitem.h \
        untils.h \
        userinfo.h \
        sqlite3.h

LIBS += -LC:\Users\delta\Desktop\deltaNote_client\lib -lsqlite3 -lws2_32

FORMS += \
        login.ui \
        mainwindow.ui \
        newuser.ui \
        todolistitem.ui \
        userinfo.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc

RC_ICONS = deltaNote.ico

DISTFILES +=
