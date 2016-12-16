#include "player/player.h"




void Player::blockBall(int xcord){


    Position pos(xcord,ball.GetY());
    if(!robo.isArrived()){
        robo.GotoPos(pos,150);
    } else {
        done();
    }

}
