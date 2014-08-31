#-------------------------------------------------
#
# Project created by QtCreator 2014-08-24T19:52:21
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServerSideApp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    serversessionthread.cpp

HEADERS  += mainwindow.h \
    server.h \
    serversessionthread.h

FORMS    += mainwindow.ui

RESOURCES += \
    AllResource.qrc
