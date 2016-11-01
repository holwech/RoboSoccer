#ifndef POLOLU_CMD_H_
#define POLOLU_CMD_H_

#include <iostream>
#include <unistd.h>		//für usleep (µSecounds) -> schläft die übergeben Anzahl von µSekunden
#include <deque>
#include <string>
#include <assert.h>

//kogmo_rtdb includes
#include "kogmo_rtdb_obj_base_classes.hxx"
#include "kogmo_rtdb_obj_typeids.h"
#include "kogmo_rtdb_obj_base.h"

#include "share.h"
#include "types.h"

#include "angle.h"
#include "position.h"

//-------------------------------------------------- class -----------------------------------------------------

using namespace std;
using namespace KogniMobil;

/** Class for the exchange of commands via the RTDB
 * The PololuCmd class holds all informations about the current the command.
 * This information can be written into the RTDB. As soon as there is a new cmd in the RTDB
 * the pololu_rtdb_module is executing this command.
 *
 * So the steps for a new command are the following:
 * - First change all necessary paramters of the PololuCmd object
 * - Then set the current data timestamp (.setDataTimestamp())
 * - Last write the data of the PololuCmd object into the RTDB (.RTDBWrite())
 *
 * However it should not be necessary to do this on your own, as the PololuControl
 * class does this for you for all necessary commands.
 * Thus it is recommended to use the PololuControl or the RoboControl class
 * (which inherits from PololuControl) to issue commands to the robot!
 *
 *  @author Julian Straub
 *  @date 31.08.2009
 *
 */
class PololuCmd : public RTDBObj
{
protected:
	/** Struct which holds all information that is stored in the RDTB. */
	kogmo_rtdb_subobj_pololu_cmd_t *obj_pololu_cmd;
public:
	/** Initializes the command information. */
	PololuCmd(class RTDBConn& DBC, const char* name = "", const int& otype =
			KOGMO_RTDB_OBJTYPE_POLOLU, const int32_t& child_size = 0,
			char** child_dataptr = NULL);

