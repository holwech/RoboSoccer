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
#include "share.h"
#include "referee.h"
#include "raw_ball.h"
using namespace std;

int main(void) {
	//--------------------------------- Init --------------------------------------------------

	/** Use client number according to your account number!
	 *
	 *	This is necessary in order to assure that there are unique
	 *	connections to the RTDB.
	 *
	 */

        const int client_nr = 9;


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

        //ePlayMode
        int rfcomm_nr_attacker = 0;
        int rfcomm_nr_1 = 1;
        int rfcomm_nr_2 = 2;



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

                RoboControl robo_attacker(DBC, rfcomm_nr_attacker);
                RoboControl robo1(DBC, rfcomm_nr_1);
                RoboControl robo2(DBC, rfcomm_nr_2);
		/** Now let's print out some information about the robot... */
                //uint8_t mac[6];
                //robo.GetMac(mac);
                cout << "Robo @ rfcomm" << rfcomm_nr_attacker << endl; /*<<" with Mac: ";


		for (int j = 0; j < 5; j++)
			cout << hex << (int) mac[j] << ":";
		cout << hex << (int) mac[5] << endl;
*/
                cout << "\t Battery Voltage: " << dec << (int) robo_attacker.GetAccuVoltage()
                                << "mV" << endl;
                cout << "\t initial position: " << robo_attacker.GetPos() << endl;
                cout << "\t initial rotation: " << robo_attacker.GetPhi() << endl;

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
                Referee myreferee(DBC);
                myreferee.Init();
                myreferee.GetPlayMode();
                int attpos=0;
		//-------------------------------------- Ende Init ---------------------------------

		/** Define four positions which form a rectangle...
		 *
		 */

                Position corner1(1.2,-0.8);

                Position corner2(1.2,0.8);


                // Referee myreferee(class RTDBConn & DBC, const char * name = "rtdb_referee", const int & otype = KOGMO_RTDB_OBJTYPE_POLOLU, const int32_t & child_size = 0,char ** child_dataptr = NULL);
                //eSide kickside=myreferee.GetSide();
                //cout << myreferee.GetPlayMode() << "blabla" << endl;
                while (myreferee.GetPlayMode()== BEFORE_PENALTY){

                    Position pos1(0.0,0.0);
                    cout << "Moving to " << pos1 << endl << endl;
                    robo_attacker.GotoXY(pos1.GetX(), pos1.GetY(), 60, true);
                    robo1.GotoXY(1.2, -0.8, 100, false);
                    robo2.GotoXY(1.2, 0.8, 100, false);
                    while (robo1.GetPos().DistanceTo(corner1) > 0.10 ||
                           robo2.GetPos().DistanceTo(corner2) > 0.10 ||
                           robo_attacker.GetPos().DistanceTo(pos1) > 0.05)
                           {usleep(50000);}
                    }
                if (myreferee.GetPlayMode()== PENALTY){
                     cout << "Referee works" << endl;
                     while (attpos==0){
                         Position pos1(ball.GetX()+0.5,ball.GetY());
                         cout << "Moving to pos 1"<< endl << endl;
                         robo_attacker.CruisetoXY(pos1.GetX(), pos1.GetY(), 150, true);
                         usleep(5000);
                         if (robo_attacker.GetPos().DistanceTo(pos1) < 0.05) attpos=1; //sleep function in microseconds
                     }
                     while (attpos==1){
                         Position pos2(ball.GetX()+0.3,ball.GetY());
                         cout << "Moving to pos2" << endl << endl;
                         robo_attacker.GotoXY(pos2.GetX(), pos2.GetY(), 120, true);
                         usleep(5000);
                         if (robo_attacker.GetPos().DistanceTo(pos2) < 0.1) attpos=2; //sleep function in microseconds
                    }
                     while (attpos==2){
                         Position pos3(ball.GetX()+0.1,ball.GetY());
                         cout << "Moving to " << pos3 << endl << endl;
                         robo_attacker.CruisetoXY(pos3.GetX(), pos3.GetY(), 100, true);
                         usleep(5000);
                         if (robo_attacker.GetPos().DistanceTo(pos3) < 0.1) attpos=3; //sleep function in microseconds
                    }
                     if (attpos==3){

                         //Position pos4(ball.GetX() - 0.1,ball.GetY());
                         cout << "Moving forward" << endl;
                         //robo_attacker.CruisetoXY(pos4.GetX(), pos4.GetY(), 160, true);
                         robo_attacker.MoveMs(200,200,2000);
                         usleep(5000);
                         //if (robo_attacker.GetPos().DistanceTo(pos4) < 0.1)
                             attpos=4;//sleep function in microseconds
                    }
                    /*while (attpos==4){
                         cout<<"start kick"<<endl;
                    robo_attacker.Kick(100,0.5);
                    }*/

              }

	} catch (DBError err) {
		cout << "Client died on Error: " << err.what() << endl;
	}
        cout << "End" << endl;
	return 0;
}

