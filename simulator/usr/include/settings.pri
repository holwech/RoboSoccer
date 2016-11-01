INCLUDEPATH += /home/daniel/simulator/usr/include/rtdb  /home/daniel/simulator/usr/include/roboctrl /home/daniel/simulator/usr/include/libmath
QMAKE_CXXFLAGS += -isystem/home/daniel/simulator/usr/include/rtdb -isystem/home/daniel/simulator/usr/include/roboctrl  -isystem/home/daniel/simulator/usr/include/libmath
QMAKE_CXXFLAGS += -O2 -Wall -Wextra -Werror -Wuninitialized

#LIBS += -L/home/daniel/simulator/usr/lib \
#        -lpololu_rtdb \
#        -lkogmo_rtdb \

LIBS += -L/home/daniel/simulator/usr/lib \
#	-lpololu_rtdb \
        -L. libkogmo_rtdb.so \
        -L. libpololu_rtdb.so \
#	-lkogmo_rtdb \
