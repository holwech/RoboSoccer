#include "robo.h"
#define CA_SCALE 5 //How munch influenced by CA, lower is more. test: 20

#define DEBUG false
#define ARRIVED_DIST 0.04



/** To complete task 2.1 part 1, we have to implement multithreading it seems
  * like. We will have problems controlling all robots at the same time without
  */

//Use Goto to set target position. Remember to also run the driveWithCA() 100 times a second

void Robo::GotoPos(Position target, double speed, bool ca){
    isIdle = false;
    this->toggleCA = ca;
    if(onlyTurn){
        onlyTurn = false;
        this->pidAngle.changeParams(ANGLE_KP_DRIVE, ANGLE_KI_DRIVE, ANGLE_KD_DRIVE);
    }
    else if (isGoalkeeper){
        this->targetPosition = target;
    }
    else{
        //cout << "Target BEFORE:" << target << endl;
        Position movedTarget = movePosInBounce(target);
        //cout << "Target AFTER:" << movedTarget << endl;
        this->targetPosition = movedTarget;
    }
    this->speed = speedModifier(target, speed);
}

double Robo::speedModifier(Position target, double speed) {
    double distance = this->GetPos().DistanceTo(target);
    // Lower number gives a sharper slow down
    double modifier = isGoalkeeper ? 0.05 : 0.2;
    double scale = distance / (distance + modifier);
    //cout << "Scale is: " << scale << endl;
    /*
    if (scale < 0.05) {
        return speed * 0.05;
    }
    */
    return scale * speed;
}

void Robo::turn(Position targetPos){
    isIdle = false;
    if (!onlyTurn){
        this->onlyTurn = true;
        this->pidAngle.changeParams(ANGLE_KP_TURN, ANGLE_KI_TURN, ANGLE_KD_TURN);
    }
    this->targetPosition = targetPos;
}

void Robo::stop() {
    idle();
}

void Robo::idle(){
    isIdle = true;
}

Position Robo::movePosInBounce(Position pos){
    //Find how much we must divide the vector to make it in bounce
    Position midPosLeft(-0.58,0);
    Position midPosRight(0.58,0);
    //cout << "length left: " << midPosLeft.DistanceTo(GetPos()) << endl;
    //cout << "length right: " << midPosRight.DistanceTo(GetPos()) << endl;
    double scaleX = 0;
    double scaleY = 0;
    double cornerScale = 0;

    //corners
    if(pos.GetX() < -1.17){
        if (pos.GetY() > 0.61){
            //bottom door
            cornerScale = midPosLeft.DistanceTo(pos)/1.04;
            //cout << "Cornerscale: " << cornerScale << endl;
            if (cornerScale > 1){
                //cout << "SCALED POS IN CORNER" << endl;
                return Position(pos.GetX()/cornerScale, pos.GetY()/cornerScale);
            }
        }
        else if(pos.GetY() < -0.58){
            //top door
            cornerScale = midPosLeft.DistanceTo(pos)/1.04;
            //cout << "Cornerscale: " << cornerScale << endl;
            if (cornerScale > 1){
                //cout << "SCALED POS IN CORNER" << endl;
                return Position(pos.GetX()/cornerScale, pos.GetY()/cornerScale);
            }
        }
    }
    else if(pos.GetX() > 1.2){
        if (pos.GetY() < -0.62){
            //top not door
            cornerScale = midPosRight.DistanceTo(pos)/1.09;
            //cout << "Cornerscale: " << cornerScale << endl;
            if (cornerScale > 1){
                //cout << "SCALED POS IN CORNER" << endl;
                return Position(pos.GetX()/cornerScale, pos.GetY()/cornerScale);
            }
        }
        else if(pos.GetY() > 0.6){
            //bottom not door
            cornerScale = midPosRight.DistanceTo(pos)/1.07;
            //cout << "Cornerscale: " << cornerScale << endl;
            if (cornerScale > 1){
                //cout << "SCALED POS IN CORNER" << endl;
                return Position(pos.GetX()/cornerScale, pos.GetY()/cornerScale);
            }
        }
    }

    //door
    if(pos.GetX() <  0){
        scaleX = pos.GetX()/-1.33;
    }
    else if(pos.GetX() > 0){
        scaleX = pos.GetX()/1.38;
    }

    if(pos.GetY() < 0){
        scaleY = pos.GetY()/-0.85;
    }
    else if(pos.GetY() > 0){
        scaleY = pos.GetY()/0.81;
    }

    //If inside, return original, or scale and return scaled version
    //cout << "scaleX, scaleY: " << scaleX << ", " << scaleY;
    if (scaleX <= 1 && scaleY <=1){
        return pos;
    }
    else{
        double largestScale = std::max(scaleY, scaleX);
        //cout << "largestScale: " << largestScale << endl;
        return Position(pos.GetX()/largestScale, pos.GetY()/largestScale);
    }
}

bool Robo::isArrived(double radius){
    return this->GetPos().DistanceTo(targetPosition) < radius;
}

