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
        cout << "6: Milestone2.1 part 2" << endl;
        cout << "7: Pull vector" << endl;
        cout << "8: getPassSide" << endl;
        cout << "9: Goalkeeper's kick" << endl;
        cout << "10: Milestone 2.1 part 1" << endl;
        cout << "11: penalty shooting" << endl;
        cout << "12: Before penalty" << endl;
        cout << "13: Turning" << endl;
        cout << "14: Map measurement" << endl;
        cout << "15: Random driving" << endl;

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
            cout << "Running 6: Milestone 2.1 part 2" << endl;
            pidCollision(master.robo0, master.ball);
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
            do_goalkeeper_kick(master.robo0, master.robo3, master.robo4, master.robo5, master.ball);
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
        case 13:
            cout << "Running 13: Turning" << endl;
            turning();
            break;
        case 14:
            cout << "Running 14: Map measurement" << endl;
            cout << "Coordinates of ball: " << master.ball.GetX() << " " << master.ball.GetY() << endl;
            break;
        case 15:
            cout << "Running 15: Random driving" << endl;
            randomDrivingWithCA();
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

void Test::turning() {
    cout << "----- Before -----" << endl;
    cout << "Robo orientation: " << master.robo0.GetPhi().Deg() << endl;

    master.robo0.TurnAbs(Angle(170));
    usleep(5000000);
    cout << "Turn complete" << endl;

    cout << "----- After -----" << endl;
    cout << "Robo orientation: " << master.robo0.GetPhi().Deg() << endl;
}

static void switchTargetPosition(Position &roboPos, vector<Position> &positions){
    if(roboPos == positions[0]){
        roboPos = positions[2];
    }else if(roboPos == positions[2]){
        roboPos = positions[0];
    }else if(roboPos == positions[1]){
        roboPos = positions[3];
    }else if(roboPos == positions[3]){
        roboPos = positions[1];
    }
}

static void newRandomPosition(Position &randomPos, double randY, double randX){
//    int randIntY = (rand() % 2*40) - 40;
//    int randIntX = (rand() % 2*40) - 40;
    //double randY = static_cast<double>(randIntY)/100;//((double)(rand() % 2*40) - 40.0) /100;
    //double randX = static_cast<double>(randIntX)/100;//((double)(rand() % 2*120) - 120.0) /100;
    randomPos.SetY(randY);
    randomPos.SetX(randX);
    cout << "randomPos: " << randomPos << endl;
}

void Test::milestone21part1() {
    //for the random function
    std::random_device rdY;
    std::random_device rdX;
    std::mt19937 mtY(rdY());
    std::mt19937 mtX(rdX());
    std::uniform_real_distribution<double> distY(-0.4, 0.4);
    std::uniform_real_distribution<double> distX(-1.2, 1.2);

    cout << "To run milestone 2.1 part 1, 4 positions are required." << endl;
    vector<Position> positions;
    string fixedCoordinates;
    do{
        cout << "Use fixed coordinates? (y/n) ";
        cin >> fixedCoordinates;
        cout << endl;
    }while(fixedCoordinates != "y" && fixedCoordinates != "n");
    if (fixedCoordinates == "n") {
        for (int i = 0; i < 4; i++) {
            double posX, posY;
            cout << "Coordinate " << i + 1 << ", X: ";
            cin >> posX;
            cout << "Coordinate " << i + 1 << ", Y: ";
            cin >> posY;
            positions.push_back(Position(posX, posY));
        }
    }else{
        positions.push_back(Position(1.2, -0.45));
        positions.push_back(Position(-1.2, -0.45));
        positions.push_back(Position(-1.2, 0.45));
        positions.push_back(Position(1.2, 0.45));
    }
    cout << "Setup complete. Coordinates are:" << endl;
    cout << "1: { " << positions[0].GetX() << ", " << positions[0].GetY() << " }" << endl;
    cout << "2: { " << positions[1].GetX() << ", " << positions[1].GetY() << " }" << endl;
    cout << "3: { " << positions[2].GetX() << ", " << positions[2].GetY() << " }" << endl;
    cout << "4: { " << positions[3].GetX() << ", " << positions[3].GetY() << " }" << endl;

    master.robo0.updatePids(positions[0]);
    master.robo1.updatePids(positions[1]);
    master.robo2.updatePids(positions[3]);
    Position robo0Pos = positions[0];
    Position robo1Pos = positions[1];
    Position robo2Pos = positions[3];
    Position randomPos;
    newRandomPosition(randomPos, distY(mtY), distX(mtX));
    master.robo3.GotoXY(randomPos.GetX(), randomPos.GetY(), 40);
    while(1){
        usleep(10000);
        master.robo0.updatePids(robo0Pos);
        master.robo1.updatePids(robo1Pos);
        master.robo2.updatePids(robo2Pos);
        //master.robo3.updatePids(randomPos);
        master.robo0.updatePositions();
        master.robo1.updatePositions();
        master.robo2.updatePositions();
        //master.robo3.updatePositions();
        master.robo0.driveWithCA();
        master.robo1.driveWithCA();
        master.robo2.driveWithCA();
        //master.robo3.driveWithCA();


        if( master.robo0.GetPos().DistanceTo(robo0Pos) < 0.2 ){
            switchTargetPosition(robo0Pos, positions);
        }
        if(master.robo1.GetPos().DistanceTo(robo1Pos) < 0.2){
            switchTargetPosition(robo1Pos, positions);
        }
        if(master.robo2.GetPos().DistanceTo(robo2Pos) < 0.2){
            switchTargetPosition(robo2Pos, positions);
        }
        if(master.robo3.GetPos().DistanceTo(randomPos) < 0.2){
            newRandomPosition(randomPos, distY(mtY), distX(mtX));
            master.robo3.GotoXY(randomPos.GetX(), randomPos.GetY(), 40);
        }
    }
}

