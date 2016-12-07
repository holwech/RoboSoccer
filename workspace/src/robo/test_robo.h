#ifndef TEST_ROBO_H
#define TEST_ROBO_H
#include "master/master.h"
class Test_robo
{
    Master master;
public:
    Test_robo(Master &m): master(m){}
    void testDriveWithCA();
    void testGoalieDrive();
    void run();
};

#endif // TEST_ROBO_H
