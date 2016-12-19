#ifndef TEST_H
#define TEST_H
#include "robo_control.h"
#include "master/master.h"

class Test
{
public:
    Test(Master& master);
    void menu();
private:
    Master *master;
};


#endif // TEST_H
