#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H
#include <vector>
#include <robo_control.h>
class pidController{
    vector<double> prevErr;
    vector<double> prevInp;
    RoboControl * robot_ptr;
    double Kp;
    double Ki;
    double Kd;
    double a;
    double b;
    double c;
    double err;
    int error_sign;
public:
    pidController();
    pidController(double input_Kp, double input_Ki, double input_Kd);
    void updateInput(double error);
    void saveNewErr(double diff);
    void saveNewInput(double input);
    double getInput();
    void setError(double error);
};

#endif // PIDCONTROLLER_H
