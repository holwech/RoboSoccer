#ifndef ROBO_CONTROL_CPP_
#define ROBO_CONTROL_CPP_

//#include "pololu_high_level_control.h"

#include "pololu_status.h"
#include "pololu_cmd.h"
#include "cam.h"
#include "raw_ball.h"
#include "angle.h"
#include "position.h"
#include "share.h"
#include "types.h"
#include "vector3d.h"

/* Enum for the status of the path planning algorithm. */
enum ePathMovingStatus {
    /** Path planning still running. Destination not yet reached. */
    ON_THE_WAY,
    /** Path planning finished. Destination reached. */
    REACHED_DESTINATION,
    /** Path destination blocked by another robot. -> Robot halted.*/
    PATH_BLOCKED_BY_ROBOT,
    /** Path destination out of playground. -> Robot halted.*/
    PATH_OUT_OF_PLAYGROUND
};

enum ePathAngleStatus {
    ANGLE_VALID,
    DESTINATION_BLOCKED_BY_ROBOT,
    DESTINATION_OUT_OF_PLAYGROUND,
    DESTINATION_EQUALS_OWN_POSITION
};

enum eBlockedState {
    NO_BLOCKING,
    EVASION_POSSIBLE,
    BLOCKED
};

/* Struct for the path-finding algorithm to store the blocked angles. */
typedef struct {
    /** Leftmost Angle in the triplet. */
    Angle left;
    /** Middle Angle in the triplet. */
    Angle middle;
    /** Rightmost Angle in the triplet. */
    Angle right;
    /** Position of the robot which belongs to those blocked angles. */
    Position roboPos;
} angles;

/* Struct to store blocking obstacles */
typedef struct {
    Position obstaclePos;
    Position pos1;
    Position pos2;
} Obstacles;

typedef Vector3d<fType> Vector;

/** @brief Class for the basic control over the robots.
 * The RoboControl class gives you access to all functionality of the robot on the playground.
 * This means on the one hand you can get all status information about the robot which is controlled
 * by the specific instance of the class. On the other hand you can issue all possible commands via
 * this class. The  class uses the PololuCmd and the PololuStatus to communicate with the
 * pololu_rtdb_module (and thus the robot) via the RTDB. It is not recommended to use the PololuCmd
 * or the PololuStatus classes directly!
 */
class RoboControl {

public:
        /** @brief Constructor gets first robot positions. */
	RoboControl(RTDBConn& DBC, const int deviceNr);
	~RoboControl();

        /** @brief Kick methode.
         * @param[in] force         Force of the kick: SOFT MEDIUM HARD.
         * @param[in] distToBall    Distance which the Robot will move forward in order to kick
         *                          If the default = 0.0 is specified the methode will measure
         *                          the distance to the ball and move this distance.
         *                          Distances bigger then 25cm are not allowed.
         * @return true if kick has been executed and false if not.
	 */
	bool Kick(eForce force, double distToBall = 0.0);

        /** @brief Kick methode.
         * @param[in] force         Force of the kick: integer value between 0 and 100 where 0 is
         *                          equivalent to SOFT and 100 equivalent to HARD.
         * @param[in] distToBall    Distance which the Robot will move forward in order to kick
         *                          If the default = 0.0 is specified the method will measure
         *                          the distance to the ball and move this distance.
         *                          Distances bigger then 25cm are not allowed.
         * @return true if kick has been executed and false if not.
	 */
	bool Kick(uint32_t force, double distToBall = 0.0);

        /** @brief Get the mac address of the robot.
         *  @param[out] mac         Mac as an array of six unsigned characters.
	 */
        void GetMac(uint8_t (&mac)[6]) const {
            mpPololuStatus->RTDBRead();
            mpPololuStatus->GetMac(mac);
        }

        /** @brief Get the values of the distance sensors.
         *  @return TODO: how is that data stored?
         */
        uint8_t GetDistanceSensors(void) const {
            mpPololuStatus->RTDBRead();
            return mpPololuStatus->GetDistanceSensors();
        }

        /** @brief Get the accuvoltage of the robot.
         *  @return Accu voltage in mV.
	 */
        uint32_t GetAccuVoltage(void) const;

        /** @brief Get the moving status of the robot.
         *  Moving status is either moving or turning or standing. Be careful this information is
         *  updated AFTER the robot really is in this moving state. This can lead to problems when
         *  you issue for example a turning command because the confimration of the robot might take
         *  some ms!
         *  @return Movingstatus in form of an enum eMovingStatus.
	 */
        eMovingStatus GetMovingStatus(void) const {
            mpPololuStatus->RTDBRead();
            return mpPololuStatus->GetMovingStatus();
        }

