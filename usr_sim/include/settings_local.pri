INCLUDEPATH += ~/Spideroak Hive/RoboSoccer/usr_sim/include/rtdb ~/Spideroak Hive/RoboSoccer/usr_sim/include/roboctrl ~/Spideroak Hive/RoboSoccer/usr_sim/include/libmath
QMAKE_CXXFLAGS += -isystem~/Spideroak Hive/RoboSoccer/usr_sim/include/rtdb -isystem~/Spideroak Hive/RoboSoccer/usr_sim/include/roboctrl  -isystem~/Spideroak Hive/RoboSoccer/usr_sim/include/libmath
QMAKE_CXXFLAGS += -O2 -Wall -Wextra -Werror -Wuninitialized

LIBS += -L~/Spideroak Hive/RoboSoccer/usr_sim/lib \
        -lpololu_rtdb \
        -lkogmo_rtdb \
#        -lmath \
