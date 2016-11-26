#ifndef TEST_H
#define TEST_H

#include "robo_control.h"
#include "collision_avoidance.h"
#include "timer.h"
#include "master.h"
#include "pidController.h"
#include "robo.h"

class Test
{
public:
    Test(Master& master);
    void testMenu();
private:
    void milestone21part1();
    void pullVector();
    void collisionAvoidance(RoboControl& roboMove, RoboControl& roboObs);
    void getPassSide();
    void testAll();
    void beforePenalty();
    void penalty();
    void goalkeeper();
    void pidCollision(RoboControl &robo, RawBall &ball, RoboControl& obstacle);
    void do_goalkeeper_kick(RoboControl& robogoalkicker, RoboControl& robo_blue_1, RoboControl& robo_blue_2,RoboControl& robo_blue_3, RawBall& ourball);
    Master master;
};


#endif // TEST_H
