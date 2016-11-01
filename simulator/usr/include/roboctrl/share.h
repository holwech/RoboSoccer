
/** File for global constants and enums.
 *
 *  @author Julian Straub
 *  @date 01.09.2009
 *
 */

#ifndef SHARE_H_
#define SHARE_H_

/** Command codes for the control over the robot via bluetooth.*/
enum eCmd
{
	CMD_NO_CMD 					= 0x0,
	CMD_SET_MOTOR_SPEEDS 		= 0x11,
	CMD_MOVE_MS 				= 0x12,
	CMD_KICK 					= 0x13,
	CMD_TURN_DEGREES 			= 0x14,
	CMD_DEBUG_MSG 				= 0x18,
	CMD_GET_MAC 				= 0x20,
	CMD_GET_VOLTAGE				= 0x21,
	CMD_GET_SENSORS				= 0x22,
	CMD_MOVE_MS_NO_ACK			= 0x23,
	CMD_MOVE_DIST				= 0x25,
	// For confirm messages of the robot.
	CMD_SET_MOTOR_SPEEDS_CFM 	= 0x31,
	CMD_MOVE_MS_CFM				= 0x32,
	CMD_KICK_CFM 				= 0x33,
	CMD_TURN_DEGREES_CFM 		= 0x34,
	CMD_MOVE_FINISHED 			= 0x35,
	CMD_GET_MAC_CFM				= 0x40,
	CMD_GET_VOLTAGE_CFM			= 0x41,
	CMD_GET_SENSORS_CFM			= 0x42,

	CMD_CRC_ERROR				= 0x43,
	// Internal  commands.
	CMD_INT_SET_POSITION 		= 0x60,
	CMD_INT_SET_POSITION_ODO 	= 0x61,
	CMD_INT_INIT_POSITION 		= 0x62,
	CMD_INT_GOTO_XY				= 0x63,
	CMD_INT_ABORT_GOTO_XY 		= 0x64,
	CMD_INT_MOVE_VECT 			= 0x65,
	CMD_INT_ABORT_MOVE_VECT 	= 0x66
};
/** Directions of the Kick:
 * Not used for now. Always use KICK_FORWARD as the robots do not have a kind of kicking-bar...
 */
enum eKick
{
	KICK_FORWARD 	= 1,
	KICK_LEFT 		= 2,
	KICK_RIGHT 		= 3
};
/** Defines for reading out the distance sensors. */
enum eSensor
{
	SENSOR_LEFT 	= 4,
	SENSOR_MIDDLE 	= 2,
	SENSOR_RIGHT 	= 1,
	SENSOR_ALL 		= 7
};

/** Defines for the moving status of the robot. ->movingStatus */
enum eMovingStatus
{
	STATUS_MOVING_STOPPED,
	STATUS_MOVING,
	STATUS_TURNING
};
/** More complex status of the pololu_rtdb_module itself. -> moduleStatus */
enum eModuleStatus
{
	STATUS_INT_NOTHING_IN_PROGRESS,
	STATUS_INT_GOTOXY_RUNNING,
	STATUS_INT_GOTOXY_FINISHED,
	STATUS_INT_MOVE_VECT_RUNNING,
	STATUS_INT_MOVE_VECT_FINISHED
};
/** Status of the localization of the robot. -> localizationStatus */
enum eLocalizationStatus
{
	STATUS_INT_NOT_LOCALIZED,
	STATUS_INT_LOCATING,
	STATUS_INT_JUST_LOCALIZED,
	STATUS_INT_LOCALIZED
};
/** object type numbers for the RTDB. */
enum kogmo_rtdb_objtype_rfb
{
  KOGMO_RTDB_OBJTYPE_RFB_REDTEAM                    = 0xFB0007,
  KOGMO_RTDB_OBJTYPE_RFB_BLUETEAM                   = 0xFBC001,
  KOGMO_RTDB_OBJTYPE_RFB_BALL                       = 0xFBBA11,
};

