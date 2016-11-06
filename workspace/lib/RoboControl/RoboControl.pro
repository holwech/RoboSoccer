#-------------------------------------------------
#
# Project created by QtCreator 2016-11-06T14:39:00
#
#-------------------------------------------------

QT       -= gui

TARGET = RoboControl
TEMPLATE = lib
CONFIG += staticlib

SOURCES += robocontrol.cpp

HEADERS += robocontrol.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
