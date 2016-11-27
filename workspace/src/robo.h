#ifndef ROBO_H
#define ROBO_H

#include "robo_control.h"
#include "pidController.h"
#include "raw_ball.h"
#include "angle.h"
#include "position.h"
#include "math.h"
#include "channel.h"
#include "collision_avoidance.h"

class Robo: public RoboControl
{
public:
    pidController pidAngle;
    pidController pidDistance;
    Robo(RTDBConn& DBC, const int deviceNr): RoboControl(DBC, deviceNr),
                                            pidAngle( 35, 3, 1),
                                            pidDistance( 150, 0,0){}
    Robo(RoboControl& other): RoboControl(other){}
    void run(cpp::channel<Position> position);
    CA ca;
    void driveWithCA(RoboControl& robo1, RawBall &ball, Position  obstPos,pidController &pidAngle, pidController &pidDistance);

private:
    Position targetPosition;
    double getObstacleAngleDiffRad(RoboControl& robo, Position obstPos);
    double getAngleErrRad(Position targetPos, Position obstPos);
    double getAngleWithCA(Force obstacleForce, Position targetPos);
};




#endif // ROBO_H
