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


using namespace std;

int main(void) {
	//--------------------------------- Init --------------------------------------------------

	/** Use client number according to your account number!
	 *
	 *	This is necessary in order to assure that there are unique
	 *	connections to the RTDB.
	 *
	 */
        const int client_nr = 6;

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
        int rfcomm_nr = 0;

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
		RoboControl robo(DBC, rfcomm_nr);

		/** Now let's print out some information about the robot... */
                //uint8_t mac[6];
                //robo.GetMac(mac);
                cout << "Robo @ rfcomm" << rfcomm_nr << endl; /*<<" with Mac: ";


		for (int j = 0; j < 5; j++)
			cout << hex << (int) mac[j] << ":";
		cout << hex << (int) mac[5] << endl;
*/
                cout << "\t Battery Voltage: " << dec << (int) robo.GetAccuVoltage()
                                << "mV" << endl;
		cout << "\t initial position: " << robo.GetPos() << endl;
		cout << "\t initial rotation: " << robo.GetPhi() << endl;

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

               // Position pos2(-0.1, 0);

                //Position pos4(0.8, 0);


			/** Sequentially move to the four different positions.
			 *  The while is excited if the position is reached.
			 */

                         Position pos1(ball.GetX()-0.5,ball.GetY());
                         cout << "Moving to " << pos1 << endl << endl;
                         robo.GotoXY(pos1.GetX(), pos1.GetY(), 30, true);
                         while (robo.GetPos().DistanceTo(pos1) > 0.05) usleep(50000); //sleep function in microseconds
                         //Camera sampling rate is 30fps -> 33ms
                         //which means that field info does not change within this time



                         Position pos2(ball.GetX()-0.3,ball.GetY());
                         cout << "Moving to " << pos2 << endl << endl;
                         robo.GotoXY(pos2.GetX(), pos2.GetY(), 60, true);
                         while (robo.GetPos().DistanceTo(pos2) > 0.1) usleep(50000); //sleep function in microseconds
                         //Camera sampling rate is 30fps -> 33ms
                         //which means that field info does not change within this time


                         Position pos3(ball.GetX(),ball.GetY());
                         cout << "Moving to " << pos3 << endl << endl;
                         robo.GotoXY(pos3.GetX(), pos3.GetY(), 100, true);
                         while (robo.GetPos().DistanceTo(pos3) > 0.1) usleep(50000); //sleep function in microseconds
                         //Camera sampling rate is 30fps -> 33ms
                         //which means that field info does not change within this time


                         Position pos4(ball.GetX() + 0.3,ball.GetY());
                         cout << "Moving to " << pos4 << endl << endl;
                         robo.GotoXY(pos4.GetX(), pos4.GetY(), 160, true);
                         while (robo.GetPos().DistanceTo(pos4) > 0.1) usleep(50000); //sleep function in microseconds
                         //Camera sampling rate is 30fps -> 33ms
                         //which means that field info does not change within this time

                         //Position pos3(ball.GetX()+0.5,ball.GetY());
                         //cout << "Moving to " << pos3 << endl << endl;
                         //robo.GotoXY(pos3.GetX(), pos3.GetY(), 100, true);
                         //while (robo.GetPos().DistanceTo(pos3) > 0.1) usleep(50000);


                        //cout << "Moving to " << pos4 << endl << endl;
                        //roboball.GetPos().GotoXY(pos4.GetX(), pos4.GetY(), 60, true);
                        //while (robo.GetPos().DistanceTo(pos4) > 0.10) usleep(50000);


                robo.Kick(100,0.0);

	} catch (DBError err) {
		cout << "Client died on Error: " << err.what() << endl;
	}
        cout << "End" << endl;
	return 0;
}


