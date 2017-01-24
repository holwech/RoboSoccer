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

/*--------------------clammer in if sentence
void Player::nextDefenderState() {
    if(ball.isStopped() && ball.inGoalArea() && fabs(goalkeeperx)-fabs(ballx)>0 && fabs(goalkeeperx)-fabs(ballx)<0.2 )
    {
        defender_state = STEP1;
    } else {

    }
}
*/

double Goalie_y(double goalkeeperx, double goalkeepery,double ballx,double bally,double ballangle, Ball ball, fieldSide side)
{
    // if the ball stops
        if (ball.isStopped())
        {
            goalkeepery = 0;
        }
    // if the ball is moving towards our gate, goalie moves to block the ball. If the ball is moving far from gate, goalie goes to the initial position
    // if the ball stops, the goalie will follow the y-axis of the ball
    if(ball.GetPhi().Deg()<90 && ball.GetPhi().Deg()>-90 && side)
    {
        goalkeepery = tan(ballangle*M_PI/180)*((goalkeeperx-0.047*side)-ballx)+bally;//------------------
    }

    else if(!(ball.GetPhi().Deg()<90 && ball.GetPhi().Deg()>-90) && !side)
    {
        goalkeepery = tan(ballangle*M_PI/180)*((goalkeeperx-0.047*side)-ballx)+bally;//------------------
    }

    if(goalkeepery>0.17)
        goalkeepery = 0.17;
    else if(goalkeepery<-0.17)
        goalkeepery = -0.17;

    return goalkeepery;
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
    Mid = Position(1.42*side,0);
    goalkeeperx = 1.42*side;

    //get position and angle of the ball
    for(i=0;i<10;i++)
    {
        ballangle = ball.GetPhi().Deg()+ballangle;
    }
    ballangle /= 10;
    for(i=0;i<10;i++)
    {
        ballx += ball.GetX();
        bally += ball.GetY();
    }
    ballx /= 10; //------change
    bally /= 10;//-------change

    switch (defender_state)
    {
      case A_STEP2:{
        // Kick the ball when the ball is within the penalty area
        bool goToDone = goTo(ballx,bally,160);
        cout<<"state2"<<endl;
        if (goToDone) {
            defender_state = A_STEP1;
        }

        break;
    }
      case A_STEP1:
        {
        goalkeepery = Goalie_y(goalkeeperx, goalkeepery, ballx, bally, ballangle, ball, side);

        POS.SetX(goalkeeperx);
        POS.SetY(goalkeepery);
        cout<<"state1"<<endl;
        robo.GotoPos(POS, 0.9);
        cout<<"2"<<endl;
//          nextDefenderState();
        /*
        if(ball.isStopped() && ball.inGoalArea() && fabs(goalkeeperx)-fabs(ballx)>0 && fabs(goalkeeperx)-fabs(ballx)<0.2 )
        {
            defender_state = A_STEP2;
        }*/
                 cout<<"3"<<endl;
          break;
        }

      default:
        cout << "No case for this step in action pass function" << endl;
        break;
    }


    }




void Player::goalkeeperkick(){

}
