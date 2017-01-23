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
    goalkeeperx = 1.39*side;

    //get position of ball and angle of the ball
    for(i=0;i<10;i++)
    {
        ballangle = ball.GetPhi().Deg()+ballangle/10;
    }


    for(i=0;i<10;i++)
    {
        ballx += ball.GetX()/10;
        bally += ball.GetY()/10;
    }

// if the ball is moving towards our gate, goalie moves. If the ball is moving far from gate, goalie goes to the initial position
    if(ball.GetPhi().Deg()<90 && ball.GetPhi().Deg()>-90 && side)
    {
        goalkeepery = tan(ballangle*M_PI/180)*(goalkeeperx-ballx)+bally;
    }

    if(!(ball.GetPhi().Deg()<90 && ball.GetPhi().Deg()>-90) && !side)
    {
        goalkeepery = tan(ballangle*M_PI/180)*(goalkeeperx-ballx)+bally;
    }





/*    if (ball.GetPhi().Deg()>=80||ball.GetPhi().Deg()<-80)
    {
        if(bally>0)
            goalkeepery = 0.1;
        else
            goalkeepery = -0.1;
    }
*/
    if (ball.isStopped())
    {
        goalkeepery = 0;
    }


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

    if(goalkeepery>0.26)
        goalkeepery = 0.15;
    else if(goalkeepery<-0.245)
        goalkeepery = -0.15;

    //if not mooving and far out, dont move
        POS.SetX(goalkeeperx);
        POS.SetY(goalkeepery);

        robo.GotoPos(POS, 1);
//        robo.CruisetoXY(goalkeeperx,goalkeepery,130);





    //distance to move too small, dont try to move
    //delta = sqrt((robo.GetX()-goalkeeperx)*(robo.GetX()-goalkeeperx)+(robo.GetY()-goalkeepery)*(robo.GetY()-goalkeepery));
    //if(delta<0.06) robo.StopAction();
       // cout<<"ballangle"<<ball.GetPhi().Deg()<<endl;

}



void Player::goalkeeperkick(){

}
