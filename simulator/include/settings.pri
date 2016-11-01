INCLUDEPATH += ~/code/TUM/RoboSoccer/simulator/include/rtdb ~/code/TUM/RoboSoccer/simulator/include/roboctrl ~/code/TUM/RoboSoccer/simulator/include/libmath
QMAKE_CXXFLAGS += -isystem~/code/TUM/RoboSoccer/simulator/include/rtdb -isystem~/code/TUM/RoboSoccer/simulator/include/roboctrl  -isystem~/code/TUM/RoboSoccer/simulator/include/libmath
QMAKE_CXXFLAGS += -O2 -Wall -Wextra -Werror -Wuninitialized

LIBS += -L/DIST/lehre/lab_roso/tech/usr_sim/lib \
        -lpololu_rtdb \
        -lkogmo_rtdb \
#        -lmath \
