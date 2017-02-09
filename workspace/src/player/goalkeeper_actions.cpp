/*
 * Goalkeeper functions
 * Prevent the goal from oponent team
 * A state machine
*/
#include "player/player.h"
#include "cmath"

#define GOALKEEPER_SPEED 1.5
#define GOALKEEPER_KICK_SPEED 2


/**
 * @brief
 *
 */
void Player::getNextGoalkeeperState(){// Define the goalkeeper's next state
    //Right close corner
    Position ballpos = ball.GetPos();
    switch(Gstate){
    case BLOCK_WINDOW:      //block at window side
    case BLOCK_NOT_WINDOW:  //block at not window side
    case DYNAMIC_DEFEND:    //dynamic defend
    case GOALKEEPER_STOP:   //stop action
    case GOALKEEPER_KICK:   //kick the ball when in penalty area
        // check if ball in corners towards window
        if(ball.inGoalArea() && abs(ballpos.GetX()) > 1.4){
            Gstate = GOALKEEPER_STOP;
        }
        else if(ball.isStopped() && ball.inGoalArea(side) && (fabs(robo.GetPos().GetX())-fabs(ballx))>0.05){
            Gstate = GOALKEEPER_KICK;
        }

        else if(ballpos.GetX()*side > 1.4-abs(ballpos.GetY()) ){
            if(ballpos.GetY() < 0){
                Gstate = BLOCK_WINDOW;
            }
            else{
                Gstate = BLOCK_NOT_WINDOW;
            }
        }
        else{
            Gstate = DYNAMIC_DEFEND;
        }
    default:
        break;
    }
}

/**
 * @brief
 *
 */
void Player::defend(){
    double goalypos_x  = 0;
    double goalypos_y  = 0;
    double ballangle = 0;
    double ballx = 0;
    double bally = 0;
    bool debug = false;
    Position pos;
    if (side == LEFT){
        goalypos_x = -1.36;
    }
    else{
        goalypos_x = 1.40;
    }
    //get position and angle of the ball

    ballangle = ball.GetPhi().Deg()+ballangle;
    Position ballPos = ball.GetPos();
    ballx = ballPos.GetX();
    bally = ballPos.GetY();

    getNextGoalkeeperState();
    switch(Gstate){ //check the state and action
    case BLOCK_WINDOW:
        if(debug){
            cout << "In BLOCK_WINDOW" << endl;
        }
        goalypos_y = -0.19;
        pos.SetX(goalypos_x);
        pos.SetY(goalypos_y);
        if(robo.GetPos().DistanceTo(pos) < 0.03){
            robo.stop();
        }else{

            robo.GotoPos(pos, GOALKEEPER_SPEED);

        }
        break;
    case BLOCK_NOT_WINDOW:
        if(debug){
            cout << "In BLOCK_NOT_WINDOW" << endl;
        }
        goalypos_y = 0.19;
        pos.SetX(goalypos_x);
        pos.SetY(goalypos_y);
        if(robo.GetPos().DistanceTo(pos) < 0.03){
            robo.stop();
        }else{

            robo.GotoPos(pos, GOALKEEPER_SPEED);

        }
        break;
    case GOALKEEPER_STOP:
        if(debug){
            cout << "In GOALKEEPER_STOP" << endl;
        }
        robo.stop();
        break;
    case DYNAMIC_DEFEND:
        if(debug){
            cout << "In DYNAMIC_DEFEND" << endl;
        }
        // towards our goal
        if(side == LEFT){
            if (!(ball.GetPhi().Deg()<90 && ball.GetPhi().Deg()>-90 && abs(ballx) < 1.30)){
                goalypos_y = tan(ballangle*M_PI/180)*((goalypos_x-0.047*side)-ballx)+bally;
            }
            else{
                goalypos_y = bally;
            }
        }
        else if(side == RIGHT){
            if(ball.GetPhi().Deg()<90 && ball.GetPhi().Deg()>-90 && abs(ballx) <1.30){
                goalypos_y = tan(ballangle*M_PI/180)*((goalypos_x-0.047*side)-ballx)+bally;
            }
            else{
                goalypos_y = bally;
            }

        }
        if (goalypos_y > 0.19){
            goalypos_y = 0.19;
        }
        else if (goalypos_y < -0.19){
            goalypos_y = -0.19;
        }
        pos.SetX(goalypos_x);
        pos.SetY(goalypos_y);

        robo.GotoPos(pos, GOALKEEPER_SPEED);

        break;
    case GOALKEEPER_KICK:
        pos.SetX(ballx);
        pos.SetY(bally);
        robo.GotoPos(pos, GOALKEEPER_KICK_SPEED);
        break;
    default:
        break;
    }
}





