#ifndef MASTER_H
#define MASTER_H
#define DEBUG true

#include <iostream>
#include <vector>
#include <thread>
#include "kogmo_rtdb.hxx"
#include "referee.h"
#include "ball/ball.h"
#include "player/player.h"
#include "timer.h"
#include "config.cpp"
#include "ball/test_ball.h"

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
    NEXT,
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
    Ball ball;
    Referee referee;
    ePlayMode state;
    fieldSide side;
    void updateSide();
    void updatePositions();
    void send(Command command, int roboNum);
    int getClosest(bool withKeeper = false);
    int setClosest(int prevClosest, bool resetTactic = false, bool withKeeper = false);
    void checkClosest(int currClosest, bool withKeeper = false);
    Position getOtherKeeperPos();
    int otherKeeperInGoalArea();
    int getClosestToTeamGoal();
    int getNotClosestToTeamGoal();
    vector<Channel> channel;
    vector<Player> player;
    vector<Position> positions;
    Position ballPos;
    bool tacticDone;
    double chrossandpassy; // used for tactics: Chross and Pass


    /** These are variables that all strategies can and should use.
     * 	These variables will not conflict because only one strategy can be run at
     * 	the same time
     */
    S_State s_state;
    S_Case s_case;
    timer s_timer;
    const vector<string> strategyStateNames = {"INIT", "NEXT", "BLOCK", "COUNTER", "SHOOT", "SHOOT_AT_GOAL", "POSITION", "INTERRUPT", "WAIT"};
    void debugContinue();
    void t_debugContinue();
    void strategy_defensive();
    void strategy_offensive();
    void strategy_offensive2();
    void strategy_offensive3();
    void strategy_demo();
    void nextDefensiveMove();
    void offensiveNextMove();
    bool bounceForward();
    void strategy_best();
    void nextMove();
    int getNotClosest();

    void GoToBeforePenaltyPosition(); // Used to manoever robots to the position before penalty
    void ActDuringPenalty(); //Make robots act during penalty shooting


    /** Please prefix all tactic-specific variables with some kind of identifier
     * 	so that we get conflicting variables with the same name. Tactic variables
     * 	should be unique since multiple tactics can be run at the same time.
     */

    // Some variables can be shared if you are sure that it will not be used multiple places
    // at the same time. Some tactics for example can only run alone, like the cross pass and
    // shoot tactic.
    void exampleTactic();

    bool crossPassAndShoot();
    bool tactic_nearpenaltyarea(double threshold, int playerNum = -1);
    bool tactic_ballchasing();
    bool throughPass();
    bool kickAtGoal(int playerNum = -1, bool is_penalty = false);

    /** SHARED TACTIC VARIABLES */
    T_State t_state;
    T_State t_state2;
    int closestRobo;
    int notClosestRobo;
    double maxDistance;
    Position t_target;


    // crossPassAndShoot-variables
    int cps_state; // Example of a prefixed variable
    void printRefereeStats();
};

#endif // MASTER_H
