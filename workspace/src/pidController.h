#ifndef PIDCONTROLLER_H
#define PIDCONTROLLER_H
#include <vector>

class Robo;
class pidController{
    std::vector<double> prevErr;
    std::vector<double> prevInp;
    Robo * robot_ptr;
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
    void changeParams(double newKp, double newKi, double newKd);
};

#endif // PIDCONTROLLER_H
