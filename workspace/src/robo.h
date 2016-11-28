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
                                            pidAngle(50.0, 0.5, 1),
                                            pidDistance(80.0, 0.0, 0.0){}
    Robo(RoboControl& other): RoboControl(other){}
    void run(cpp::channel<Position> position);
    CA ca;
    void driveWithCA();
    void updatePids(Position targetPos);
    void setVariables(Robo& team1, Robo& team2, Robo& otherTeam1, Robo& otherTeam2, Robo& otherTeam3);
    void updatePositions();
private:
    Position targetPosition;
//    double getObstacleAngleDiffRad(RoboControl& robo);
    double getAngleErrRad(Position targetPos);
    double getAngleWithCA(Force obstacleForce, Position targetPos);
    void updateAnglePid(Position targetPos);
    void updateDistancePid(Position targetPos);
    vector<Robo*> team;
    vector<Robo*> otherTeam;
    vector<Position> posTeam;
    vector<Position> posOtherTeam;
    double angleErrorRad;
};




#endif // ROBO_H
