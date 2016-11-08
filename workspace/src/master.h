#ifndef MASTER_H
#define MASTER_H

#include <time.h>
#include <iostream>
#include <vector>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "referee.h"

class Master
{
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
    RTDBConn DBC;
    RoboControl robo0;
    RoboControl robo1;
    RoboControl robo2;
    RawBall ball;
    Referee referee;
    void runPenalty();
    void runGoalkeeper();
    void runStartPos();
};

#endif // MASTER_H
