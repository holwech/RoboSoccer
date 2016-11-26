#include "robo.h"
#include "collision_avoidance.h"


/** To complete task 2.1 part 1, we have to implement multithreading it seems
  * like. We will have problems controlling all robots at the same time without
  */
void Robo::run(cpp::channel<Position> positionCh) {
    while (1) {
    }
}

/**
TEST(ChannelTest, SelectRecv)
{
  cpp::channel<char> c;
  cpp::ichannel<char> in(c);
  char i = '\0';

  std::thread a(send_chars<'F'>, c);
  cpp::thread_guard a_guard(a);

  cpp::select().recv_only(c, i).wait();
  EXPECT_EQ('A', i);

  cpp::select().recv(c, i, [](){}).wait();
  EXPECT_EQ('B', i);

  cpp::select().recv_only(in, i).wait();
  EXPECT_EQ('C', i);

  cpp::select().recv(in, i, [](){}).wait();
  EXPECT_EQ('D', i);

  cpp::select().recv(c, [&i](const char k) { i = k; }).wait();
  EXPECT_EQ('E', i);

  cpp::select().recv(in, [&i](const char k) { i = k; }).wait();
  EXPECT_EQ('F', i);
}
*/
double getTargetAngleDiffRad(RoboControl& robo, Position targetPos);
double getObstacleAngleDiffRad(RoboControl& robo, Position obstPos);

double getAngleDiffWithCA(double obstAngleDiffRad, double distToObst);

void driveWithCA(RoboControl& robo1, RawBall &ball, Position obstPos ,pidController &pidAngle, pidController &pidDistance){
    usleep(10000);
    double targetDiff_rad = getTargetAngleDiffRad(robo1, ball.GetPos());

    double sinTargetDiff_rad = sin(targetDiff_rad/2);

    double dist_error = robo1.GetPos().DistanceTo(ball.GetPos());
    if (dist_error < 0.03){
        pidDistance.updateInput(dist_error);
    }
    else{
        pidDistance.updateInput(1);
    }

    double obstAngleDiffRad = getObstacleAngleDiffRad(robo1, obstPos);
    double distToObst = robo1.GetPos().DistanceTo(obstPos);

    double total_update_angle_rad = sinTargetDiff_rad + getAngleDiffWithCA(obstAngleDiffRad, distToObst);
    pidAngle.updateInput(total_update_angle_rad);
    double driveSpeed = pidDistance.getInput();

    // the cos will make it drive fastest in the right direction, and also back up and turn if behind you
    double rightWheel =driveSpeed*cos(targetDiff_rad) + pidAngle.getInput();
    double leftWheel = driveSpeed*cos(targetDiff_rad) - pidAngle.getInput();
    robo1.MoveMs(leftWheel,rightWheel, 100, 10);
    //out << endl <<robo1.GetPos().AngleOfLineToPos(ball.GetPos())-robo1.GetPhi() << endl;
    //robo1.TurnAbs(robo1.GetPos().AngleOfLineToPos(ball.GetPos())-robo1.GetPhi());
}

double getAngleDiffWithCA(Force obstForce){
    //get the magnitude of force
    double forceMagnitude = sqrt(pow(obstForce.X, 2) + pow(obstForce.Y, 2));

    if( forceMagnitude < 1 || obstAngleDiffRad > M_PI/2){
        return 0; //No collition avoidance needed
    } else{
        // collition avoidance needed
        // right sign,          more turn the more direct treath, 4 is just scaling. Last part increases when closer to obstacle. TO BE CHANGED FOR JOACHIMS STUFF
        return -obstAngleSign * cos(obstAngleDiffRad)/4          * 0.3/(distToObst);
    }
}

double getTargetAngleDiffRad(RoboControl& robo, Position targetPos){
    //get the error
    double ref_deg = robo.GetPos().AngleOfLineToPos( targetPos).Deg();
    double myAngle_deg = robo.GetPhi().Deg();
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

double getObstacleAngleDiffRad(RoboControl& robo, Position obstPos){
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
