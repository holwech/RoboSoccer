#include "attacker.h"

Attacker::Attacker(vector<Position>* positions, Position* ball, Command* command, Robo* robo) : positions(positions), ball(ball), command(command), robo(robo) {
    state = A_MOVING;
}

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

/**
void moveSquare() {
    Position pos1 = {0.5, 0.5};
    Position pos2 = {-0.5, 0.5};
    Position pos3 = {-0.5, -0.5};
    Position pos4 = {0.5, -0.5};
    int step = 0;
    Position targetPos;
    while(1) {
        robo->updatePids(targetPos);
        robo->updatePositions();
        robo->driveWithCA();
        if(master.robo.GetPos().DistanceTo(targetPos) < 0.2){

        }
    }

}
*/

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