        /** @brief Get the status of the localization procedure.
         *  Be careful, that the pololu_rtdb_module might think that it is located properly but in
         *  fact it is not. The information STATUS_INT_LOCALIZED does only mean, that the
         *  localization is finished but not that the robot was localized correctly (how would he
         *  know?!?) however a wrong localization should be very rare... The only error which can
         *  occur here is that the localization of robots in a team could be interchanged or that
         *  they are equal.
         *  @return Localization status in form of an enum eLocalizationStatus.
	 */
        eLocalizationStatus GetLocalizationStatus(void) const {
            mpPololuStatus->RTDBRead();
            return mpPololuStatus->GetLocalizationStatus();
        }

        /** @brief Get the module status (Status of gotoXY, MoveVector).
         *  This returns the status of either the gotoXY command or the MoveVector command.
         *  This is helpful to determine if one of these commands is already finished.
         *  @return Modul status (pololu_rtdb_modul) in form of an enum eModuleStatus.
	 */
        eModuleStatus GetModuleStatus(void) const {
            mpPololuStatus->RTDBRead();
            return mpPololuStatus->GetModuleStatus();
        }

        /** @brief Get the speed of the left motor of the robot.
         *  @return TODO
	 */
        int32_t GetSpeedLeft(void) const {
		mpPololuStatus->RTDBRead();
		return mpPololuStatus->GetSpeedLeft();
        }

        /** @brief Get the speed of the right motor of the robot.
         *  @return TODO
	 */
        int32_t GetSpeedRight(void) const {
		mpPololuStatus->RTDBRead();
		return mpPololuStatus->GetSpeedRight();
        }

        /** @brief Get the ms to move from the status of the robot.
         *  @return TODO
	 */
        uint32_t GetMsToGo(void) const {
		mpPololuStatus->RTDBRead();
		return mpPololuStatus->GetMsToGo();
        }

        /** @brief Get the x-coordinate of the robot.
         *  @return TODO
	 */
        double GetX(void) const {
		mpPololuStatus->RTDBRead();
		return mpPololuStatus->GetX();
        }

        /** @brief Get the y-coordinate of the robot.
         *  @return TODO
	 */
        double GetY(void) const {
		mpPololuStatus->RTDBRead();
		return mpPololuStatus->GetY();
        }

        /** @brief Get the absolute orientation of the robot.
         *  @return TODO
         */
        Angle GetPhi(void) const {
		mpPololuStatus->RTDBRead();
		return mpPololuStatus->GetPhi();
        }

        /** @brief Gets the cartesian coordinates of the robot.
         *  @return TODO
         */
        Position GetPos(void) const {
		mpPololuStatus->RTDBRead();
		return Position(mpPololuStatus->GetX(),mpPololuStatus->GetY());
        }

        /** @brief Gets the Rfcomm Number which the robot is connected to.
         *  @return TODO
         */
        int GetRfcommNr(void) const {
            return mRfcommNr;
        }

        /** @brief Set the motor speeds to specific values with a timeout.
         *  A rampuptime can be specified. This is recommended when you want to drive faster than
         *  120 otherwise the motors might slip... The default rampuptime is 200ms.
         *  @param[in] left         left motor speed as a raw value ranging from -255 to 255.
         *  @param[in] right        right motor speed as a raw value ranging from -255 to 255.
         *  @param[in] ms           ms which the movement should last.
         *  @param[in] rampUp       Ramp up time. The motors will get faster in a linear way during
         *                          this period of time.
         */
        void MoveMs(const int32_t left, const int32_t right, const uint32_t ms, const uint32_t rampUp = DEFAULT_RAMPUP_TIME) {
		//assert(ms < 6000);
		mpPololuCmd->MoveMs(left, right, ms ,rampUp);
		mpPololuCmd->setDataTimestamp();
		mpPololuCmd->RTDBWrite();
        }

        /** @brief Same as MoveMs with the difference that the program execution will be blocked.
         * Blocked until either the robot will confirm that the movement has stopped or a timeout
         * has expired (timeout is calculated to be 400ms more than the anticipated movement time).
	 */
        void MoveMsBlocking(const int32_t left, const int32_t right, const uint32_t ms, const uint32_t rampUp = DEFAULT_RAMPUP_TIME);

        /** @brief Turn a specified angle.
         *  Clock-wise is positive and counter-clock-wise is negative.
         *  @param[in] angle        Angle which should be rotated as an Angle object. To turn via a
         *                          given value in degree or double just use Angle(degree/double).
	 */
        void Turn(const Angle& angle) {
		mpPololuCmd->ClearAll();
		mpPololuCmd->Turn(angle);
		mpPololuCmd->setDataTimestamp();
		mpPololuCmd->RTDBWrite();
        }

