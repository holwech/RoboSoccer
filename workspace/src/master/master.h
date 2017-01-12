#ifndef MASTER_H
#define MASTER_H

#include <time.h>
#include <iostream>
#include <vector>
#include "kogmo_rtdb.hxx"
#include "referee.h"
#include "raw_ball.h"
#include "player/player.h"
#include "timer.h"
#include "config.cpp"
#include "thread"

enum ePlayModePlus {
    STATE_MENU,
    STATE_GOALKEEPER,
    STATE_PENALTY,
    STATE_STARTPOS
};

/** Different strategy states. */
enum S_State {
    AGGRESSIVE,
    DEFENSIVE,
    BALANCED
};

enum S_Case {
    INIT,
    BLOCK,
    COUNTER,
    SHOOT,
    SHOOT_AT_GOAL,
    POSITION,
    INTERRUPT,
    WAIT
};

enum T_State {
    STEP1,
    STEP2,
    STEP3,
    STEP4,
    STEP5,
    STEP6
};


class Master {
public:
    friend class Test;
    Master(string& team, RTDBConn& DBC, vector<int>& rfNumber);
    void run();
private:
    void resetTVariables();
    void strategies();
    void strategyController();
    void manual();
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
    bool tacticDone;
    int robonr; // used for tactics: near_penalty
    double chrossandpassy; // used for tactics: Chross and Pass


    /** These are variables that all strategies can and should use.
     * 	These variables will not conflict because only one strategy can be run at
     * 	the same time
     */
    S_State s_state;
    S_Case s_case;
    timer s_timer;
    void strategy_defensive();
    void strategy_offensive();
    void strategy_offensive2();
    void strategy_demo();


    /** Please prefix all tactic-specific variables with some kind of identifier
     * 	so that we get conflicting variables with the same name. Tactic variables
     * 	should be unique since multiple tactics can be run at the same time.
     */

    // Some variables can be shared if you are sure that it will not be used multiple places
    // at the same time. Some tactics for example can only run alone, like the cross pass and
    // shoot tactic.
    void exampleTactic();

    bool crossPassAndShoot();
    bool tactic_nearpenaltyarea(double threshold);
    bool tactic_ballchasing();
    bool kickAtGoal();

    /** SHARED TACTIC VARIABLES */
    T_State t_state;
    int closestRobo;
    double maxDistance;
    Position t_target;


    // crossPassAndShoot-variables
    int cps_state; // Example of a prefixed variable

};

#endif // MASTER_H
