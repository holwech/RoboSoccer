#include "attacker.h"

Position Attacker::nextMove() {
    update();
    Position nextMove;
    switch(state) {
    case DEFEND:
        nextMove = defend();
        break;
    case GOAL_KICK:
        nextMove = goalKick();
        break;
    case GOAL_KICK_TO_TEAM:
        nextMove = goalKickToTeam();
        break;
    case IDLE:
        break;
    case MOVING:
        break;
    }
    return nextMove;
}

void Attacker::update() {
    for (int r = 0; r <= 5; r++) {
        positions[r] = robot[r]->GetPos();
    }
    ballPos = ball.GetPos();
}

AState Attacker::getState() {
    return state;
}

AState Attacker::getPrevState() {
    return prevState;
}

void Attacker::setState(AState newState) {
    prevState = state;
    state = newState;
}

/** These functions should be called doing something else probably
 *  I was just lazy, so copy paste from goalkeeper
 **/
Position Attacker::defend() {
    return Position(0,0);
}

Position Attacker::goalKick() {
    return Position(0,0);
}

Position Attacker::goalKickToTeam() {
    return Position(0,0);
}