void Test::randomDrivingWithCA() {
    //for the random function
    std::random_device rdY;
    std::random_device rdX;
    std::mt19937 mtY(rdY());
    std::mt19937 mtX(rdX());
    std::uniform_real_distribution<double> distY(-0.4, 0.4);
    std::uniform_real_distribution<double> distX(-1.2, 1.2);

    Position rPos0, rPos1, rPos2, rPos3, rPos4, rPos5;
    newRandomPosition(rPos0, distY(mtY), distX(mtX));
    newRandomPosition(rPos1, distY(mtY), distX(mtX));
    newRandomPosition(rPos2, distY(mtY), distX(mtX));
    newRandomPosition(rPos3, distY(mtY), distX(mtX));
    newRandomPosition(rPos4, distY(mtY), distX(mtX));
    newRandomPosition(rPos5, distY(mtY), distX(mtX));


    while(1){
        usleep(10000);
        master.robo0.updatePids(rPos0);
        master.robo1.updatePids(rPos1);
        master.robo2.updatePids(rPos2);
        master.robo3.updatePids(rPos3);
        master.robo4.updatePids(rPos4);
        master.robo5.updatePids(rPos5);
        master.robo0.updatePositions();
        master.robo1.updatePositions();
        master.robo2.updatePositions();
        master.robo3.updatePositions();
        master.robo4.updatePositions();
        master.robo5.updatePositions();
        master.robo0.driveWithCA();
        master.robo1.driveWithCA();
        master.robo2.driveWithCA();
        master.robo3.driveWithCA();
        master.robo4.driveWithCA();
        master.robo5.driveWithCA();

        if(master.robo0.GetPos().DistanceTo(rPos0) < 0.2){
            newRandomPosition(rPos0, distY(mtY), distX(mtX));
            master.robo0.GotoXY(rPos0.GetX(), rPos0.GetY(), 40);
        }
        if(master.robo1.GetPos().DistanceTo(rPos1) < 0.2){
            newRandomPosition(rPos1, distY(mtY), distX(mtX));
            master.robo1.GotoXY(rPos1.GetX(), rPos1.GetY(), 40);
        }
        if(master.robo2.GetPos().DistanceTo(rPos2) < 0.2){
            newRandomPosition(rPos2, distY(mtY), distX(mtX));
            master.robo2.GotoXY(rPos2.GetX(), rPos2.GetY(), 40);
        }
        if(master.robo3.GetPos().DistanceTo(rPos3) < 0.2){
            newRandomPosition(rPos3, distY(mtY), distX(mtX));
            master.robo3.GotoXY(rPos3.GetX(), rPos3.GetY(), 40);
        }
        if(master.robo4.GetPos().DistanceTo(rPos4) < 0.2){
            newRandomPosition(rPos4, distY(mtY), distX(mtX));
            master.robo4.GotoXY(rPos4.GetX(), rPos4.GetY(), 40);
        }
        if(master.robo5.GetPos().DistanceTo(rPos5) < 0.2){
            newRandomPosition(rPos5, distY(mtY), distX(mtX));
            master.robo5.GotoXY(rPos5.GetX(), rPos5.GetY(), 40);
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
            cout << "Current position, X: " << basePos.GetX() << " Y: " << basePos.GetY() << endl;
            cout << "Pass side: " << passSide << endl;
            cout << "Pull vector: { " << pull.X << " , " << pull.Y << " }" << endl;
            cout << "Pull length: " << pull.len << endl;
            cout << "Pull reference angle, rad: " << pull.rad << " deg: " << pull.deg << endl;
            debugTimer.reset();
        }
        usleep(100);
    }
}

