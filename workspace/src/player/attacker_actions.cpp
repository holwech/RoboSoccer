#include "player/player.h"




/**
 * @brief Function for robot to block ball.
 *
 * @param xcord an integer giving x-coordinate.
 * @return bool a boolean to tell if action has been finished.
 */
bool Player::blockBall(int xcord){

    cout << "Inside blockball" << endl;
    Position pos(xcord,ball.GetY());
    if(!robo.isArrived(0.04)){
        robo.GotoPos(pos,1);
    } else {
        return true; // Done
    }
    return false;
}
