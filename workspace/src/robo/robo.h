#ifndef ROBO_H
#define ROBO_H

#include "robo_control.h"
#include "control/pidController.h"
#include "raw_ball.h"
#include "angle.h"
#include "position.h"
#include "math.h"
#include "control/collision_avoidance.h"

class Robo: public RoboControl
{
public:
    pidController pidAngle;
    pidController pidDistance;
    Robo(RTDBConn& DBC, const int deviceNr): RoboControl(DBC, deviceNr),
                                            pidAngle(50.0, 0.5, 1),
                                            pidDistance(80.0, 0.0, 0.0){}
    Robo(RoboControl& other): RoboControl(other){}
    CA ca;
    void Goto(Position target);
    void goalieGoto(Position target);
    void driveWithCA();
    void updatePids(Position targetPos);
    void setVariables(Robo& team1, Robo& team2, Robo& otherTeam1, Robo& otherTeam2, Robo& otherTeam3);
    void updatePositions();
    void turnWithPid(Position targetPos);
private:
    Position targetPosition;
//    double getObstacleAngleDiffRad(RoboControl& robo);
    double getRealDiffRad(double angle1, double angle2);
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
