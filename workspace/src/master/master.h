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
    friend class Test_robo;
    friend class Test_player;
    Master(string& team, RTDBConn& DBC, RawBall& ball, Referee& referee, vector<Robo>& robo, vector<int>& rfNumbers);
    void run();
private:
    int client_nr;
    string team;
    eSide side;
    RTDBConn DBC;
    RawBall ball;
    Referee referee;
    vector<Robo> robo;
    bool shotCompleted;
    bool shooterInitFirstStepDone;
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
