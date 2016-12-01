TEMPLATE = app
TARGET = soccerMain

include(/DIST/lehre/lab_roso/tech/usr/include/settings.pri)
#include(/DIST/lehre/lab_roso/tech/usr_sim/include/settings.pri)

QMAKE_CXXFLAGS += -std=gnu++11 -fpermissive

DEPENDPATH += src
INCLUDEPATH += src

DESTDIR = bin
MOC_DIR = .moc
OBJECTS_DIR = .obj
#QT += core
#QT -= gui
CONFIG +=  debug


# Input
SOURCES += src/soccerMain.cpp \
    src/master/master.h \
    src/master/master.cpp \
    src/robo/robo.h \
    src/robo/robo.cpp \
    src/control/collision_avoidance.h \
    src/control/collision_avoidance.cpp \
    src/control/pidController.h \
    src/control/pidController.cpp \
    src/goalkeeper/goalkeeper.h \
    src/goalkeeper/goalkeeper.cpp \
    src/goalkeeper/test_goalkeeper.h \
    src/goalkeeper/test_goalkeeper.cpp \
    src/attacker/attacker.h \
    src/attacker/attacker.cpp \
    src/attacker/test_attacker.h \
    src/timer.h \
    src/timer.cpp \
    src/test.h \
    src/test.cpp \
    src/attacker/test_attacker.cpp \
    src/config.cpp

		
##############
## Documentation
##############
# custom target 'doc' in *.pro file
dox.target = doc
dox.commands = doxygen Doxyfile
dox.depends = FORCE

# somewhere else in the *.pro file
QMAKE_EXTRA_TARGETS += dox


