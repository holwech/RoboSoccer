/** File for global type definitions.
 *
 *  @author Julian Straub
 *  @date 01.09.2009
 *
 */

#ifndef TYPES_H_
#define TYPES_H_


#include <deque>
#include <math.h>
#include <iostream>

#include "angle.h"
#include "position.h"
#include "share.h"
#include "kogmo_time.hxx"


using namespace KogniMobil;

//######################################## Structs ##################################

/** Struct for storing the status of the robot in the RTDB.
 *
 */
typedef struct
{
	/** Mac of this Pololu. */
	uint8_t mac[6];
	/** Distance sensor information.
	 * Format: 	Bit2 = left sensor / Bit1 = middle sensor / Bit0 = right sensor
	 */
	uint8_t distanceSensors;

	/** Voltage of Pololu 3pi in mV. */
	uint32_t accuVoltage;

	/** Holds the status of the module itself. Like for example STATUS_INT_GOTOXY_FINISHED. */
	eModuleStatus moduleStatus;
	/** Holds the status of the localization of the robot. */
	eLocalizationStatus localizationStatus;
	/** Holds the status of movement of the robot. */
	eMovingStatus movingStatus;
	//These variables are actualized by the commands which are send.
	int32_t leftSpeed;
	int32_t rightSpeed;
	uint32_t msToGo;
	//Position and orientation extracted from camera image
	double x; 		// [m]
	double y; 		// [m]
	Angle phi;		// [rad]
	bool localized;

} kogmo_rtdb_subobj_pololu_status_t;

/** Struct for storing all localization information */
typedef struct
{
	Position pos [MAX_TEAMSIZE*2];
	Angle phi [MAX_TEAMSIZE*2];
	bool localize [MAX_TEAMSIZE*2];


} kogmo_rtdb_subobj_pololu_localize_t;

/** Struct for sending commands via the RTDB to a robot.
 *
 */
typedef struct
{
	uint8_t rfcommNr;
// contains the command to execute the necessary data is determined by the command...
	eCmd cmd;
// for kicking
	eKick direction;
	uint8_t force;
// for the rotation
	Angle degree;
// for general movement
	int32_t leftSpeed;
	int32_t rightSpeed;
	uint32_t ms;
	uint32_t rampUp;
	bool precise;
// Position and orientation
	double x; 		// [m]
	double y; 		// [m]
	Angle phi;		// [rad]
// length of for example a vector
	double length;		// [m]

} kogmo_rtdb_subobj_pololu_cmd_t;

/** Struct for retrieving the positions of the robots of a team from the RTDB.
 *
 */
typedef struct
{
    int members;
    double pos_x[MAX_TEAMSIZE];
    double pos_y[MAX_TEAMSIZE];
    double phi[MAX_TEAMSIZE];
} kogmo_rtdb_subobj_rfb_team_t;

/** Struct for retrieving the position, the velocity and the direction of the ball from the RTDB.
 *
 */
typedef struct
{
    int found;
    double pos_x;
    double pos_y;
    double phi;
    double velocity;
} kogmo_rtdb_subobj_rfb_ball_t;

/** Struct for the information from the referee.
 *
 */
typedef struct
{
	ePlayMode playMode;

	// used to announce which side has to do for example the kick off or the penalty
	eSide side;

	eSide blue;
	//bool leftReady;
	//bool rightReady;
	int leftSideGoals;
	int rightSideGoals;

	Timestamp startTimer;
	bool timerStarted;
	int gameTime;

} kogmo_rtdb_subobj_pololu_referee_t;

typedef struct
{
	KogniMobil::Timestamp red_ready;
	KogniMobil::Timestamp blue_ready;
} kogmo_rtdb_subobj_pololu_referee2_t;


typedef struct {
	double dist_to_old_pos;
	Angle diff_to_old_angle;
	double speed;
	double distLim;
} localizePositions;



#endif /* TYPES_H_ */
