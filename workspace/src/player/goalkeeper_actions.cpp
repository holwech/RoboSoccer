#include "player/player.h"

void Player::defend_tom(){
    //int x=ball.GetX();
    double x = ball.GetX();
    double y = ball.GetY();
    Angle dir=ball.GetPhi();

    if (y<-0.23 || y>0.21 || x>=1.35){
        y = ball.GetY();
        dir=ball.GetPhi();
        cout << "dir:" << dir << endl;
        cout << "staying" << endl;
    }
    else{
        y = ball.GetY();
        dir = ball.GetPhi();
        if(dir > 0 ){
            robo.CruisetoXY(1.35,ball.GetY()+0.1,170);
        }
        if(dir < 0){
            robo.CruisetoXY(1.35,ball.GetY()-0.1,170);
        }
        else{
            robo.CruisetoXY(1.35,ball.GetY(),170);
        }
    }
}

void Player::defend()
{
    int i= 0;
    double ballangle = 0;
    double ballx = 0;
    double bally= 0;
    double delta;
    Position POS(0,0);
    Position Mid(1.4,0);


    for(i=0;i<10;i++)
    {
        ballangle = ball.GetPhi().Deg()+ballangle/10;
    }

    for(i=0;i<10;i++)
    {
        ballx = ballx + ball.GetX()/10;
        bally = bally + ball.GetY()/10;
    }


    double goalkeeperx =  1.39 ;//1.36
    double goalkeepery = 0;


    goalkeepery = tan(ballangle*3.141593/180)*(goalkeeperx-ballx)+bally;


//    if (ball.GetPhi().Deg()>=80)
//    {
//        if(bally>0)
//            goalkeepery = 0.1;
//        else
//            goalkeepery = -0.1;
//    }
//    if (ball.GetPhi().Deg()<-80)
//    {
//        if(bally>0)
//            goalkeepery = 0.1;
//        else
//            goalkeepery = -0.1;
//    }
    if (goalkeepery == bally)
    {
        if(ballx>0.7)
        {
            if(bally>0.3)
                goalkeepery = 0.1;
            else if(bally<-0.3)
                goalkeepery = -0.1;
            else
                goalkeepery = 0;
        }
        else
        {
            goalkeepery = 0;
        }
    }

    if(goalkeepery>0.26)
        goalkeepery = 0.15;

    if(goalkeepery<-0.245)
        goalkeepery = -0.15;

   // go towards the ball if the ball is somewhere near the penalty area and moving not very fast
/*
    if(ball->GetPos().DistanceTo(Mid)<0.5)
    {
        if(ball->GetVelocity()<0.005)
        {
            goalkeepery = bally;
            goalkeeperx = ballx;
        }
    }
*/
    /*
    if(ballx>1.26)
    {
        if(fabs(bally)<0.3)
        {
            if(ball->GetVelocity()<0.005)
            {
                goalkeepery = bally;
                goalkeeperx = ballx;
            }
        }
    }
*/


    POS.SetX(goalkeeperx);
    POS.SetY(goalkeepery);
    robo.GotoPos(POS);
 //   robo.CruisetoXY(goalkeeperx,goalkeepery,120);

    cout << "Goaly_y: " << goalkeepery << endl;
     delta = sqrt((robo.GetX()-goalkeeperx)*(robo.GetX()-goalkeeperx)+(robo.GetY()-goalkeepery)*(robo.GetY()-goalkeepery));
     if(delta<0.06) robo.StopAction();

/*     while(robo->GetPos().DistanceTo(ball.GetPos())<0.2)
         robo->GotoXY(ball.GetX(),ball.GetY(),160);
*/
}



void Player::goalkeeperkick(){


}
