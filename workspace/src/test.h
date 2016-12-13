#ifndef TEST_H
#define TEST_H

#include "robo_control.h"
#include "control/collision_avoidance.h"
#include "timer.h"
#include "master/master.h"
#include "control/pidController.h"
#include "robo/robo.h"
#include "test.h"
#include "robo/robo.h"
#include "stdlib.h"
#include "math.h"
#include "player/test_player.h"

class Test
{
public:
    Test(Master& master);
    void testMenu();
    void specializedTestMenu();
private:
    void testDriveWithCA();
    void milestone21part1();
    void pullVector();
    void collisionAvoidance(Robo& roboMove, Robo& roboObs);
    void getPassSide();
    void testAll();
    void beforePenalty();
    void penalty();
    void goalkeeper();
    void pidCollision(Robo &robo, RawBall &ball);
    void do_goalkeeper_kick(Robo& robogoalkicker, Robo& robo_blue_1, Robo& robo_blue_2,Robo& robo_blue_3, RawBall& ourball);
    void turning();
    void randomDrivingWithCA();
    void goalkeeperingame();
    void timerTest();
    Master *master;
    Test_player ta;
};


#endif // TEST_H
