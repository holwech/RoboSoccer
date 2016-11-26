#include "test.h"
#include <limits>
#include "robo.h"
#include "stdlib.h"
#include"math.h"

Test::Test(Master& master) : master(master)
{
}

void Test::testMenu() {
    while(1) {
        cout << "Choose test program: " << endl;
        cout << "0: Return to program" << endl;
        cout << "1: Print to screen" << endl;
        cout << "2: Go to (0, 0)" << endl;
        cout << "3: Test all" << endl;
        cout << "4: Goalkeeper" << endl;
        cout << "5: Collision avoidance" << endl;

        cout << "6: Obstacle" << endl;
        cout << "7: Pull vector" << endl;
        cout << "8: getPassSide" << endl;
        cout << "9: Goalkeeper's kick" << endl;
        cout << "10: Milestone 2.1 part 1" << endl;
        cout << "11: penalty shooting" << endl;
        cout << "12: Before penalty" << endl;

        int program;
        cin >> program;
        bool stop = false;

        switch(program) {
        case 0:
            stop = true;
            break;
        case 1:
            cout << "Program 1 running..." << endl;
            break;
        case 2:
            cout << "Running 2: Go to (0, 0)" << endl;
            master.robo0.GotoXY(0.0, 0.0);
            break;
        case 3:
            cout << "Running 3: Test all" << endl;
            testAll();
            break;
        case 4:
            cout << "Running 4: Goalkeeper" << endl;
            goalkeeper();
            break;
        case 5:
            cout << "Running 5: Collision avoidance (2.1 - part 1)" << endl;
            collisionAvoidance(master.robo0, master.robo1);
            break;
        case 6:
            cout << "Running 6: Obstacle" << endl;
            pidCollision(master.robo1, master.ball, master.robo2);
            break;
        case 7:
            cout << "Running 7: Pull vector" << endl;
            pullVector();
            break;
        case 8:
            cout << "Running 8: getPassSide" << endl;
            getPassSide();
            break;
        case 9:
            cout << "Running 9: Goalkeeper's kick" << endl;
            do_goalkeeper_kick(master.robo1, master.robo3, master.robo4, master.robo5, master.ball);
            break;
        case 10:
            cout << "Running 10: Milestone 2.1 part 1" << endl;
            milestone21part1();
            break;
        case 11:
            cout << "Running 11: Penalty Shooting"<<endl;
            penalty();
            break;
        case 12:
            cout << "Running 12: Before Penalty"<<endl;
            beforePenalty();
            break;
        default:
            stop = true;
            break;
        }
        if (stop) {
            break;
        }
    }
    cout << "Leaving test menu" << endl;
}

void Test::milestone21part1() {
    cout << "To run milestone 2.1 part 1, 4 positions are required." << endl;
    vector<Position> positions;
    for (int i = 0; i < 4; i++) {
        double posX, posY;
        cout << "Coordinate " << i + 1 << ", X: ";
        cin >> posX;
        cout << "Coordinate " << i + 1 << ", Y: ";
        cin >> posY;
        positions.push_back(Position(posX, posY));
    }
    cout << "Setup complete. Coordinates are:" << endl;
    cout << "1: { " << positions[0].GetX() << ", " << positions[0].GetY() << " }" << endl;
    cout << "2: { " << positions[1].GetX() << ", " << positions[1].GetY() << " }" << endl;
    cout << "3: { " << positions[2].GetX() << ", " << positions[2].GetY() << " }" << endl;
    cout << "4: { " << positions[3].GetX() << ", " << positions[3].GetY() << " }" << endl;

    master.robo0.GotoPos(positions[0]);
    master.robo1.GotoPos(positions[1]);
    master.robo2.GotoPos(positions[3]);
    int step = 1;

    while (1) {
        if (step == 1) {
            if (
                master.robo0.GetPos().DistanceTo(positions[0]) < 0.2 &&
                master.robo1.GetPos().DistanceTo(positions[1]) < 0.2 &&
                master.robo2.GetPos().DistanceTo(positions[3]) < 0.2)
            {
                cout << "Step 1 reached" << endl;
                master.robo0.GotoPos(positions[2]);
                master.robo1.GotoPos(positions[3]);
                master.robo2.GotoPos(positions[1]);
                step = 2;
            }
        } else if (step == 2) {
            if (
                master.robo0.GetPos().DistanceTo(positions[2]) < 0.2 &&
                master.robo1.GetPos().DistanceTo(positions[3]) < 0.2 &&
                master.robo2.GetPos().DistanceTo(positions[1]) < 0.2)
            {
                cout << "Step 2 reached" << endl;
                master.robo0.GotoPos(positions[0]);
                master.robo1.GotoPos(positions[1]);
                master.robo2.GotoPos(positions[3]);
                step = 1;
            }
        }
    }
}

