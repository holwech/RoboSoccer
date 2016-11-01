#ifndef RAW_BALL_H_
#define RAW_BALL_H_

//kogmo_rtdb includes
#include "kogmo_rtdb_obj_base_classes.hxx"
#include "kogmo_rtdb_obj_typeids.h"
#include "kogmo_rtdb_obj_base.h"

#include "angle.h"
#include "position.h"
#include "cam.h"

#include "share.h"
#include "types.h"

/** Simple ball class which gives access to all ball informations stored in the RTDB.
 * The RawBall class gives a simple access to the ball informations which are extracted
 * by soccerlab_vision process and then stored in the RTDB.
 * The informations about the ball are:
 * - Position on the playground.
 * - velocity in m/s
 * - the angle of the movement on the playground (phi)
 */
class RawBall
{

private:

	Timestamp mLastBallTimestamp;

protected:
	/** Cam object which allows to retrieve the informations of the ball from the camera.*/
	Cam mCam;
	/** Struct for the storage of all the information of the ball.*/
	BallInfo mBall;

	/** Checks whether new valid data is present and if so updates mBall. */
	void UpdateBallInfoIfNecessary(void)
	{
		do{while(mCam.getBallPosition(mBall));}while((mBall.pos_x == 5000.0)||(mLastBallTimestamp >= mBall.timestamp));
		mLastBallTimestamp = mBall.timestamp;
	}

public:
	/** If the velocity of the ball is beneath this threshold the ball is assumed to stand still. */
	static const double BALL_MOVING_TRESHOLD = 0.0001; //0.00005;

	/** Gets the first ballInfo at construction */
	RawBall(RTDBConn &DBC) : mCam(DBC)
	{
		//nice(5);
	  std::cout << "lala" << std::endl;
	  while(mCam.getBallPosition(mBall)){std::cout << mCam.getBallPosition(mBall) << std::endl;}
	  std::cout << "lala" << std::endl;
		mLastBallTimestamp = mBall.timestamp;
	}

	/** Get the current x-coordinate of the ball.
	 * @return X-coordinate in m.
	 */
	double GetX(void) 		{this->UpdateBallInfoIfNecessary();return mBall.pos_x;}
	/** Get the current y-coordinate of the ball.
	 * @return Y-coordinate in m.
	 */
	double GetY(void) 		{this->UpdateBallInfoIfNecessary();return mBall.pos_y;}
	/** Get the current moving direction of the ball.
	 * @return Moving direction as an Angle object.
	 */
	Angle GetPhi(void) 		{this->UpdateBallInfoIfNecessary();return Angle(mBall.phi);}
	/** Get the current velocity of the ball.
	 * @return Velocity in m/s.
	 */
	double GetVelocity(void)	{this->UpdateBallInfoIfNecessary();return mBall.velocity;}
	/** Get the current position of the ball
	 * @return Position of the ball as a Position object.
	 */
	Position GetPos(void)
	{
		this->UpdateBallInfoIfNecessary();
		return Position(mBall.pos_x, mBall.pos_y);
	}

};

#endif /* RAW_BALL_H_ */
