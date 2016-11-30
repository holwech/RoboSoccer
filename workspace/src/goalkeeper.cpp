#include "goalkeeper.h"


Position Goalkeeper::nextMove() {
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

void Goalkeeper::update() {
    for (int r = 0; r <= 5; r++) {
        positions[r] = robot[r]->GetPos();
    }
    ballPos = ball.GetPos();
}

GState Goalkeeper::getState() {
    return state;
}

GState Goalkeeper::getPrevState() {
    return prevState;
}

void Goalkeeper::setState(GState newState) {
    prevState = state;
    state = newState;
}


Position Goalkeeper::defend() {
    return Position(0,0);
}

Position Goalkeeper::goalKick() {
    return Position(0,0);
}

Position Goalkeeper::goalKickToTeam() {
    return Position(0,0);
}