void Robo::updatePositions(vector<Position> positions) {
    //Hacky way to make things work. Sorry. --- You are forgiven, we all make mistakes.
    posTeam.push_back(Position(0.0, 0.0));
    for(int p = 0; p < (int)positions.size(); p++) {
        if (p <= 2) {
            posTeam[p] = positions[p];
        } else {
            posOtherTeam[p - 3] = positions[p];
        }
    }
    posTeam.erase(posTeam.begin() + rfNumber);
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

void Robo::updatePidsGoalie(Position targetPos){
    updateDistancePid(targetPos);
    updateAnglePidGoalie(targetPos);
}

void Robo::updateDistancePid(Position targetPos){
    //double dist_error = this->GetPos().DistanceTo(targetPos);
    //if (dist_error < ( precise ? 0.15 : 0.06 )){
        //pidDistance.updateInput(5*dist_error);
    //}
    //else{
        pidDistance.updateInput(speed);
    //}
}

void Robo::setPrecise(bool val){
    precise = val;
}

void Robo::updateAnglePidWithoutCA(Position targetPos){
    this->angleErrorRad = getReferenceAngleErrRad(targetPos, toggleCA);
    double sinAngleErrorRad = sin(this->angleErrorRad/2);
    pidAngle.updateInput(sinAngleErrorRad);
}
void Robo::updateAnglePidWithCA(Position targetPos){
    this->angleErrorRad = getReferenceAngleErrRad(targetPos, toggleCA);
    double sinAngleErrorRad = sin(this->angleErrorRad/2);
    pidAngle.updateInput(sinAngleErrorRad);
}

void Robo::updateAnglePidGoalie(Position targetPos){
    this->angleErrorRad = getGoalieReferenceAngleErrRad(targetPos);
    double sinAngleErrorRad = sin(this->angleErrorRad/2);
    pidAngle.updateInput(sinAngleErrorRad);
}
void Robo::makeTurn(){
    this->updateAnglePidWithoutCA(targetPosition);
    double angleInput = pidAngle.getInput();
    double rightWheel = +angleInput;
    double leftWheel = -angleInput;

    cout << "leftwheel: " << leftWheel << endl;
    cout << "rightwheel: " << rightWheel << endl;
    this->MoveMs(leftWheel, rightWheel,100,10);
}

//DRIVE - related functions
void Robo::goalieDrive(){
    isGoalkeeper = true;
    if(isIdle){
        return;
    }
    if(onlyTurn){
        makeTurn();
    }else{
        updatePidsGoalie(targetPosition);

        double driveSpeed = pidDistance.getInput();
        double angleInput = pidAngle.getInput();

        if (ballBehindRobo){
            driveSpeed *= -1;
            //angleInput *= -1;
        }
        double rightWheel = driveSpeed+ angleInput;
        double leftWheel = driveSpeed- angleInput;
        if(DEBUG){
            cout << "leftwheel: " << leftWheel << endl;
            cout << "rightwheel: " << rightWheel << endl;
        }
        this->MoveMs(leftWheel,rightWheel, 100, 10);
    }
}


//Drive functions must be run 100 times a second for robot to drive. Target position set by Goto()
void Robo::driveWithCA() {
    isGoalkeeper = false;
    if(isIdle){
        return;
    }
    if(onlyTurn){
       makeTurn();
    }
    else{
          // CHECK IF IN SAME POSITION WHILE TRYING TO MOVE, IF TOO LONG, DRIVE TOWARDS MIDDLE
        if(GetPos().DistanceTo(sampeled_pos) < 0.05){
            not_moving_count++;
        }
        else{
            sampeled_pos = GetPos();
            not_moving_count = 0;
        }
        if (not_moving_count > 150){
            go_to_mid = true;
            not_moving_count = 0;
        }

        if(go_to_mid){
            updatePids(Position(0,0), true);
            go_to_mid_count++;
            if(go_to_mid_count > 50){
                go_to_mid = false;
                go_to_mid_count = 0;
            }
        }
        else{
            updatePids(targetPosition, true);
        }
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
        myAngleDeg = ((int)myAngleDeg + 180 + 4*180) % (2*180);
        ballBehindRobo = true;
        return getDiffBetweenAnglesRad(ref_deg, myAngleDeg);
    }
    else{
        ballBehindRobo = false;
        return diffAngle;
    }
}


double Robo::getReferenceAngleErrRad(Position targetPos, bool ca = true){
    //get the error
    Position myPos = this->GetPos();
    double ref_deg;
    if (ca){
        ref_deg = getRefAngleWithCA(this->ca.getTotalPull(myPos, targetPos, posTeam, posOtherTeam, false), targetPos);
    }
    else if(avoidBall){
        ref_deg = getRefAngleWithCA(this->ca.getBallPull(myPos, targetPos, ball.GetPos()), targetPos);
    }else{
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

void Robo::setAvoidBall(bool avoid){
    avoidBall = avoid;
}
