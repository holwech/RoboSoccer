//============================================================================
// Name        : soccer_client.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Client for RTDB which controls team 1, Ansi-style
//============================================================================


#include <time.h>
#include <iostream>
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include <stdlib.h>
#include "share.h"
#include <math.h>


using namespace std;

int main(void) {
        //--------------------------------- Init --------------------------------------------------

        /** Use client number according to your account number!
         *
         *	This is necessary in order to assure that there are unique
         *	connections to the RTDB.
         *
         */

        const int client_nr = 7;



        /** Type in the rfcomm number of the robot you want to connect to.
         *  The numbers of the robots you are connected to can be found on the
         *  screen when you connected to them.
         *
         *  The red robots' number will be in the range of 3 to 5
         *  The blue robots' number will be in the range from 0 to 2
         *
         *  The robots are always connected to the lowest free rfcomm device.
         *  Therefore if you have two blue robots connected the will be
         *  connected to rfcomm number 0 and number 1...
         *
         */
        int rfcomm_nr_0 = 3;
        int rfcomm_nr_1 = 4;
        int rfcomm_nr_2 = 5;

/*
        //Activate the project option "Run in terminal" in Ctrl+5 (Ctrl+2 to come back here)
        //to use "cin" function.

        char cInput;

        cout << "Specify rfcomm number of the robot you want to move. Blue 0-2, Red 3-5: ";
        while (1) {
                cin >> cInput;
                if (!((cInput >= '0') && (cInput <= '7'))) {
                        cout << "Please specify valid rfcomm number between 0 and 7"
                                        << endl;
                } else {
                        rfcomm_nr = cInput-48; //simple "char to int" function
                        break;
                }
        }
*/

        try {

                /** Establish connection to the RTDB.
                 *
                 *  The connection to the RTDB is necessary in order to get access
                 *  to the control and the status of the robots which are both stored
                 *  in the RTDB.
                 *
                 *  In the RTDB there are also informations about the ball and the
                 *  other robot positions.
                 *
                 */
                cout << endl << "Connecting to RTDB..." << endl;
                /** Create the client name with the unique client number*/
                string client_name = "pololu_client_";
                client_name.push_back((char) (client_nr + '0'));
                RTDBConn DBC(client_name.data(), 0.1, "");

                /** Create a new RoboControl object.
                 *
                 *  This is the basis for any communication with the robot.
                 *
                 *  We need to hand over the RTDB connection (DBC) and the rfcomm
                 *  number of the robot we want to control.
                 */
                RoboControl robo0(DBC, rfcomm_nr_0);
                RoboControl robo1(DBC, rfcomm_nr_1);
                RoboControl robo2(DBC, rfcomm_nr_2);


                /** Create a ball object
                 *
                 *  This ball abject gives you access to all information about the ball
                 *  which is extracted from the cam.
                 *
                 */
                RawBall ball(DBC);
                /** lets print this information: */
                cout << "Ball informations:" << endl;
                cout << "\t initial position: " << ball.GetPos() << endl;
                /** Notice that the rotation here refers to the moving direction of the ball.
                 *  Therefore if the ball does not move the rotation is not defined.
                 */
                cout << "\t initial direction: " << ball.GetPhi() << endl;
                cout << "\t initial velocity: " << ball.GetVelocity() << endl;

                //-------------------------------------- Ende Init ---------------------------------

                /** Define four positions which form a rectangle...
                 *
                 */
                Position ballPos(ball.GetPos());

                cout << " WHAAAT" << endl;
                while (1) {
                    /** Sequentially move to the four different positions.
                    *  The while is excited if the position is reached.
                    */
                    ballPos = ball.GetPos();
                    double ballangle = ball.GetPhi().Deg();
                    double velocityy = sin(ballangle)*ball.GetVelocity();
                    double nextPos = ball.GetY() + velocityy * (33 / 1000);
                    if (nextPos > 0.2) {
                        nextPos = 0.2;
                    } else if (nextPos < -0.2) {
                        nextPos = -0.2;
                    }
                    robo0.GotoXY(1.3 , nextPos, 50, true);
                    while (abs(robo0.GetY()-ball.GetY()) > 0.40 && abs(robo0.GetX()-0.4) > 0.40) {
                        usleep(5000);
                    }

                    //sleep function in microseconds
                    //Camera sampling rate is 30fps -> 33ms
                    //which means that field info does not change within this time

                }

        } catch (DBError err) {
            cout << "Client died on Error: " << err.what() << endl;
        }
        cout << "End" << endl;
        return 0;
}