void Test::getPassSide() {
    cout << "Remember to set debug to true in the getPassSide function" << endl;
    while (1) {
        Position basePos = master.robo0.GetPos();
        Position target = master.ball.GetPos();
        Position obstacle = master.robo1.GetPos();
        double diffAngle = master.ca.getPassSide(basePos, target, obstacle);
        diffAngle = diffAngle;
        usleep(1000);
    }
}

/**
  */
void Test::pullVector() {
    timer debugTimer;
    debugTimer.start();
    while (1) {
        Position basePos = master.robo0.GetPos();
        Position target = master.ball.GetPos();
        Position obstacle = master.robo1.GetPos();
        double passSide = master.ca.getPassSide(basePos, target, obstacle);
        Force pull = master.ca.getPull(basePos, target, obstacle);
        if (debugTimer.getTime() > 0.01) {
            cout << "----- ----- -----" << endl;
            cout << "Pass side: " << passSide << endl;
            cout << "Pull vector: { " << pull.X << " , " << pull.Y << " }" << endl;
            debugTimer.reset();
        }
        usleep(100);
    }
}

void Test::pidCollision(RoboControl &robo, RawBall &ball, RoboControl &obstacle){
    pidController pidAngle(15.0, 0, 1.0);
    pidController pidDistance(80.0, 0.0, 0.0);
    while(1){
        driveWithCA(robo, ball, obstacle.GetPos(), pidAngle, pidDistance);
    }

}

void Test::collisionAvoidance(RoboControl& roboMove, RoboControl& roboObs) {
    roboObs.GotoXY(0.0, 0.0, 60);
    CA ca;
    timer debugTimer;
    debugTimer.start();

    cout << "Moving or stationary obstacle? (m/s) ";
    string answer;
    cin >> answer;
    Position setPoint1, setPoint2;
    if (answer == "m") {
        cout << "Moving obstacle chosen" << endl;
        setPoint1.SetX(1.0);
        setPoint1.SetY(0.8);
        setPoint2.SetX(-1.0);
        setPoint2.SetY(-0.8);
    } else {
        if (answer != "s") {
            cout << "Assuming stationary obstacle" << endl;
            answer = "m";
        } else {
            cout << "Stationary obstacle chosen" << endl;
        }
        setPoint1.SetX(0.0);
        setPoint1.SetY(0.0);
        setPoint2.SetX(0.0);
        setPoint2.SetY(0.0);
    }

    Position currTarget = setPoint1;
    roboObs.GotoPos(currTarget);
    int nextTarget = 1;

    while(1) {
        if (roboObs.GetPos().DistanceTo(currTarget) < 0.20) {
            if (nextTarget == 1) {
                currTarget = setPoint1;
                nextTarget = 2;
            } else {
                currTarget = setPoint2;
                nextTarget = 1;
            }
            roboObs.GotoPos(currTarget);
        }
        Position roboMovePos = roboMove.GetPos();
        Position roboObsPos = roboObs.GetPos();
        Force force = ca.getForce(roboMovePos.GetX(), roboMovePos.GetY(), roboObsPos.GetX(), roboObsPos.GetY());
        if (debugTimer.getTime() > 0.5) {
            cout << "----- ----- -----" << endl;
            cout << "PosX: " << roboMovePos.GetX() << " PosY: " << roboMovePos.GetY() << endl;
            cout << "ForceX: " << force.X << " ForceY: " << force.Y << endl;
            debugTimer.reset();
        }
        usleep(100);
    }
}

