#-------------------------------------------------
#
# Project created by QtCreator 2015-04-18T17:51:40
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RemoteV8Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serverlistener.cpp \
    serverreplylistener.cpp \
    clientsettingsmanager.cpp

HEADERS  += mainwindow.h \
    serverlistener.h \
    serverreplylistener.h \
    clientsettingsmanager.h

FORMS    += mainwindow.ui
