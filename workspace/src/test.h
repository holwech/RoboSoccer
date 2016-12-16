#ifndef TEST_H
#define TEST_H
#include "robo_control.h"
#include "master/master.h"
#include "robo/test_robo.h"

class Test
{
public:
    Test(Master& master);
    void menu();
private:
    Master *master;
    Test_robo tr;
};


#endif // TEST_H
