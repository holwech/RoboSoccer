#include "player/player.h"




bool Player::blockBall(int xcord){

    cout << "Inside blockball" << endl;
    Position pos(xcord,ball.GetY());
    if(!robo.isArrived()){
        robo.GotoPos(pos,1);
    } else {
        return true; // Done
    }
    return false;
}
