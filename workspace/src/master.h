#ifndef MASTER_H
#define MASTER_H

#include <time.h>
#include <iostream>
#include <vector>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"

class Master
{
public:
    Master(string& team,
           RTDBConn& DBC,
           RoboControl& robo0,
           RoboControl& robo1,
           RoboControl& robo2,
           RawBall& ball);
private:
    int client_nr;
    string team;
    RTDBConn DBC;
    RoboControl robo0;
    RoboControl robo1;
    RoboControl robo2;
    RawBall ball;
};

#endif // MASTER_H
