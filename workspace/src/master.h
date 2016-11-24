#ifndef MASTER_H
#define MASTER_H

#include <time.h>
#include <iostream>
#include <vector>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "referee.h"
#include "timer.h"
#include "collision_avoidance.h"
#include "robo.h"
#include "pidController.h"

enum ePlayModePlus {
    STATE_MENU,
    STATE_GOALKEEPER,
    STATE_PENALTY,
    STATE_STARTPOS
};


class Master {
public:
    friend class Test;
    Master(string& team,
           RTDBConn& DBC,
           RoboControl& robo0,
           RoboControl& robo1,
           RoboControl& robo2,
           RoboControl& robo3,
           RoboControl& robo4,
           RoboControl& robo5,
           RawBall& ball,
           Referee& referee);
    void run();
private:
    int client_nr;
    string team;
    eSide side;
    RTDBConn DBC;
    RoboControl robo0;
    RoboControl robo1;
    RoboControl robo2;
    RoboControl robo3;
    RoboControl robo4;
    RoboControl robo5;
    RawBall ball;
    Referee referee;
    CA ca;
    bool shotCompleted;
    bool shooterInitFirstStepDone;
    void updateFieldSide();
    void testMenu();
    void runPenalty();
    void penaltyShoot();
    void runGoalkeeper();
    void runStartPos();
    void beforePenalty();
    void penalty();
    void printInfo();
    ePlayMode state;

    //Debug stuff
    timer debugTimer;
};

#endif // MASTER_H