void Test::beforePenalty() {
    cout << "Currently not working, loser" << endl;
    master.robo0.GotoXY(0.3,0, 60, true);
    master.robo1.GotoXY(1.2, -0.8, 100, false);
    master.robo2.GotoXY(1.2, 0.8, 100, false);
}
void Test::penalty(){
    cout<< "this is the test for Penalty shooting"<<endl;
    Position pos1(master.ball.GetX()+0.5,master.ball.GetY());
    Position pos2(master.ball.GetX()+0.3,master.ball.GetY());
    Position pos3(master.ball.GetX()+0.1,master.ball.GetY());
    int attpos=0;
    while (attpos==0){

        cout << "Moving to pos 1"<< endl << endl;
        master.robo0.CruisetoXY(pos1.GetX(), pos1.GetY(), 150, true);
        usleep(5000);
        if (master.robo0.GetPos().DistanceTo(pos1) < 0.05) attpos=1;
    }
    while (attpos==1){

        cout << "Moving to pos2" << endl << endl;
        master.robo0.GotoXY(pos2.GetX(), pos2.GetY(), 120, true);
        usleep(5000);
        if (master.robo0.GetPos().DistanceTo(pos2) < 0.1) attpos=2;
   }
    while (attpos==2){

        cout << "Moving to " << pos3 << endl << endl;
        master.robo0.CruisetoXY(pos3.GetX(), pos3.GetY(), 100, true);
        usleep(5000);
        if (master.robo0.GetPos().DistanceTo(pos3) < 0.1) attpos=3;
   }
    if (attpos==3){
        cout << "Moving forward" << endl;
        master.robo0.MoveMs(200,200,2000);
        usleep(5000);
        attpos=4;
    }

   }

void Test::goalkeeper() {
    while(1) {
        master.runGoalkeeper();
    }
}


