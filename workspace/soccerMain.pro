TEMPLATE = app
TARGET = soccerMain

include(/DIST/lehre/lab_roso/tech/usr/include/settings.pri)
#include(/DIST/lehre/lab_roso/tech/usr_sim/include/settings.pri)


DEPENDPATH += src
INCLUDEPATH += src

DESTDIR = bin
MOC_DIR = .moc
OBJECTS_DIR = .obj
#QT += core
#QT -= gui
CONFIG +=  debug


# Input
HEADERS += \ 
    src/robo.h \
    src/master.h \
    src/timer.h
SOURCES += src/soccerMain.cpp \
    src/robo.cpp \
    src/master.cpp \
    src/timer.cpp
		
		
##############
## Documentation
##############
# custom target 'doc' in *.pro file
dox.target = doc
dox.commands = doxygen Doxyfile
dox.depends = FORCE

# somewhere else in the *.pro file
QMAKE_EXTRA_TARGETS += dox







