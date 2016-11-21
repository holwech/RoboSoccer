#include "pidController.h"
#define NUM_SAVED_ERR 3
#define TIMESTEP 0.001
#define NUM_SAVED_INP 2

class RoboControl;
pidController::pidController(RoboControl * robo_ptr, double input_Kp, double input_Ki, double input_Kd)
                            : Kp(input_Kp), Ki(input_Ki), Kd(input_Kd), error_sign(0){
    //Init error list
    for(int i = 0; i < NUM_SAVED_ERR; i++){
        prevErr.push_back(0);
    }
    for(int i = 0; i < NUM_SAVED_INP; i++){
        prevInp.push_back(0);
    }
    //Init konstants for update function
    a = Kp + Ki*(TIMESTEP/2) + Kd/TIMESTEP;
    b = -Kp + Ki*(TIMESTEP/2) -2*Kd/TIMESTEP;
    c = Kd/TIMESTEP;
}

void pidController::saveNewErr(double diff){
    if(error_sign == diff/abs(diff) || error_sign == 0){
        double tempErrA = diff;
        double tempErrB;
        for (int i = 0; i < NUM_SAVED_ERR; i++){
            tempErrB = prevErr[i];
            prevErr[i] = tempErrA;
            tempErrA = tempErrB;
        }
    }
    else {
        for(int i = 0; i < NUM_SAVED_ERR; i++){
            prevErr[i] = 0;
        }
        prevErr[0] = diff;
    }
    error_sign = diff/abs(diff);
    for (int i = 0; i < NUM_SAVED_ERR; i++){
        // cout << i << ": " << prevErr[i] << endl;
    }
    //cout << endl;
}
void pidController::updateInput(double error){
    saveNewErr(error);
}
void pidController::setError(double error){
    err = error;
}
void pidController::saveNewInput(double input){
    double tempInpA = input;
    double tempInpB;
    int i;
    for (i = 0; i < NUM_SAVED_INP; i++){
        tempInpB = prevInp[i];
        prevInp[i] = tempInpA;
        tempInpA = tempInpB;
    }
}
double pidController::getInput(){
    //u[k] = u[k-1] + a*e[k] + b*e[k-1] + c*e[k-2]
    double u = prevInp[0] + a*prevErr[0] + b*prevErr[1] + c*prevErr[2];
    saveNewInput(u);
    return u;
}
