
QMAKE_CXXFLAGS += -O2 -Wall # -Wextra -Werror -Wuninitialized
QMAKE_CXXFLAGS += -isystem/./libraries/kogmo/include -isystem/./libraries/libmath/include  -isystem/./libraries/libpololurtdb/

TEMPLATE = app
TARGET = soccer_sample

DEPENDPATH += src
INCLUDEPATH += src
INCLUDEPATH += ./libraries/kogmo/include ./libraries/libmath/include ./libraries/libpololurtdb/include

DESTDIR = bin
MOC_DIR = .moc
OBJECTS_DIR = .obj
CONFIG +=  debug
CONFIG += thread


##############
## Documentation
##############
# custom target 'doc' in *.pro file
dox.target = doc
dox.commands = doxygen Doxyfile
dox.depends = FORCE

# somewhere else in the *.pro file
QMAKE_EXTRA_UNIX_TARGETS += do


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libraries/kogmo/lib/release/ -lkogmo_rtdb
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libraries/kogmo/lib/debug/ -lkogmo_rtdb
else:unix: LIBS += -L$$PWD/libraries/kogmo/lib/ -lkogmo_rtdb

INCLUDEPATH += $$PWD/libraries/kogmo/include
DEPENDPATH += $$PWD/libraries/kogmo/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libraries/libmath/lib/release/ -lmath
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libraries/libmath/lib/debug/ -lmath
else:unix: LIBS += -L$$PWD/libraries/libmath/lib/ -lmath

INCLUDEPATH += $$PWD/libraries/libmath/include
DEPENDPATH += $$PWD/libraries/libmath/include


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libraries/libpololurtdb/lib/release/ -lpolulu
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libraries/libpololurtdb/lib/debug/ -lpolulu
else:unix: LIBS += -L$$PWD/libraries/libpololurtdb/lib/ -lpolulu

INCLUDEPATH += $$PWD/libraries/libpololurtdb/include
DEPENDPATH += $$PWD/libraries/libpololurtdb/include

LIBS += -lrt
















