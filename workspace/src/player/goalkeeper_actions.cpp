#include "player/player.h"
#include "cmath"

void Player::defend_tom(){
    //int x=ball.GetX();
    double x = ball.GetX();
    double y = ball.GetY();
    Angle dir=ball.GetPhi();

    if (y<-0.23 || y>0.23 || x>=1.35){
        y = ball.GetY();
        dir=ball.GetPhi();
        cout << "dir:" << dir << endl;
        cout << "staying" << endl;
    }
    else{
        y = ball.GetY();
        dir = ball.GetPhi();
        if(dir > 0 ){
            robo.GotoPos(1.35,ball.GetY()+0.1);
        }
        if(dir < 0){
            robo.GotoPos(1.35,ball.GetY()-0.1);
        }
        else{
            robo.GotoPos(1.35,ball.GetY());
        }
    }
}

void Player::defend()
{
    int i= 0;
    double ballangle = 0;
    double ballx = 0;
    double bally = 0;
    //double delta;
    Position POS(0,0);
    double goalkeepery = 0;
    Position Mid;
    double goalkeeperx;

    //initial goalkeeper position depending on which side
    Mid = Position(1.4*side,0);
    goalkeeperx = 1.4*side;

    //get position and angle of the ball
    if (ball.isStopped())
    {
        goalkeepery = 0;
    }

    for(i=0;i<10;i++)
    {
        ballangle = ball.GetPhi().Deg()+ballangle/10;
    }
    for(i=0;i<10;i++)
    {
        ballx += ball.GetX()/10;
        bally += ball.GetY()/10;
    }

// if the ball stops
    if (ball.isStopped())
    {
        goalkeepery = 0;
    }

// if the ball is too near to our penalty area, goalkeepery = bally
    if(ballx>1.1||ballx<-1.1)
    {
        goalkeepery = bally;
    }

// if the ball is moving towards our gate, goalie moves. If the ball is moving far from gate, goalie goes to the initial position

    if(ball.GetPhi().Deg()<90 && ball.GetPhi().Deg()>-90 && side)
    {
        goalkeepery = tan(ballangle*M_PI/180)*((goalkeeperx-0.042*side)-ballx)+bally;
    }

    else if(!(ball.GetPhi().Deg()<90 && ball.GetPhi().Deg()>-90) && !side)
    {
        goalkeepery = tan(ballangle*M_PI/180)*((goalkeeperx-0.042*side)-ballx)+bally;
    }


/*    if (ball.GetPhi().Deg()>=80||ball.GetPhi().Deg()<-80)
    {
        if(bally>0)
            goalkeepery = 0.1;
        else
            goalkeepery = -0.1;
    }
*/


//    if(side==RIGHT&&)
//    {
//        if(ballx>0.7)
//        {
//            if(bally>0.3)
//                goalkeepery = 0.1;
//            else if(bally<-0.3)
//                goalkeepery = -0.1;
//            else
//                goalkeepery = 0;
//        }
//        else
//        {
//            goalkeepery = 0;
//        }
//    }

// not let goalie go away from penalty area


    if(goalkeepery>0.15)
        goalkeepery = 0.15;
    else if(goalkeepery<-0.16)
        goalkeepery = -0.16;

    cout << "y pos " << goalkeepery<<endl;


        POS.SetX(goalkeeperx);
        POS.SetY(goalkeepery);

        robo.GotoPos(POS, 1);
//        robo.CruisetoXY(goalkeeperx,goalkeepery,130);

        if(ball.isStopped() && ball.inGoalArea())
        {
            // I do not know whether I can use T_State
        }




    //distance to move too small, dont try to move
    //delta = sqrt((robo.GetX()-goalkeeperx)*(robo.GetX()-goalkeeperx)+(robo.GetY()-goalkeepery)*(robo.GetY()-goalkeepery));
    //if(delta<0.06) robo.StopAction();
<<<<<<< HEAD
//        cout<<"ballangle"<<ball.GetPhi().Deg()<<endl;
=======
       // cout<<"ballangle"<<ball.GetPhi().Deg()<<endl;
>>>>>>> 0ff354f440c319a6d8cd73612fcb1dc74c37346e

}



void Player::goalkeeperkick(){

}