        /** @brief Turn to an absolute angle.
         * @param[in] angle         Absolute angle to turn to.
	 */
        void TurnAbs(const Angle& angle) {
		mpPololuStatus->RTDBRead();
                //const double l = 0.5;
		//Position target = Position(cos(angle)*l+this->mpPololuStatus->GetX(),sin(angle)*l+this->mpPololuStatus->GetY());
		//Angle winkel = mpPololuStatus->GetPos().AngleOfLineToPos(target) - mpPololuStatus->GetPhi();
		//cout << "berechnetes target: "<< target << " aktueller winkel "<< this->mpPololuStatus->GetPhi() << " Ziel " << angle << " ��nderung: " << winkel << endl;
                int aktwinkel = this->GetPhi().Deg(); // TODO this ?
		int sollwinkel = angle.Deg();
                aktwinkel = (aktwinkel + 4 * 180) % (2 * 180);
                sollwinkel = (sollwinkel + 4 * 180) % (2 * 180);
                Angle winkel = sollwinkel - aktwinkel;
		//cout << "gew��nscht: " << angle << " aktuell: " << this->GetPhi() << " zu drehen: " << winkel << endl;
                this->Turn(winkel); // TODO this ?
        }

        /** @brief Tell the pololu_rtdb_module to move the robot to a specific place.
         *  Notice: If precise flag is set ramp down at the end of the route is on.
         *  @warning Do not change the default speed!
         *  @param[in] x            x-coordinate of the goal position.
         *  @param[in] y            y-coordinate of the goal position.
         *  @param[in] speed        Speed with which to move.
         *  @param[in] precise      Toggles if the robot runs precise at the end of a route.
	 */
        void GotoXY(const double x, const double y, int32_t speed = 160, const bool precise = true) {
                //TODO: speed ??
		//cout << "RoboControl: Received GotoXY to " << x << " / " << y << endl;
                //if ((mpPololuCmd->GetPos().DistanceTo(Position(x,y)) > 0.01)||(mpPololuCmd->getDataTimestamp().diffs(Timestamp::getNow())>1.0)) {
                    mpPololuCmd->ClearAll();
                    mpPololuCmd->GotoXY(x,y,speed, precise);
                    mpPololuCmd->setDataTimestamp();
                    mpPololuCmd->RTDBWrite();
		//}
		//else {
                //  cout << "Anfrage blockiert" << endl;
		//}
        }

        /** @brief Tell the pololu_rtdb_module to move the robot to a specific place.
         *  Uses GotoXY internally with speed 80 and precise mode on.
         *  @param[in] dest         Destination in form of a Position object.
	 */
        void GotoPos(const Position dest) {
                this->GotoXY(dest.GetX(),dest.GetY(),80);   // TODO this ?
	}

        /** @brief Test whether a robot is moving or not.
         *  @return Returns if the robot is moving (true) or not (false). Returns true when the
         *          robot rescived a movement command and is execuing it. When the command was not
         *          yet received by the robot or has been executed completely false is returned.
         */
        bool IsMoving(void) const;

        /** @brief Stops any movement.
         */
        void StopAction(void) {
            mpPololuCmd->ClearAll();
            mpPololuCmd->AbortGotoXY();
            mpPololuCmd->setDataTimestamp();
            mpPololuCmd->RTDBWrite();
	}

        /** @brief Turn and block program execution.
         *  Same as Turn with the difference that the program execution will be blocked until either
         *  the robot will confirm that the rotation has stopped or a timeout has expired (timeout
         *  is calculated to be more than the anticipated movement time)
         *  @param[in] angle        Angle which should be rotated as an Angle object.
         */
        void TurnBlocking(const Angle& angle);

        /** @brief Move forward and stop after a specified distance.
         *  It is possible to define speed and precise value. It is not possible to drive backwards.
         *  @param[in] dist         Distance which should be moved.
         */
        inline void MoveDist(double dist, int32_t speed=160, bool precise=true) {
            this->GotoXY(cos(mpPololuStatus->GetPhi())*dist, sin(mpPololuStatus->GetPhi())*dist,speed,precise); // TODO this?
            // mpPololuCmd->MoveDist(dist);
            // mpPololuCmd->setDataTimestamp();
            // mpPololuCmd->RTDBWrite();
        }

        /** @brief Abort GotoXY command.
         *  Just to keep old software working.
         */
        void AbortGotoXY(void) {
                mpPololuCmd->AbortGotoXY();
                mpPololuCmd->setDataTimestamp();
                mpPololuCmd->RTDBWrite();
        }

        TeamInfo mOwnTeamInfo;
        TeamInfo mOtherTeamInfo;

