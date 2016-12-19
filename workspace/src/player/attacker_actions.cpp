#include "player/player.h"




void Player::blockBall(int xcord){

    cout << "Inside blockball" << endl;
    Position pos(xcord,ball.GetY());
    if(!robo.isArrived()){
        robo.GotoPos(pos,1);
    } else {
        done();
    }

}
