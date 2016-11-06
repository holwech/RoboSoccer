//============================================================================
// Name        : soccer_client.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Client for RTDB which controls team 1, Ansi-style
//============================================================================


#include <time.h>
#include <iostream>
#include <vector>
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

        int client_nr;

        cout << endl << "Please write your client number: ";
        cin >> client_nr;

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

        string answer;
        bool cont = false;
        while (!cont) {
            cout << "Which team do you want to control? (blue/red) ";
            cin >> answer;
            if (answer == "blue" || answer == "red") {
                cont = true;
                cout << "Initiating for " << answer << endl;
            }
        }

	try {

                /** Establish connection to the RTDB. */
		cout << endl << "Connecting to RTDB..." << endl;
		/** Create the client name with the unique client number*/
                string client_name = "pololu_client_";
		client_name.push_back((char) (client_nr + '0'));
		RTDBConn DBC(client_name.data(), 0.1, "");

                /** Init robot(s) */
                vector<RoboControl> robots;
                int rfNumber0, rfNumber1, rfNumber2;
                if (answer == "blue") {
                    rfNumber0 = 0;
                    rfNumber1 = 1;
                    rfNumber2 = 2;
                } else {
                    rfNumber0 = 3;
                    rfNumber1 = 4;
                    rfNumber2 = 5;
                }

                RoboControl robo0(DBC, rfNumber0);
                RoboControl robo1(DBC, rfNumber1);
                RoboControl robo2(DBC, rfNumber2);
                cout << "Robots are ready" << endl;

                /** Create a ball object */
                RawBall ball(DBC);

                cout << "Program starting..." << endl;

                //-------------------------------------- Ende Init ---------------------------------
                Position pos0(0.6, 0.6);
                Position pos1(0.6, -0.6);
                Position pos2(-0.6, 0.6);
                while(1) {
                    robo0.GotoXY(pos0.GetX(), pos0.GetY(), 100, true);
                    robo1.GotoXY(pos1.GetX(), pos1.GetY(), 100, true);
                    robo2.GotoXY(pos2.GetX(), pos2.GetY(), 100, true);
                    while (robo0.GetPos().DistanceTo(pos0) > 0.05 ||
                           robo1.GetPos().DistanceTo(pos1) > 0.05 ||
                           robo2.GetPos().DistanceTo(pos2) > 0.05) {
                       usleep(50000);
                    }
                }

	} catch (DBError err) {
		cout << "Client died on Error: " << err.what() << endl;
	}
        cout << "End" << endl;
	return 0;
}