void Test::pidCollision(Robo &robo, RawBall &ball){
    while(1){
        usleep(10000);
        robo.updatePids(ball.GetPos());
        robo.driveWithCA();
        robo.updatePositions();
    }
}

void Test::collisionAvoidance(Robo& roboMove, Robo& roboObs) {
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


void Test::do_goalkeeper_kick(Robo& robogoalkicker, Robo& robo_blue_1, Robo& robo_blue_2,Robo& robo_blue_3, RawBall& ourball){
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


    double dist_larg_wall = 0.85 - fabs(posvect[2][larg]);
    double dist_small_wall = 0.85 - fabs(posvect[2][small]);


      Position Targetpoint(0,0);


    if(dist1>=dist2){
        cout << "Hello dist1" << endl;
        if (dist1 > dist_larg_wall && dist1 > dist_small_wall){
            Targetpoint.SetX(posvect[1][midpo] + 0.5*(posvect[1][small]-posvect[1][midpo]));
            Targetpoint.SetY(posvect[2][midpo] + 0.5*(posvect[2][small]-posvect[2][midpo]));
        }

        if (dist_larg_wall > dist1 && dist_larg_wall > dist_small_wall){
            Targetpoint.SetX(posvect[1][larg]);
            Targetpoint.SetY((0.85 + posvect[2][larg])/2);
        }

        if (dist_small_wall > dist1 && dist_small_wall > dist_larg_wall){
            Targetpoint.SetX(posvect[1][small]);
            Targetpoint.SetY((-0.85 + posvect[2][small])/2);



        }

        }else{

        cout << "Hello dist2" << endl;

        if (dist2 > dist_larg_wall && dist2 > dist_small_wall){
        Targetpoint.SetX(posvect[1][midpo] + 0.5*(posvect[1][larg]-posvect[1][midpo]));
        Targetpoint.SetY(posvect[2][midpo] + 0.5*(posvect[2][larg]-posvect[2][midpo]));
        }

        if (dist_larg_wall > dist2 && dist_larg_wall > dist_small_wall){
            Targetpoint.SetX(posvect[1][larg]);
            Targetpoint.SetY((0.85 + posvect[2][larg])/2);
        }

        if (dist_small_wall > dist2 && dist_small_wall > dist_larg_wall){
            Targetpoint.SetX(posvect[1][small]);
            Targetpoint.SetY((-0.85 + posvect[2][small])/2);
        }


        }

    if(Targetpoint.GetX()>0.75){


        if(posvect[2][midpo] < -0.40 || posvect[2][midpo] > 0.40){

            Targetpoint.SetX(-0.4);
            Targetpoint.SetY(ourball.GetY());


        }else{


            Targetpoint.SetY((Targetpoint.GetY() + posvect[2][midpo])/2);
            Targetpoint.SetX(Targetpoint.GetX() + 0.02);


        }

    }



//    ----------------------------------
    Position pos1(1, 0);
    Position pos2(1.3, -0.35);
    Position pos3(1.3, 0.35);
    Position pos4(1.1, 0);

    pos1.SetY(robogoalkicker.GetY());

    double goalx, goaly,ballx, bally, initrobox, initroboy;
    Angle ang;
    double delta = 0.09;


    goalx = Targetpoint.GetX();
    goaly = Targetpoint.GetY();


       // robogoalkicker.GotoXY(0,0,160,true);
        robogoalkicker.GotoPos(pos1);
         while(robogoalkicker.GetPos().DistanceTo(pos1)>0.1);

         robogoalkicker.GotoPos(pos4);
          while(robogoalkicker.GetPos().DistanceTo(pos4)>0.1);

        if(ourball.GetY()>0)
        {

            robogoalkicker.GotoPos(pos2);
            while(robogoalkicker.GetPos().DistanceTo(pos2)>0.08);
        }
        else
        {

            robogoalkicker.GotoPos(pos3);
            while(robogoalkicker.GetPos().DistanceTo(pos3)>0.08);
        }
        ballx = ourball.GetX();
        bally = ourball.GetY();

        initrobox = ballx+delta;
        initroboy = bally + delta*(goaly-bally)/(goalx-ballx);



        if(bally>0)
        {
            initroboy = initroboy+0.02;
        }
        else
        {
            initroboy = initroboy-0.02;
        }


        if(Targetpoint.GetX()>0.75){


            if(bally>0)
            {
                initroboy = initroboy-0.02;
            }
            else
            {
                initroboy = initroboy+0.02;
            }


        }

        robogoalkicker.GotoXY(initrobox,initroboy,50,true);
        Position initP(initrobox,initroboy);
        while(robogoalkicker.GetPos().DistanceTo(initP)>0.05);


        ang = robogoalkicker.GetPos().AngleOfLineToPos(ourball.GetPos());

        //ang = ourball.GetPos().AngleOfLineToPos(Targetpoint);
/*        robogoalkicker.TurnAbs(ang);
        while((ang-robogoalkicker.GetPhi()).Deg()*(ang-robogoalkicker.GetPhi()).Deg()>0.1);
*/

        ///////// Claculate angle

//        double pi = 3.1415926535897;
//        double our_angle = 180/pi*acos((ballx-Targetpoint.GetX())/abs(ourball.GetPos().DistanceTo(Targetpoint)));
//        double our_angle = 180+180/pi*atan((Targetpoint.GetY()-bally)/(Targetpoint.GetX()-ballx));
/*
        if(Targetpoint.GetY()>=0){
            our_angle = 180-our_angle;

        }else{

            our_angle = our_angle-180;
        }
*/

//        cout<< our_angle << endl;


        //robogoalkicker.MoveMs(-25,25,50000);
        //while(abs(abs(ang.Deg())-abs(robogoalkicker.GetPhi().Deg())) > 0.05); //*(ang.Deg()-robogoalkicker.GetPhi().Deg())>0.05);
        //while((ang.Deg()-robogoalkicker.GetPhi().Deg())*(ang.Deg()-robogoalkicker.GetPhi().Deg()) > 0.1); //*(ang.Deg()-robogoalkicker.GetPhi().Deg())>0.05);
 //       while(abs(our_angle-robogoalkicker.GetPhi().Deg())>0.1)


        cout << "----- Before -----" << endl;
        cout << "Robo orientation: " << robogoalkicker.GetPhi().Deg() << endl;

        cout << "Turning to " << ang.Deg() << " degrees" << endl;

        int i =1;

        for(i=1;i<10;i++)
        {
        robogoalkicker.TurnAbs(ang);
         usleep(300000);
        while((ang.Deg()-robogoalkicker.GetPhi().Deg())*(ang.Deg()-robogoalkicker.GetPhi().Deg()) > 0.1)
        {
            if(fabs(robogoalkicker.GetSpeedLeft()==robogoalkicker.GetSpeedRight())<0.01)
            {
                usleep(2000);
                break;
            }
        }
        }

        usleep(3000000);
        cout << "Turn complete" << endl;

        cout<<Targetpoint.GetY()<<endl;


        robogoalkicker.MoveMs(250,250,500);
//        usleep(3000);
        //robogoalkicker.MoveMs(255,225,500);


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
