#ifndef MASTER_H
#define MASTER_H

#include <time.h>
#include <iostream>
#include <vector>
#include "kogmo_rtdb.hxx"
#include "referee.h"
#include "player/player.h"
#include "timer.h"
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
    Master(string& team, RTDBConn& DBC, vector<int>& rfNumber);
    void run();
private:
    int client_nr;
    string team;
    eSide side;
    RawBall ball;
    Referee referee;
    ePlayMode state;
    void updatePositions();
    void send(Command command, int roboNum);
    vector<Channel> channel;
    vector<Player> player;
    vector<Position> positions;
    Position ballPos;

};

#endif // MASTER_H
