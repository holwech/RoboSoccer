#include "robo.h"
#define CA_SCALE 5 //How munch influenced by CA, lower is more. test: 20

#define DEBUG true

/** To complete task 2.1 part 1, we have to implement multithreading it seems
  * like. We will have problems controlling all robots at the same time without
  */

//Use Goto to set target position. Remember to also run the driveWithCA() 100 times a second
void Robo::GotoPos(Position target){
    this->targetPosition = target;
}

void Robo::goalieGoto(Position target){

}

void Robo::setVariables(Robo& team1, Robo& team2, Robo& otherTeam1, Robo& otherTeam2, Robo& otherTeam3) {
    team.push_back(&team1);
    team.push_back(&team2);
    otherTeam.push_back(&otherTeam1);
    otherTeam.push_back(&otherTeam2);
    otherTeam.push_back(&otherTeam3);

    posTeam.push_back(team[0]->GetPos());
    posTeam.push_back(team[1]->GetPos());
    posOtherTeam.push_back(otherTeam[0]->GetPos());
    posOtherTeam.push_back(otherTeam[1]->GetPos());
    posOtherTeam.push_back(otherTeam[2]->GetPos());
}

void Robo::updatePositions() {
   posTeam[0] = team[0]->GetPos();
   posTeam[1] = team[1]->GetPos();
   posOtherTeam[0] = otherTeam[0]->GetPos();
   posOtherTeam[1] = otherTeam[1]->GetPos();
   posOtherTeam[2] = otherTeam[2]->GetPos();
}
//PID - related functions
void Robo::updatePids(Position targetPos, bool ca = true){
    updateDistancePid(targetPos);
    if (ca){
        updateAnglePidWithCA(targetPos);
    }else{
        updateAnglePidWithoutCA(targetPos);
    }
}

void Robo::updateDistancePid(Position targetPos){
    double dist_error = this->GetPos().DistanceTo(targetPos);
    if (dist_error < 0.03){
        pidDistance.updateInput(dist_error);
    }
    else{
        pidDistance.updateInput(1.1);
    }
}
void Robo::updateAnglePidWithoutCA(Position targetPos){
    this->angleErrorRad = getReferenceAngleErrRad(targetPos, true);
    double sinAngleErrorRad = sin(this->angleErrorRad/2);
    pidAngle.updateInput(sinAngleErrorRad);
}
void Robo::updateAnglePidWithCA(Position targetPos){
    this->angleErrorRad = getReferenceAngleErrRad(targetPos, false);
    double sinAngleErrorRad = sin(this->angleErrorRad/2);
    pidAngle.updateInput(sinAngleErrorRad);
}

void Robo::updateAnglePidGoalie(Position targetPos){
    this->angleErrorRad = 9000;
}

//DRIVE - related functions
void Robo::turn(Position targetPos){
    this->updateAnglePidWithoutCA(targetPos);
    double angleInput = pidAngle.getInput();
    double rightWheel = +angleInput;
    double leftWheel = -angleInput;

    cout << "leftwheel: " << leftWheel << endl;
    cout << "rightwheel: " << rightWheel << endl;
    this->MoveMs(leftWheel, rightWheel,100,10);
}
//Drive functions must be run 100 times a second for robot to drive. Target position set by Goto()
void Robo::driveWithCA() {
    updatePositions();
    updatePids(targetPosition, true);
    // Get designated pid values
    double driveSpeed = pidDistance.getInput();
    double angleInput = pidAngle.getInput();

    // the cos will make it drive fastest in the right direction, and also back up and turn if behind you
    double rightWheel = driveSpeed*cos(this->angleErrorRad) + angleInput;
    double leftWheel = driveSpeed*cos(this->angleErrorRad) - angleInput;
    if(DEBUG){
        cout << "leftwheel: " << leftWheel << endl;
        cout << "rightwheel: " << rightWheel << endl;
    }
    this->MoveMs(leftWheel,rightWheel, 100, 10);
    //out << endl <<robo1.GetPos().AngleOfLineToPos(ball.GetPos())-robo1.GetPhi() << endl;
    //robo1.TurnAbs(robo1.GetPos().AngleOfLineToPos(ball.GetPos())-robo1.GetPhi());
}

double Robo::getDiffBetweenAnglesRad(double angle1, double angle2){
    double ajusted_angle1 = ((int)angle1 + 4*180) % (2*180);
    double ajusted_angle2 = ((int)angle2 + 4*180) % (2*180);
    if (fabs(ajusted_angle1 - ajusted_angle2) < fabs(angle1 - angle2)){
        return (ajusted_angle1 - ajusted_angle2) *(M_PI/180);
    }
    else{
        return (angle1 - angle2) *(M_PI/180);
    }
}

double Robo::getRefAngleWithCA(Force obstacleForce, Position targetPos){
    double targetDeg = this->GetPos().AngleOfLineToPos(targetPos).Deg();
    double caScale = obstacleForce.len/(CA_SCALE + obstacleForce.len);
    double obstacleAngle = obstacleForce.deg;
    double ajusted_obstacleAngle = ((int)obstacleAngle + 4*180) % (2*180);
    double ajustedTargetDeg = ((int)targetDeg + 4*180) % (2*180);
    if(fabs(ajusted_obstacleAngle - ajustedTargetDeg) < fabs(obstacleAngle - targetDeg)){
        obstacleAngle = ajusted_obstacleAngle;
        targetDeg = ajustedTargetDeg;
    }
    double targetDegWithCA = obstacleAngle*(caScale) + targetDeg*(1 - caScale);
    return targetDegWithCA;
}

double Robo::getRefAngleWithoutCA(Position targetPos){
    return this->GetPos().AngleOfLineToPos(targetPos).Deg();
}



double Robo::getGoalieReferenceAngleErrRad(Position targetPos){
   // Position myPos = this->GetPos();
    double ref_deg = getRefAngleWithoutCA(targetPos);
    double myAngleDeg = this->GetPhi().Deg();
    double diffAngle = getDiffBetweenAnglesRad(ref_deg, myAngleDeg);
    if (abs(diffAngle) > M_PI/2){
        myAngleDeg = ((int)myAngleDeg - 180 + 4*180) % (2*180);
        return getDiffBetweenAnglesRad(ref_deg, myAngleDeg);
    }
    else{
        return diffAngle;
    }
}


double Robo::getReferenceAngleErrRad(Position targetPos, bool ca = true){
    //get the error
    Position myPos = this->GetPos();
    double ref_deg;
    if (ca){
        ref_deg = getRefAngleWithCA(this->ca.getTotalPull(myPos, targetPos, posTeam, posOtherTeam, false), targetPos);
    } else{
        ref_deg = getRefAngleWithoutCA(targetPos);
    }
    double myAngle_deg = this->GetPhi().Deg();
    double err_rad;
    //and solving the angle gap-problem
//    double ajusted_ref_deg = ((int)ref_deg + 4*180) % (2*180); // This magic is to fix the 180 to -180 gap problem in angle. Here, I calculate what the smallest diff angle is with 360 to 0 gap, and later compare these diff values.
//    double ajusted_myAngle_deg = ((int)myAngle_deg + 4*180) % (2*180);
//    double err_rad;
//    if (fabs(ajusted_ref_deg - ajusted_myAngle_deg) < fabs(ref_deg - myAngle_deg)){
//        err_rad = (ajusted_ref_deg - ajusted_myAngle_deg) *(M_PI/180);
//    }
//    else{
//        err_rad = (ref_deg - myAngle_deg) *(M_PI/180);
//    }
    err_rad = getDiffBetweenAnglesRad(ref_deg, myAngle_deg);
    return err_rad;
}

