#include "test.h"
#include <limits>
#include "robo.h"

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
            cout << "Running 5: Collision avoidance" << endl;
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
            do_goalkeeper_kick(master.robo1, master.ball);
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
    pidController pidAngle(40.0, 1, 1.0);
    pidController pidDistance(150.0, 0.0, 0.0);
    while(1){
        driveSoFast(robo, ball, obstacle.GetPos(), pidAngle, pidDistance);
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
}

void Test::goalkeeper() {
    while(1) {
        master.runGoalkeeper();
    }
}


void Test::do_goalkeeper_kick(RoboControl& robogoalkicker, RawBall& ourball){

    double xstart = 1; // on penlaty line
    double ystart = 0.4; // on penalty line

    Position currTarget(xstart,ystart);

    while(robogoalkicker.GetPos().DistanceTo(currTarget) > 0.10)
    robogoalkicker.GotoXY(xstart,ystart); // Go to/on goal line

    double xball = ourball.GetX();
    double yball = ourball.GetY();


    if(yball>=0){
    while(robogoalkicker.GetY()- (yball-0.15) > 0.05){
          robogoalkicker.GotoXY(xstart,yball-0.15);}
    }else{
    while(robogoalkicker.GetY()- (yball+0.15) > 0.05){
          robogoalkicker.GotoXY(xstart,yball+0.15);}
    }

    if(robogoalkicker.GetPhi().Deg()!=180){
    robogoalkicker.TurnAbs(180);
    }




    double yrobot = robogoalkicker.GetY();



    while((xball+0.15) - robogoalkicker.GetX() >  0.05){
          robogoalkicker.GotoXY(xball+0.15,yrobot);}

    cout << "Test" << endl;

    double xrobot = robogoalkicker.GetX();

    while(robogoalkicker.GetY()-(yball) > 0.02){
          robogoalkicker.GotoXY(xrobot,yball);}

    if(robogoalkicker.GetPhi().Deg()!=0){
    robogoalkicker.TurnAbs(0);
    }
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