    bool NewCmd(void);
	/** Tests whether there is a new command in the RTDB.*/
	/** Turn a specified angle.
	 * 	Clock-wise is positive and counter-clock-wise is negative.
	 * 	@param[in] angle angle which should be rotated as an Angle object.
	 */
	void Turn(const Angle angle)
	{
		obj_pololu_cmd->cmd = CMD_TURN_DEGREES;
		obj_pololu_cmd->degree = angle;
	};
	/** Turn a specified angle.
	 * 	Clock-wise is positive and counter-clock-wise is negative.
	 * 	@param[in] angle angle which should be rotated in radian.
	 */
	void Turn(const double angle)
	{
		this->Turn(Angle(angle));
	};
	/** Turn a specified angle.
	 * 	Clock-wise is positive and counter-clock-wise is negative.
	 * 	@param[in] degree angle which should be rotated in degree.
	 */
	void Turn(const int32_t degree)
	{
		this->Turn(Angle((int32_t)degree).toDouble());
	};
	/** Do a kick.
	 *  Kick in a specified direction with a specified force.
	 *  @warning at the moment only KICK_FORWARD should be used!
	 *  @param[in] direction Direction of the kick (this makes only sense with a mounted kicker).
	 *  @param[in] force Force with which the robot will hit the ball. Equal to the raw motor speeds.
	 */
	void Kick(eKick direction , uint8_t force)
	{
		obj_pololu_cmd->cmd = CMD_KICK;
		obj_pololu_cmd->direction = direction;
		obj_pololu_cmd->force = force;
	};
	/** Only set motor speeds without a time limit
	 * 	@warning do not use this any more!
	 */
	void SetMotorSpeed(int32_t left, int32_t right)
	{
		// this methode should not be used any more...
		assert(false);
		obj_pololu_cmd->cmd = CMD_SET_MOTOR_SPEEDS;
		obj_pololu_cmd->leftSpeed = left;
		obj_pololu_cmd->rightSpeed = right;
	};
	/** Set the motor speeds to specific values with a timeout.
	 * 	A rampuptime can be specified. This is recommended when you want to drive faster than 120
	 * 	otherwise the motors might slip... The default rampuptime is 200ms.
	 *  @param[in] left		left motor speed as a raw value ranging from -255 to 255.
	 *  @param[in] right    right motor speed as a raw value ranging from -255 to 255.
	 *  @param[in] ms       ms which the movement should last.
	 *  @param[in] rampUp   ramup time. The motors will get faster in a linear way during this period of time.
     */
	void MoveMs(int32_t left, int32_t right, uint32_t ms , uint32_t rampUp = DEFAULT_RAMPUP_TIME)
	{
		obj_pololu_cmd->cmd = CMD_MOVE_MS;
		obj_pololu_cmd->leftSpeed = left;
		obj_pololu_cmd->rightSpeed = right;
		obj_pololu_cmd->ms = ms;
		obj_pololu_cmd->rampUp = rampUp;
	};
	/** Move forward with a specified speed for some ms.
	 *
	 *	@warning Moves without ramping motors up!
	 *	@param[in] speed motor speeds as a raw value ranging from -255 to 255.
	 *	@param[in] ms ms which the forward movement should last.
	 */
	void Forward(uint8_t speed, uint32_t ms)
	{
		assert(120 < speed);
		obj_pololu_cmd->cmd = CMD_MOVE_MS;
		obj_pololu_cmd->leftSpeed = speed;
		obj_pololu_cmd->rightSpeed = speed;
		obj_pololu_cmd->ms = ms;
		obj_pololu_cmd->rampUp = 0;
	};
	/** Move forward and stop after a specified distance you.
	 *  @param[in] dist Distance which should be moved. If negative dist is specified the robot moves backward.
	 */
	void MoveDist(double dist)
	{
		//assert(0 > dist);
		//TODO: do I use this one any more?!
		obj_pololu_cmd->cmd = CMD_MOVE_DIST;
		obj_pololu_cmd->length = dist;
	};
	/** Stops the robot immediately.
	 * 	Uses the MoveMs command with zero speeds and one ms timeout.
	 *
	 */
	void StopMovement(void)
	{
		this->MoveMs(0,0,50,0);
	}
	/** Tell the pololu_rtdb_module to move the robot to a specific place.
	 * 	@warning Do not change the default speed!
	 *  @param[in] x 		x-coordinate of the goal position.
	 *  @param[in] y 		y-coordinate of the goal position.
	 *	@param[in] speed	Speed with which to move.
	 */
	void GotoXY(const double x, const double y, int32_t speed = 160, bool precise = true)
	{
		// TODO: speed ??
		//assert(160 != speed);
		obj_pololu_cmd->cmd = CMD_INT_GOTO_XY;
		obj_pololu_cmd->x = x;
		obj_pololu_cmd->y = y;
		obj_pololu_cmd->precise = precise;
		obj_pololu_cmd->leftSpeed = speed;
		obj_pololu_cmd->rightSpeed = speed;
	};
	/** Abort the gotoXY command.
	 *  @warning You have to do so before issuing new commands!
	 */
	void AbortGotoXY(void)
	{
		obj_pololu_cmd->cmd = CMD_INT_ABORT_GOTO_XY;
	};
	/** Issues the pololu_rtdb_module to (re)localize the robot.
	 *  This is useful if for example you have two robots with the same position...
	 */
	void LocalizeRobot(void)
	{
		obj_pololu_cmd->cmd = CMD_INT_INIT_POSITION;
	};
	/** Tell the pololu_rtdb_module to move a vector.
	 *  The vector is described by the absolute angle to turn to and the length of the line which the robot will move forward.
	 *
	 *	@warning Do not change the default speed!
	 *  @param[in] phi 		Absolute Angle to turn to.
	 *  @param[in] l		Length of the vector.
	 *	@param[in] speed	Speed with which to move.
	 */
	void MoveVect(const Angle phi, const double l, int32_t speed = 160)
	{
		//assert(160 != speed);
		obj_pololu_cmd->cmd = CMD_INT_MOVE_VECT;
		obj_pololu_cmd->phi = phi;
		obj_pololu_cmd->length = l;
		//speed parameter not used at the moment, implementation in rtdb module not complete
		obj_pololu_cmd->leftSpeed = abs(speed);
		obj_pololu_cmd->rightSpeed = abs(speed);
	};
	/** Abort the MoveVector command.
	 *  @warning You have to do so before issuing new commands!
	 */
	void AbortMoveVect(void)
	{
		obj_pololu_cmd->cmd = CMD_INT_ABORT_MOVE_VECT;
	};
	/** Clears all data in a RTDB Object */
	void ClearAll(void) {
		obj_pololu_cmd->degree=0;
		obj_pololu_cmd->leftSpeed=0;
		obj_pololu_cmd->rightSpeed=0;
	}
	// Writing access to all command parameters.
	void SetRfcommNr(uint8_t rfcommNr)		{ obj_pololu_cmd->rfcommNr = rfcommNr;};
	void SetCmd(eCmd cmd)					{ obj_pololu_cmd->cmd = cmd;};
	void SetDirection(eKick direction) 		{ obj_pololu_cmd->direction = direction;};
	void SetForce(uint8_t force) 			{ obj_pololu_cmd->force = force;};
	void SetAngle(Angle angle) 				{ obj_pololu_cmd->phi = angle;};
	void SetGetMs(uint32_t msToGo) 			{ obj_pololu_cmd->ms = msToGo;};
	void SetGetRampUp(uint32_t rampUp) 		{ obj_pololu_cmd->rampUp = rampUp;};
	void SetSpeeds(int32_t left, int32_t right)
	{
		obj_pololu_cmd->leftSpeed = left;
		obj_pololu_cmd->rightSpeed = right;
	};

	// reading access to all command parameters
	uint8_t GetRfcommNr(void)	{ return obj_pololu_cmd->rfcommNr;};
	eCmd GetCmd(void)			{ return obj_pololu_cmd->cmd;};
	eKick GetDirection(void)	{ return obj_pololu_cmd->direction;};
	uint8_t GetForce(void)		{ return obj_pololu_cmd->force;};
	Angle GetAngle(void) 		{ return obj_pololu_cmd->degree;};
	int32_t GetSpeedLeft(void)	{ return obj_pololu_cmd->leftSpeed;};
	int32_t GetSpeedRight(void)	{ return obj_pololu_cmd->rightSpeed;};
	uint32_t GetMs(void)		{ return obj_pololu_cmd->ms;};
	uint32_t GetRampUp(void)	{ return obj_pololu_cmd->rampUp;};
	double GetX(void)			{ return obj_pololu_cmd->x;};
	double GetY(void)			{ return obj_pololu_cmd->y;};
	Angle GetPhi(void)			{ return obj_pololu_cmd->phi;};
	double GetLength(void)		{ return obj_pololu_cmd->length;};
	Position GetPos(void) 		{ return Position(obj_pololu_cmd->x,obj_pololu_cmd->y);};
	bool GetPrecise(void) 		{ return obj_pololu_cmd->precise;};

private:
	Timestamp mLastCmdTimestamp;
};



#endif /* POLOLU_CMD_H_ */
