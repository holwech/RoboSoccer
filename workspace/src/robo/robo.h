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
    Robo(RTDBConn& DBC, const int deviceNr): RoboControl(DBC, deviceNr),
                                            pidAngle(50.0, 0.5, 1),
                                            pidDistance(80.0, 0.0, 0.0),
                                            ca(), ballBehindRobo(false), onlyTurn(false){}
    Robo(RoboControl& other): RoboControl(other){}
    pidController pidAngle;
    pidController pidDistance;
    CA ca;
    void GotoPos(Position target, int speed = 1);
    void driveWithCA();
    void goalieDrive();
    void updatePids(Position targetPos, bool ca);
    void updatePidsGoalie(Position targetPos);
    void setVariables(Robo& team1, Robo& team2, Robo& otherTeam1, Robo& otherTeam2, Robo& otherTeam3);
    void updatePositions();
    void turn(Position targetPos);
    bool isArrived();
private:
    Position targetPosition;
    void makeTurn();
    double getDiffBetweenAnglesRad(double angle1, double angle2);
    void updateAnglePidGoalie(Position targetPos);
    double getGoalieReferenceAngleErrRad(Position targetPos);
    double getReferenceAngleErrRad(Position targetPos, bool ca);
    double getRefAngleWithCA(Force obstacleForce, Position targetPos);
    double getRefAngleWithoutCA(Position targetPos);
    void updateAnglePidWithCA(Position targetPos);
    void updateAnglePidWithoutCA(Position targetPos);
    void updateDistancePid(Position targetPos);
    vector<Robo*> team;
    vector<Robo*> otherTeam;
    vector<Position> posTeam;
    vector<Position> posOtherTeam;
    double angleErrorRad;
    bool ballBehindRobo;
    bool onlyTurn;
    int speed;
};

#endif // ROBO_H
