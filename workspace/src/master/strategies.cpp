#include "master/master.h"

/** Include all strategies here
 */


void strategy_offensive(){

}
void Master::strategy_defensive(){
    //send(Command(ACTION_DEFEND), 0);
    //send(Command(ACTION_BLOCK_BALL), 1);
    send(Command(ACTION_BEFORE_KICK), 1);
}

void Master::strategy_demo(){
    /*
     * if ahead on score:
     * 		do strategy_defensive();
     * else:
     * 		do strategy_offensive();
     */
}
