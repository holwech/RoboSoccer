#include "test.h"

Test::Test()
{
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
            cout << "X: " << force.X << " Y: " << force.Y << endl;
            debugTimer.reset();
        }

    }
}