void Test::do_goalkeeper_kick(RoboControl& robogoalkicker, RoboControl& robo_blue_1, RoboControl& robo_blue_2,RoboControl& robo_blue_3, RawBall& ourball){
/*
    double xstart = 1; // on penlaty line
    double ystart = 0.4; // on penalty line

    Position currTarget(xstart,ystart);

    while(robogoalkicker.GetPos().DistanceTo(currTarget) > 0.10)
    robogoalkicker.GotoXY(xstart,ystart); // Go to/on goal line

    double xball = ourball.GetX();
    double yball = ourball.GetY();


    if(yball>=0){                                       // Stay on penalty line a drive next to ball
    while(robogoalkicker.GetY()- (yball-0.15) > 0.05){
          robogoalkicker.GotoXY(xstart,yball-0.15);}

            if(robogoalkicker.GetPhi().Deg()!=180){            // Turn to 180 degrees
            robogoalkicker.TurnAbs(180);
            }

            double yrobot = robogoalkicker.GetY();

            while((xball+0.15) - robogoalkicker.GetX() >  0.05){    // Keep y, move behind ball in x
                  robogoalkicker.GotoXY(xball+0.15,yrobot);}



            double xrobot = robogoalkicker.GetX();

            while((yball) - robogoalkicker.GetY() > 0.02){        // Move behind the ball
                  robogoalkicker.GotoXY(xrobot,yball);}
            cout << "Test" << endl;
           if(robogoalkicker.GetPhi().Deg()!=0){              // Adjust orientation
            robogoalkicker.TurnAbs(0);
            }

    }else{
    while(robogoalkicker.GetY()- (yball+0.15) > 0.05){
          robogoalkicker.GotoXY(xstart,yball+0.15);}

            if(robogoalkicker.GetPhi().Deg()!=180){            // Turn to 180 degrees
            robogoalkicker.TurnAbs(180);
            }

            double yrobot = robogoalkicker.GetY();

            while((xball+0.15) - robogoalkicker.GetX() >  0.05){    // Keep y, move behind ball in x
                  robogoalkicker.GotoXY(xball+0.15,yrobot);}

            cout << "Test" << endl;

            double xrobot = robogoalkicker.GetX();

            while(robogoalkicker.GetY()-(yball) > 0.02){        // Move behind the ball
                  robogoalkicker.GotoXY(xrobot,yball);}

           if(robogoalkicker.GetPhi().Deg()!=0){              // Adjust orientation
            robogoalkicker.TurnAbs(0);
             }
    }
*/
//----------------------------

    //int port = 0;       // Describes between which blue robots the ball should pass - 0: between small and midpos - 1: between midpos and larg

    double dist1 = 0;  // Check order of blue robots
    double dist2 = 0;

    double yblue_1 = robo_blue_1.GetY();
    double yblue_2 = robo_blue_2.GetY();
    double yblue_3 = robo_blue_3.GetY();


    double posvect[4][4];

    posvect[1][0] = 0;
    posvect[2][0] = 0;
    posvect[1][1] = robo_blue_1.GetX();
    posvect[2][1] = robo_blue_1.GetY();
    posvect[1][2] = robo_blue_2.GetX();
    posvect[2][2] = robo_blue_2.GetY();
    posvect[1][3] = robo_blue_3.GetX();
    posvect[2][3] = robo_blue_3.GetY();

    int small = 0;
    int midpo = 0;
    int larg = 0;

    if (yblue_1 < yblue_2 && yblue_1 < yblue_3){
        small = 1;
        if (yblue_2 < yblue_3){
        larg = 3;
        midpo = 2;
        dist1 = robo_blue_1.GetPos().DistanceTo(robo_blue_2.GetPos());
        dist2 = robo_blue_2.GetPos().DistanceTo(robo_blue_3.GetPos());
        }else{
        larg = 2;
        midpo = 3;
        dist1 = robo_blue_1.GetPos().DistanceTo(robo_blue_3.GetPos());
        dist2 = robo_blue_3.GetPos().DistanceTo(robo_blue_2.GetPos());
        }
    }else{
        if(yblue_2 < yblue_1 && yblue_2 < yblue_3){
        small = 2;
        if (yblue_1 < yblue_3){
        larg = 3;
        midpo = 1;
        dist1 = robo_blue_2.GetPos().DistanceTo(robo_blue_1.GetPos());
        dist2 = robo_blue_1.GetPos().DistanceTo(robo_blue_3.GetPos());
        }else{
        larg = 1;
        midpo = 3;
        dist1 = robo_blue_2.GetPos().DistanceTo(robo_blue_3.GetPos());
        dist2 = robo_blue_3.GetPos().DistanceTo(robo_blue_1.GetPos());
        }

        }else{
            small = 3;
            if (yblue_1 < yblue_2){
            larg = 2;
            midpo = 1;
            dist1 = robo_blue_3.GetPos().DistanceTo(robo_blue_1.GetPos());
            dist2 = robo_blue_1.GetPos().DistanceTo(robo_blue_2.GetPos());
            }else{
            larg = 1;
            midpo = 2;
            dist1 = robo_blue_3.GetPos().DistanceTo(robo_blue_2.GetPos());
            dist2 = robo_blue_2.GetPos().DistanceTo(robo_blue_1.GetPos());
            }
        }
    }


    cout << yblue_1 << endl;
    cout << yblue_2 << endl;
    cout << yblue_3 << endl;

    cout << small << endl;
    cout << midpo << endl;
    cout << larg << endl;

    cout << dist1 << endl;
    cout << dist2 << endl;




    Position Targetpoint(0,0);


    if(dist1>=dist2){

        Targetpoint.SetX(posvect[1][midpo] + 0.5*(posvect[1][small]-posvect[1][midpo]));
        Targetpoint.SetY(posvect[2][midpo] + 0.5*(posvect[2][small]-posvect[2][midpo]));
        cout << "Hello dist1" << endl;

        }else{

        Targetpoint.SetX(posvect[1][midpo] + 0.5*(posvect[1][larg]-posvect[1][midpo]));
        Targetpoint.SetY(posvect[2][midpo] + 0.5*(posvect[2][larg]-posvect[2][midpo]));
        cout << "Hello dist2" << endl;
        }
//    ----------------------------------
    Position pos1(1, 0);
    Position pos2(1.27, -0.35);
    Position pos3(1.27, 0.35);

    double goalx, goaly,ballx, bally, initrobox, initroboy;
    Angle ang;
    double delta = 0.15;

    goalx = Targetpoint.GetX();
    goaly = Targetpoint.GetY();


       // robogoalkicker.GotoXY(0,0,160,true);
        robogoalkicker.GotoPos(pos1);
         while(robogoalkicker.GetPos().DistanceTo(pos1)>0.1);

        if(ourball.GetY()>0)
        {

            robogoalkicker.GotoPos(pos2);
            while(robogoalkicker.GetPos().DistanceTo(pos2)>0.1);
        }
        else
        {

            robogoalkicker.GotoPos(pos3);
            while(robogoalkicker.GetPos().DistanceTo(pos3)>0.1);
        }
        ballx = ourball.GetX();
        bally = ourball.GetY();

        initrobox = ballx+delta;
        initroboy = bally + delta*(goaly-bally)/(goalx-ballx);

        robogoalkicker.GotoXY(initrobox,initroboy,50,true);
        Position initP(initrobox,initroboy);
        while(robogoalkicker.GetPos().DistanceTo(initP)>0.05);


        ang = robogoalkicker.GetPos().AngleOfLineToPos(ourball.GetPos());
        robogoalkicker.TurnAbs(ang);
        while((ang-robogoalkicker.GetPhi()).Deg()*(ang-robogoalkicker.GetPhi()).Deg()>0.1);


        usleep(5000);
        usleep(5000);


        robogoalkicker.MoveMs(-255,-255,300);
        usleep(3000);
        robogoalkicker.MoveMs(255,255,500);


/*   ///////////////////// Start Soung: use Targetpoint between 2 blue robots with largest distance


    cout << Targetpoint.GetY() << endl;
    cout << Targetpoint.GetX() << endl;
    cout << "testtest-----"<<endl;
    double ballx,bally,initrobox,initroboy,delta,goaly,goalx,k;

    robogoalkicker.GotoPos(Targetpoint);

    delta = 0.15;

    goalx = Targetpoint.GetX();
    goaly = Targetpoint.GetY();

    ballx = ourball.GetX();
    bally = ourball.GetY();

    k = (goaly-bally)/(goalx-ballx);

    initrobox = ballx+delta;
    initroboy = bally + delta*k;

    robogoalkicker.GotoXY(initrobox,initroboy,50,true);
//     robogoalkicker.GotoPos(Targetpoint);
    robogoalkicker.TurnAbs(-atan(k)*180/3.14159);
    cout<<atan(k)<<endl;
//    robogoalkicker.TurnAbs(180);
//    robogoalkicker.TurnAbs(ourball.GetPos().AngleOfLineToPos(Targetpoint).Deg());

//    cout << ourball.GetPos().AngleOfLineToPos(Targetpoint).Deg() << endl;
    cout<<initrobox<<endl;
    cout<<initroboy<<endl;
    cout<<"-------"<<endl;
    cout<<ballx<<endl;
    cout<<bally<<endl;

  ////////////// Turn

*/

    /*
    usleep(50000);
    robogoalkicker.TurnAbs(ourball.GetPos().AngleOfLineToPos(Targetpoint).Deg() + 180);

    cout << ourball.GetPos().AngleOfLineToPos(Targetpoint).Deg() << endl;
    */


//    double pi = 3.1415926535897;
    //robogoalkicker.TurnAbs(180/pi * acos((abs(xball)-abs(Targetpoint.GetX()))/abs(ourball.GetPos().DistanceTo(Targetpoint.GetPos()))));


/*    cout << 180/pi * acos(abs((abs(xball)-abs(Targetpoint.GetX())))/abs(ourball.GetPos().DistanceTo(Targetpoint.GetPos()))) << endl;


    double x_start_robo_kick = 0;
    double y_start_robo_kick = 0;



    if(yball>=Targetpoint.GetY()){
    x_start_robo_kick = xball + 0.12*(xball - Targetpoint.GetX());
    y_start_robo_kick = yball + 0.12*abs((yball - Targetpoint.GetY()));
    }else{
        x_start_robo_kick = xball + 0.12*(xball - Targetpoint.GetX());
        y_start_robo_kick = yball - 0.12*abs((yball - Targetpoint.GetY()));
            }
    Position Traget_behind_ball(x_start_robo_kick,y_start_robo_kick);

    while(robogoalkicker.GetPos().DistanceTo(Traget_behind_ball) > 0.01){        // Move behind the ball
          robogoalkicker.GotoXY(x_start_robo_kick,y_start_robo_kick);

    }

     usleep(50000);           // Adjust orientation
     robogoalkicker.TurnAbs(0);
     usleep(50000);
     robogoalkicker.GotoXY(xball,yball,160,true);
     usleep(50000);
     robogoalkicker.MoveMs(250,250,2000);
     //robogoalkicker.Kick(100,0.0);

    //usleep(50000);       // Adjust orientation
    //robogoalkicker.TurnAbs(pi/180*abs((abs(xball)-abs(Targetpoint.GetX())))/abs(ourball.GetPos().DistanceTo(Targetpoint.GetPos())));



    double dist_ball_target = 0;
    double angle_ball_target = 0;
    double pi = 3.1415926535897;

    dist_ball_target = ourball.GetPos().DistanceTo(Targetpoint.GetPos());
    angle_ball_target = 180/pi * acos((abs(Targetpoint.GetY())-abs(xball))/abs(dist_ball_target)); // Pi

    cout << dist_ball_target << endl;
    cout << angle_ball_target << endl;
*/

/*
    double xtarget;
    double ytarget;

    if(dist1>=dist2){

    xtarget = 5;//posvect[1][midpo] + 0.5*(posvect[1][small]-posvect[1][midpo]);
    ytarget = 5;//posvect[2][midpo]+ 0.5*(posvect[2][small]-posvect[2][midpo]);

     cout << "Hello dist1" << endl;

    }else{

    xtarget = 5;//posvect[1][midpo] + 0.5*(posvect[1][larg]-posvect[1][midpo]);
    ytarget = 5;//posvect[2][midpo]+ 0.5*(posvect[2][larg]+posvect[2][midpo]);

     cout << "Hello dist2" << endl;

    }


    Position Targetpoint(0,0);
    Targetpoint.SetX(posvect[1][midpo] + 0.5*(posvect[1][small]-posvect[1][midpo]));
    Targetpoint.SetY(posvect[2][midpo]+ 0.5*(posvect[2][small]-posvect[2][midpo]));
    cout << "Hello dist1" << endl;

    //Targetpoint(posvect[1][midpo] + 0.5*(posvect[1][larg]-posvect[1][midpo]),posvect[2][midpo]+ 0.5*(posvect[2][larg]+posvect[2][midpo]));
    cout << "Hello dist2" << endl;
    cout << Targetpoint.GetY() << endl;
    cout << Targetpoint.GetX() << endl;

    cout << posvect[1][midpo] + 0.5*(posvect[1][small]-posvect[1][midpo]) << endl;
    cout << posvect[2][midpo]+ 0.5*(posvect[2][small]-posvect[2][midpo]) << endl;

*/

/*



    if(robo_blue_1.GetPos().DistanceTo(robo_blue_2.GetPos()) > robo_blue_1.GetPos().DistanceTo(robo_blue_3.GetPos()){

        dist1 = robo_blue_1.GetPos().DistanceTo(robo_blue_3.GetPos();

    }else{
        dist1 = robo_blue_1.GetPos().DistanceTo(robo_blue_2.GetPos();

    }

    if(robo_blue_1.GetPos().DistanceTo(robo_blue_2.GetPos()) > robo_blue_1.GetPos().DistanceTo(robo_blue_3.GetPos()){

        dist1 = robo_blue_1.GetPos().DistanceTo(robo_blue_3.GetPos();

    }


*/

}

void Test::testAll() {
    cout << "Running all available tests" << endl;
    cout << "Make sure that the robots are ready \n and the ball is on the field." << endl;

    cout << "Write anything to continue " << endl;
    string answer;
    cin >> answer;
    beforePenalty();

    cout << "TEST 1/1: Before penalty" << endl;


    cout << "Tests completed successfully (hopefully)" << endl;
}
