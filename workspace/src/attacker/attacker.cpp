#include "attacker.h"


void Attacker::run() {
   while(1) {
       switch(state) {
        case A_DEFEND:
            break;
        case A_GOAL_KICK:
            break;
        case A_GOAL_KICK_TO_TEAM:
            break;
        case A_IDLE:
            break;
        case A_MOVING:
            break;
       }
   }
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
