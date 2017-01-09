#include "master/master.h"

/** Include all strategies here
 */


void Master::strategy_offensive(){
}

void Master::strategy_defensive(){
    // Fix this so it works for both sides
    if (ball.GetPos().GetX() > -0.2) {
        s_case = BLOCK;
    } else if (ball.GetPos().GetX() < -0.2) {

    }
    switch(s_case) {
    case BLOCK:
        break;
    default:
        cout << "No case for this case in strategy defensive" << endl;
        break;
    }
}

void Master::strategy_demo(){

    if(referee.GetLeftSideGoals() > referee.GetRightSideGoals()){

    //do strategy_defensive();

        }


    //do strategy_offensive();




    /*
     * if ahead on score:
     * 		do strategy_defensive();
     * else:
     * 		do strategy_offensive();
     */
}