/** Status of the GotoXY state-machine. */
#define GOTOXY_STATE_START				0x00
#define GOTOXY_STATE_TURNING_START		0x01
#define GOTOXY_STATE_TURNING			0x02
#define GOTOXY_STATE_TURNING_END		0x03
#define GOTOXY_STATE_MOVING_START		0x04
#define GOTOXY_STATE_MOVING				0x05
#define GOTOXY_STATE_WAIT_MOVEMENT_END	0x06
#define GOTOXY_STATE_CHECK_ROUTE		0x07
#define GOTOXY_STATE_FINISHED 			0x08
#define GOTOXY_STATE_WAIT_CAM_1			0x09
#define GOTOXY_STATE_WAIT_CAM_2			0x10
/** Status of the MoveVect state-machine. */
#define MOVE_VECT_STATE_START				0x00
#define MOVE_VECT_STATE_TURNING_START		0x01
#define MOVE_VECT_STATE_TURNING				0x02
#define MOVE_VECT_STATE_TURNING_END			0x03
#define MOVE_VECT_STATE_MOVING_START		0x04
#define MOVE_VECT_STATE_MOVING				0x05
#define MOVE_VECT_STATE_MOVING_END			0x06
#define MOVE_VECT_STATE_FINISHED 			0x07
#define MOVE_VECT_STATE_WAIT_CAM 			0x08


// Playground dimensions: (the dimensions in which the robots are allowed to
// move -> the real dimension is half the radius of a robot biogger)
// shrinked the playground -> 1cm on every side to avoid crashes with the walls
#define LEFT_SHORTER_SIDE_X		-1.39 //-1.313
#define RIGHT_SHORTER_SIDE_X	1.39  //1.418
#define NEARER_LONG_SIDE_Y		0.829 //0.629
#define FARTHER_LONG_SIDE_Y		-0.829//-1.028

// defines for the goal area
#define GOAL_AREA_DEPTH 		0.15
#define GOAL_AREA_WIDTH			0.47

// defines for the penalty area
#define PENALTY_AREA_DEPTH		0.448
#define PENALTY_AREA_WIDTH		1.04

/** For ms computation in the turn command. */
#define MULTIPLIER_SPEED_80		1.97 //2.89
#define OFFSET_SPEED_80			8.56 //6.67

//* pololu_control.h
#define MAX_INPUT_BUFFER_SIZE		255
//serial.h
#define SERIAL_LOW_LEVEL_BUFSIZE	255
// I dont know what this is good for, but it is necessary to have a value here
#define KOGMO_RTDB_OBJTYPE_POLOLU 	99

// measured / computed real speeds [m/s] at different raw speeds
#define DEFAULT_RAMPUP_TIME	200  //[ms]
#define RAMPUP_DIST			0.08 //[m]   @160 speed
#define SPEED_AT_250		0.90 //[m/s] just a "good" guess
#define SPEED_AT_160		0.51 //[m/s]
#define SPEED_AT_80			0.34 //[m/s]

// pololu dimensions
#define POLOLU_RADIUS	82.5 * 0.5	//[mm]
#define DIST_TO_RAD		1.0 / POLOLU_RADIUS
#define DIST_TO_DEG		DIST_TO_RAD * RAD_TO_DEG

/** Defines for the teams. */
enum eTeam {
	BLUE_TEAM = 0,
	RED_TEAM = 1
};

// defines for the Cam
#define MIN_BALLS 		0
#define MAX_BALLS 		100
#define MIN_TEAMSIZE 	0
#define MAX_TEAMSIZE 	11
#define NUM_TEAMS 		2
#define INVALID_DATA 	5000

/** Defines for the playground-sides.
 * Out of the perspective of standing in front of the
 * playground facing the windows.
 */
enum eSide
{
	LEFT_SIDE = 0,
	RIGHT_SIDE = 1,
	OWN_SIDE = 2
};
/** Play modes which are announced by the referee and have to be obeyed.
 * The playmodes can be obtained via the referee class. ( -> GetPlayMode())
 */
enum ePlayMode
{
	/** Init status, no new data */
	REFEREE_INIT,
	/** Get your players ready for the kick-off */
	BEFORE_KICK_OFF,
	/** Now the game is free for the kick-off*/
	KICK_OFF,
	/** Get the players ready for penalty */
	BEFORE_PENALTY,
	/** Now the penalty can be executed */
	PENALTY,
	/** Default play mode in which the game is played */
	PLAY_ON,
	/** Game is paused -> stop all movements! */
	PAUSE,
	/** The timer has expired -> stop your players and party if you won ;-) */
	TIME_OVER
};
/** Force of the kick*/
enum eForce
{
	SOFT,
	MEDIUM,
	/** be careful with HARD - might be to hard as the robot's wheels might slip.
	 * This reduces the accuracy of the kick significantly.
	 */
	HARD
};

double const COMMAND_PERIOD_US = 10000;
double const COMMAND_PERIOD_S = COMMAND_PERIOD_US/1000000;

#endif /* SHARE_H_ */
