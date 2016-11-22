#include "robo.h"
#include "angle.h"


void driveSoFast(RoboControl& robo1, RawBall &ball, Position obstPos ,pidController &pidAngle, pidController &pidDistance){
    usleep(10000);
    //get the error
    double ref_deg = robo1.GetPos().AngleOfLineToPos( ball.GetPos() ).Deg();
    double myAngle_deg = robo1.GetPhi().Deg();
    //and solving the angle gap-problem
    //cout << "ref before: " << ref_deg << endl;
    //cout << "angle before: " << myAngle_deg << endl;
    double ajusted_ref_deg = ((int)ref_deg + 4*180) % (2*180);
    double ajusted_myAngle_deg = ((int)myAngle_deg + 4*180) % (2*180);
      //  cout << "ref after: " << ajusted_ref_deg << endl;
       // cout << "angle after: " << ajusted_myAngle_deg << endl;
    double err_rad;
    if (fabs(ajusted_ref_deg - ajusted_myAngle_deg) < fabs(ref_deg - myAngle_deg)){
        err_rad = (ajusted_ref_deg - ajusted_myAngle_deg) *(M_PI/180);
    }
    else{
        err_rad = (ref_deg - myAngle_deg) *(M_PI/180);
    }
    //cout << "Err in rad: " << err_rad << endl;
    double sin_err_rad = sin(err_rad/2);

    double dist_error = robo1.GetPos().DistanceTo(ball.GetPos());
    if (dist_error < 0.03){
        pidDistance.updateInput(dist_error);
    }
    else{
        pidDistance.updateInput(1);
    }

    double roboAngleObstacle_deg = robo1.GetPos().AngleOfLineToPos(obstPos).Deg();
    double ajusted_roboAngleObstacle_deg = ((int)roboAngleObstacle_deg + 4*180) % (2*180);
    double diffRoboObstAngle_rad;
    if(fabs(ajusted_roboAngleObstacle_deg - ajusted_myAngle_deg) < fabs(roboAngleObstacle_deg - myAngle_deg)){
        diffRoboObstAngle_rad = (ajusted_roboAngleObstacle_deg - ajusted_myAngle_deg)*(M_PI/180);
    }else{
        diffRoboObstAngle_rad = (roboAngleObstacle_deg - myAngle_deg)*(M_PI/180);
    }
    cout << "robo2.pos: " << obstPos << endl;
    cout << "RoboDiff: " << diffRoboObstAngle_rad << endl;
    double distToObst = robo1.GetPos().DistanceTo(obstPos);

    int obstAngleSign = diffRoboObstAngle_rad/fabs(diffRoboObstAngle_rad);

    double total_update_angle_rad = sin_err_rad + (distToObst > 0.3 || diffRoboObstAngle_rad > M_PI/2 ? 0 : (-obstAngleSign*cos(diffRoboObstAngle_rad)/4)*0.3/(distToObst));
    pidAngle.updateInput(total_update_angle_rad);
    double pidDist_input = pidDistance.getInput();
    cout << "angle_error: " << err_rad << endl;

    double rightWheel =pidDist_input*cos(err_rad) + pidAngle.getInput();
    double leftWheel = pidDist_input*cos(err_rad) -pidAngle.getInput();
    robo1.MoveMs(leftWheel,rightWheel, 100, 10);
    //out << endl <<robo1.GetPos().AngleOfLineToPos(ball.GetPos())-robo1.GetPhi() << endl;
    //robo1.TurnAbs(robo1.GetPos().AngleOfLineToPos(ball.GetPos())-robo1.GetPhi());

}