        //	/** Issues the pololu_rtdb_module to (re)localize the robot.
        //	 *  This is useful if for example you have two robots with the same position...
        //	 *
        //	 *  @return true if robot is now localized and false if not.
        //	 */
        //	bool Localize(void) {
        //		mpPololuCmd->LocalizeRobot();
        //		mpPololuCmd->setDataTimestamp();
        //		mpPololuCmd->RTDBWrite();
        //		while(this->GetLocalizationStatus() != STATUS_INT_LOCATING);
        //		while(this->GetLocalizationStatus() == STATUS_INT_LOCATING);
        //
        //		if(this->GetLocalizationStatus() == STATUS_INT_LOCALIZED)
        //		{
        //			return true;
        //		} else {
        //			return false;
        //		}
        //	};

        ///** Path planning algorithm to move to a certain destination (dest) which avoids other players.
        // * This method has to be called in a loop in order to work. It does not block the program execution.
        // *
        // * @return ePathMovingStatus: Gives you a clue on what the satus of the path planning algorithm is like.
        // * @param[in] pathLength The maximum length of the single straight path which in their sum make the complex path to the destination.
        // * @param[in] dest The destination which the robot should be moved to.
        // */
        //ePathMovingStatus AdjustPath(const Position &dest, const double pathLength = 1.0);

protected:
//	bool pathBlocked(Position start, Position end, Position obstac, Position& pos1, Position& pos2const, double distanceBetweenObstacleAndEvasivePosition);
//	bool pathBlocked(Position start, Position end);
//	eBlockedState evasivePath(Position& tmpDest, Position& evasivePos);
protected:
        RawBall mBall;

        Timestamp mLastOwnTeamTimestamp;
        Timestamp mLastOtherTeamTimestamp;

        eTeam mOwnTeam;
        eTeam mOtherTeam;

private:
        /** @brief Tell the pololu_rtdb_module to move a vector.
	 *  The vector is described by the absolute angle to turn to and the length of the line which the robot will move forward.
	 *
         *  @warning Do not change the default speed!
         *  @param[in] phi Absolute Angle to turn to.
         *  @param[in] l Length of the vector.
	 */
        void MoveVect(const Angle& phi, const double l,const uint speed =160) {
		//mpPololuCmd->MoveVect(phi,l,speed);
		cout << "RoboControl: Received MoveVect to " << phi << " length: " << l << endl;
		this->GotoXY(cos(phi)*l+this->mpPololuStatus->GetX(),sin(phi)*l+this->mpPololuStatus->GetY(),speed);
		//mpPololuCmd->setDataTimestamp();
		//mpPololuCmd->RTDBWrite();
        }

        /** Block the program execution until any movement which has to be started before
	 *  calling this method is finished.
	 *
	 *  @param[in] maxWaitingTimeGetMs Maximum time to wait for a MovementStopped information from the robot.
	 */
	void WaitTillMovementStopped(int64_t maxWaitingTimeGetMs = 0);

        /** Abort the MoveVector command.
	 *  @warning You have to do so before issuing new commands after a MoveVector command!!
	 */
        void AbortMoveVect(void) {
		mpPololuCmd->AbortMoveVect();
		mpPololuCmd->setDataTimestamp();
		mpPololuCmd->RTDBWrite();
        }

	/** Initialize pololu_status and pololu_cmd in the RTDB.*/
	void Init(void);

        // Private variables
        /** Radius of the circle around the destination of the path planning
         * algorithm in which the robot has reached the destination.
         */
        static const double PATH_PLANNING_MAX_POSITIONING_ERROR	= 0.025;
        /** Maximum angle error while moving to the destination.
         *  When this error is exceeded the absolute orientation is corrected.
         */
        static const int32_t PATH_PLANNING_MAX_ANGLE_ERROR = 10;
        /** The radius which is assumed to be blocked around another robot.
         */
        static const double OBSTACLE_RADIUS = 0.12;
        /** Own diamter.
         */
        static const double ROBO_DIAMETER = 0.1;
        /** Own radius.
         */
        static const double ROBO_RADIUS = 0.05;

        bool mStartedMoving;
        int mMyNumber;
        Position mTmpDest;
        Position mCmdDest;
        Position mEvasivePos;
        bool mEvasion;

        int mRfcommNr;

        string mRtdbObjPololuCmdName;
        string mRtdbObjPololuStatusName;

        PololuCmd *mpPololuCmd;
        PololuStatus *mpPololuStatus;

public:
        /** Stops the robot immediately. links to StopAction. Just to keep old software working.
	 */
        //TODO: Change to private
        inline void StopMovement(void) {
            this->StopAction();
        }

};


#endif /* ROBO_CONTROL_CPP_ */
