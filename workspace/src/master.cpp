#include "master.h"

Master::Master(string& team,
               RTDBConn& DBC,
               RoboControl& robo0,
               RoboControl& robo1,
               RoboControl& robo2,
               RawBall& ball) :
                team(team),
                DBC(DBC),
                robo0(robo0),
                robo1(robo1),
                robo2(robo2),
                ball(ball)
{

}
