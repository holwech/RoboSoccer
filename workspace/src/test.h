#ifndef TEST_H
#define TEST_H

#include "robo_control.h"
#include "collision_avoidance.h"
#include "timer.h"
#include "master.h"
#include "pidController.h"
#include "robo.h"
#include <thread>

class Test
{
public:
    Test(Master& master);
    void testMenu();
private:
    void pullVector();
    void collisionAvoidance(RoboControl& roboMove, RoboControl& roboObs);
    void getPassSide();
    void testAll();
    void beforePenalty();
    void goalkeeper();
    void pidCollision(RoboControl &robo, RawBall &ball, RoboControl& obstacle);
    Master master;
};


#endif // TEST_H
