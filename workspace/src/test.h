#ifndef TEST_H
#define TEST_H

#include "robo_control.h"
#include "collision_avoidance.h"
#include "timer.h"

class Test
{
public:
    Test();
    void collisionAvoidance(RoboControl& roboMove, RoboControl& roboObs);
};


#endif // TEST_H
