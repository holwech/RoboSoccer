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
    for(i=0;i<10;i++)
    {
        ballangle = ball.GetPhi().Deg()+ballangle;
    }
    ballangle = ballangle/10;
    for(i=0;i<10;i++)
    {
        ballx += ball.GetX();
        bally += ball.GetY();
    }
    ballx = ballx/10;
    bally = bally/10;

    if(ball.isStopped() && ball.inGoalArea() && (fabs(goalkeeperx)-fabs(ballx))>0 && (fabs(goalkeeperx)-fabs(ballx))<0.2 )
    {
        robo.CruisetoXY(ballx,bally,200);
    }

    else if(ball.isStopped() && ball.inGoalArea() && (fabs(goalkeeperx)-fabs(ballx))<0 )
    {
        robo.StopAction();
    }

    else
    {
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
            goalkeepery = tan(ballangle*M_PI/180)*((goalkeeperx-0.047*side)-ballx)+bally;
        }

        else if(!(ball.GetPhi().Deg()<90 && ball.GetPhi().Deg()>-90) && !side)
        {
            goalkeepery = tan(ballangle*M_PI/180)*((goalkeeperx-0.047*side)-ballx)+bally;
        }





        if(goalkeepery>0.15)
            goalkeepery = 0.15;
        else if(goalkeepery<-0.16)
            goalkeepery = -0.16;


            POS.SetX(goalkeeperx);
            POS.SetY(goalkeepery);

    //        robo.GotoPos(POS, 0.9);
            robo.CruisetoXY(goalkeeperx,goalkeepery,140);



    }

}



void Player::goalkeeperkick(){

}
