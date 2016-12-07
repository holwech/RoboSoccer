#ifndef MASTER_H
#define MASTER_H

#include <time.h>
#include <iostream>
#include <vector>
#include "kogmo_rtdb.hxx"
#include "robo/robo.h"
#include "referee.h"
#include "timer.h"
#include "control/collision_avoidance.h"
#include "robo/robo.h"
#include "control/pidController.h"
#include "config.cpp"

enum ePlayModePlus {
    STATE_MENU,
    STATE_GOALKEEPER,
    STATE_PENALTY,
    STATE_STARTPOS
};


class Master {
public:
    friend class Test;
<<<<<<< HEAD
    friend class Test_robo;
=======
    friend class Test_player;
>>>>>>> 11a20cb7b096079aae8197d2f06cc3eeb5c345e8
    Master(string& team,
           RTDBConn& DBC,
           Robo& robo0,
           Robo& robo1,
           Robo& robo2,
           Robo& robo3,
           Robo& robo4,
           Robo& robo5,
           RawBall& ball,
           Referee& referee);
    void run();
private:
    int client_nr;
    string team;
    eSide side;
    RTDBConn DBC;
    Robo robo0;
    Robo robo1;
    Robo robo2;
    Robo robo3;
    Robo robo4;
    Robo robo5;
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

    void updatePositions();
    void send(Command command, int roboNum);
    vector<Position> positions;
    Position ballPos;
    vector<Channel> channels;

    //Debug stuff
    timer debugTimer;
};

#endif // MASTER_H
