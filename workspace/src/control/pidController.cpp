#include "pidController.h"
#define NUM_SAVED_ERR 3
#define TIMESTEP 0.01
#define NUM_SAVED_INP 2

#include <math.h>
#include <iostream>
//soccerlab_vision -g is the name i allways forget
/**
 * @brief Constructor, init constants and and error list
 *
 * @param input_Kp
 * @param input_Ki
 * @param input_Kd
 */
pidController::pidController(double input_Kp, double input_Ki, double input_Kd)
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

/**
 * @brief Change PID parameters
 *
 * @param newKp
 * @param newKi
 * @param newKd
 */
void pidController::changeParams(double newKp, double newKi, double newKd){
    Kp = newKp;
    Ki = newKi;
    Kd = newKd;
}

/**
 * @brief insert new reference error value in list
 *
 * @param diff
 */
void pidController::saveNewErr(double diff){
    if(error_sign == diff/fabs(diff) || error_sign == 0){
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
    error_sign = diff/fabs(diff);
//    std::cout << "Diff: " << diff << std::endl;
//    for (int i = 0; i < NUM_SAVED_ERR; i++){
//         std::cout << i << ": " << prevErr[i] << std::endl;
//    }
    //cout << endl;
}
/**
 * @brief Get new input value for PID controller
 *
 * @param error
 */
void pidController::updateInput(double error){
    saveNewErr(error);
}
/**
 * @brief save the new input value in list
 *
 * @param input
 */
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
/**
 * @brief get the new output from pid, input to sys
 *
 * @return double
 */
double pidController::getInput(){
    //u[k] = u[k-1] + a*e[k] + b*e[k-1] + c*e[k-2]
    //double u = prevInp[0] + a*prevErr[0] + b*prevErr[1] + c*prevErr[2];
    double derivative = ((prevErr[0]-prevErr[1])/TIMESTEP + (prevErr[1]-prevErr[2])/TIMESTEP)/2;
    double u = Kp*prevErr[0] + Ki*(prevErr[0]+prevErr[1] + prevErr[2])*TIMESTEP + Kd*(derivative);
    saveNewInput(u);
    return u;
}
