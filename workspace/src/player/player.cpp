#include "player.h"

Player::Player(vector<Position>* positions, Position* ball, Channel* channel, Robo* robo) :  positions(positions), ball(ball), channel(channel), robo(robo) {
    state = IDLE;
}

void Player::run() {
   while(1) {
       switch(state) {
       case IDLE:
           break;
       case BEFORE_PASS:
           break;
       case PASS:
           break;
       case GOTO:
           break;
       case BEFORE_KICK:
           break;
       case KICK:
           break;
       case BLOCK_BALL:
           break;
       case DEFEND:
           break;
       case KICK_OUT:
           break;
       case TEST:
           break;
       default:
           break;
       }
       readCommand();
   }
}

void Player::readCommand() {
    command = channel->read();

    switch(command.action) {
    case ACTION_GOTO:
        setState(GOTO);
        robo->GotoPos(command.target);
    case ACTION_TEST:
        setState(TEST);
        break;
    default:
        break;
    }

}


void Player::goTo() {
    if (robo->GetPos().DistanceTo(command.target) < 0.2) {
        state = IDLE;
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

PState Player::getState() {
    return state;
}

PState Player::getPrevState() {
    return prevState;
}

void Player::setState(PState newState) {
    prevState = state;
    state = newState;
}

/** These functions should be called doing something else probably
 *  I was just lazy, so copy paste from goalkeeper
 **/
Position Player::defend() {
    return Position(0,0);
}

Position Player::goalKick() {
    return Position(0,0);
}

Position Player::goalKickToTeam() {
    return Position(0,0);
}
