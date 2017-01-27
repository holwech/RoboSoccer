#ifndef ROBO_H
#define ROBO_H

#include "robo_control.h"
#include "control/pidController.h"
#include "raw_ball.h"
#include "angle.h"
#include "position.h"
#include "math.h"
#include "control/collision_avoidance.h"
#include "ball/ball.h"

#define ANGLE_KP_TURN 40
#define ANGLE_KI_TURN 3
#define ANGLE_KD_TURN 1

#define ANGLE_KP_DRIVE 70
#define ANGLE_KI_DRIVE 0.5
#define ANGLE_KD_DRIVE 1

class Robo: public RoboControl
{
public:
    Robo(RTDBConn& DBC, const int deviceNr): RoboControl(DBC, deviceNr),
                                            pidAngle(ANGLE_KP_DRIVE, ANGLE_KI_DRIVE, ANGLE_KD_DRIVE),
                                            pidDistance(80.0, 0.0, 0.0),
                                            ca(),
                                             sampeled_pos(0,0),
                                            not_moving_count(0),
                                            go_to_mid(false),
                                            go_to_mid_count(0),
                                            rfNumber(deviceNr),
                                            posTeam({Position(0.0, 0.0), Position(0.0, 0.0)}),
                                            posOtherTeam({Position(0.0, 0.0), Position(0.0, 0.0), Position(0.0, 0.0)}),
                                            ball(DBC),
                                            ballBehindRobo(false),
                                            onlyTurn(false),
                                            isIdle(true),
                                            avoidBall(false),
                                            precise(false),
                                            toggleCA(false){}
    //Robo(RoboControl& other): RoboControl(other){}
    pidController pidAngle;
    pidController pidDistance;
    CA ca;
    void GotoPos(Position target, double speed = 1, bool ca = false);
    void driveWithCA();
    void goalieDrive();
    void updatePids(Position targetPos, bool ca);
    void updatePidsGoalie(Position targetPos);
    void updatePositions(vector<Position> positions);
    void turn(Position targetPos);
    bool isArrived(double radius);
    void idle();
    void stop();
    void setAvoidBall(bool avoid);
    void setPrecise(bool val);
    Position movePosInBounce(Position pos);
private:
    Position sampeled_pos;
    int not_moving_count;
    bool go_to_mid;
    int go_to_mid_count;

    Position targetPosition;
    int rfNumber;
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
    vector<Position> posTeam;
    vector<Position> posOtherTeam;
    Ball ball;
    double angleErrorRad;
    bool ballBehindRobo;
    bool onlyTurn;
    double speed;
    bool isIdle;
    bool avoidBall;
    bool avoidAll;
    bool precise;
    bool toggleCA;
    bool isGoalkeeper;
};

#endif // ROBO_H
