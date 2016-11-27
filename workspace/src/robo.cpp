#include "robo.h"
#include "collision_avoidance.h"
#define CA_SCALE 5 //How munch influenced by CA, lower is more. test: 20

/** To complete task 2.1 part 1, we have to implement multithreading it seems
  * like. We will have problems controlling all robots at the same time without
  */
void Robo::run(cpp::channel<Position> positionCh) {
    while (1) {
    }
}


void Robo::driveWithCA(RoboControl& robo1, RawBall &ball, Position obstPos ,pidController &pidAngle, pidController &pidDistance){
    usleep(10000);
    double dist_error = robo1.GetPos().DistanceTo(ball.GetPos());
    if (dist_error < 0.03){
        pidDistance.updateInput(dist_error);
    }
    else{
        pidDistance.updateInput(1.2);
    }
    double angleErrorRad = getAngleErrRad(ball.GetPos(), obstPos);
    double sinAngleErrorRad = sin(angleErrorRad/2);
    pidAngle.updateInput(sinAngleErrorRad);
    double driveSpeed = pidDistance.getInput();
    //std::cout << "angleErrorRad: " << angleErrorRad << std::endl;
    double angleInput = pidAngle.getInput();
    //std::cout << "angleInput: " << angleInput<< std::endl;

    // the cos will make it drive fastest in the right direction, and also back up and turn if behind you
    double rightWheel =driveSpeed*cos(angleErrorRad) + angleInput;
    double leftWheel = driveSpeed*cos(angleErrorRad) - angleInput;
    //rightWheel += leftWheel;
    //leftWheel = leftWheel;
    robo1.MoveMs(leftWheel,rightWheel, 100, 10);
    //out << endl <<robo1.GetPos().AngleOfLineToPos(ball.GetPos())-robo1.GetPhi() << endl;
    //robo1.TurnAbs(robo1.GetPos().AngleOfLineToPos(ball.GetPos())-robo1.GetPhi());

}

double Robo::getAngleWithCA(Force obstacleForce, Position targetPos){

    double targetDeg = this->GetPos().AngleOfLineToPos(targetPos).Deg();
    double caScale = obstacleForce.len/(CA_SCALE + obstacleForce.len);
    double obstacleAngle = obstacleForce.deg;
    double ajusted_obstacleAngle = ((int)obstacleAngle + 4*180) % (2*180);
    double ajustedTargetDeg = ((int)targetDeg + 4*180) % (2*180);
    if(fabs(ajusted_obstacleAngle - ajustedTargetDeg) < fabs(obstacleAngle - targetDeg)){
        obstacleAngle = ajusted_obstacleAngle;
        targetDeg = ajustedTargetDeg;
    }
    //cout << fabs(ajusted_obstacleAngle - ajustedTargetDeg) << " and " << fabs(obstacleAngle - targetDeg) << endl;
    double targetDegWithCA = obstacleAngle*(caScale) + targetDeg*(1 - caScale);
    cout << "targetDeg : " << targetDeg << endl;
    cout << "obstacleForce.deg : " << obstacleAngle << endl;
    cout << "targetDegWithCA : " << targetDegWithCA << endl;
    return targetDegWithCA;
//    //get the magnitude of force
//    int obstAngleSign = obstAngleDiffRad/fabs(obstAngleDiffRad);
//    if(distToObst > 0.3 || obstAngleDiffRad > M_PI/2){
//        return 0; //No collition avoidance needed
//    } else{
//        // collition avoidance needed
//        // right sign,          more turn the more direct treath, 4 is just scaling. Last part increases when closer to obstacle. TO BE CHANGED FOR JOACHIMS STUFF
//        return -obstAngleSign * cos(obstAngleDiffRad)/4          * 0.3/(distToObst);
//    }
}

double Robo::getAngleErrRad(Position targetPos, Position obstPos){
    //get the error
    CA ca;
    Position myPos = this->GetPos();
    double ref_deg = getAngleWithCA(ca.getPull(myPos, targetPos, obstPos ), targetPos);
    double myAngle_deg = this->GetPhi().Deg();
    //and solving the angle gap-problem
    //cout << "ref before: " << ref_deg << endl;
    //cout << "angle before: " << myAngle_deg << endl;
    double ajusted_ref_deg = ((int)ref_deg + 4*180) % (2*180); // This magic is to fix the 180 to -180 gap problem in angle. Here, I calculate what the smallest diff angle is with 360 to 0 gap, and later compare these diff values.
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
    return err_rad;
}

double Robo::getObstacleAngleDiffRad(RoboControl& robo, Position obstPos){
    double myAngle_deg = robo.GetPhi().Deg();
    double roboAngleObstacle_deg = robo.GetPos().AngleOfLineToPos(obstPos).Deg();

    double ajusted_myAngle_deg = ((int)myAngle_deg + 4*180) % (2*180);
    double ajusted_roboAngleObstacle_deg = ((int)roboAngleObstacle_deg + 4*180) % (2*180);
    double diffRoboObstAngle_rad;
    if(fabs(ajusted_roboAngleObstacle_deg - ajusted_myAngle_deg) < fabs(roboAngleObstacle_deg - myAngle_deg)){
        diffRoboObstAngle_rad = (ajusted_roboAngleObstacle_deg - ajusted_myAngle_deg)*(M_PI/180);
    }else{
        diffRoboObstAngle_rad = (roboAngleObstacle_deg - myAngle_deg)*(M_PI/180);
    }
//    cout << "robo2.pos: " << obstPos << endl;
//    cout << "RoboDiff: " << diffRoboObstAngle_rad << endl;
    return diffRoboObstAngle_rad;

}
