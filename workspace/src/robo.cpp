#include "robo.h"

//    pidController pidAngle(40.0, 1, 1.0);
//    pidController pidDistance(150.0, 0.0, 0.0);
//    while(1){
//        driveSoFast(robo1, ball, pidAngle, pidDistance);
//     }

void driveSoFast(RoboControl& robo1, RawBall &ball, pidController &pidAngle, pidController &pidDistance){
   //if timer is time, update pids()
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
    pidAngle.updateInput(sin_err_rad);

    double dist_error = robo1.GetPos().DistanceTo(ball.GetPos());
    if (dist_error < 0.03){
        pidDistance.updateInput(dist_error);
    }
    else{
        pidDistance.updateInput(1);
    }
    double pidDist_input =pidDistance.getInput();
    cout << "dist_erro: " <<  dist_error << endl;
    double rightWheel =pidDist_input*cos(err_rad) + pidAngle.getInput();
    double leftWheel = pidDist_input*cos(err_rad) -pidAngle.getInput();
    robo1.MoveMs(leftWheel,rightWheel, 100, 10);
    //out << endl <<robo1.GetPos().AngleOfLineToPos(ball.GetPos())-robo1.GetPhi() << endl;
    //robo1.TurnAbs(robo1.GetPos().AngleOfLineToPos(ball.GetPos())-robo1.GetPhi());

}
