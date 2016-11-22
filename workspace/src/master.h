#ifndef MASTER_H
#define MASTER_H

#include <time.h>
#include <iostream>
#include <vector>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "referee.h"
#include "timer.h"
#include "robo.h"
#include "pidController.h"

enum State {
    STATE_MENU,
    STATE_GOALKEEPER,
    STATE_PENALTY,
    STATE_STARTPOS
};


class Master {
public:
    Master(string& team,
           RTDBConn& DBC,
           RoboControl& robo0,
           RoboControl& robo1,
           RoboControl& robo2,
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
    RawBall ball;
    Referee referee;
    bool shotCompleted;
    bool shooterInitFirstStepDone;
    void updateFieldSide();
    void menu();
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
