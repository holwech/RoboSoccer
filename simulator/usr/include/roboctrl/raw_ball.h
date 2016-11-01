#ifndef RAW_BALL_H_
#define RAW_BALL_H_

//kogmo_rtdb includes
#include "kogmo_rtdb_obj_base_classes.hxx"
#include "kogmo_rtdb_obj_typeids.h"
#include "kogmo_rtdb_obj_base.h"
#include "kogmo_rtdb.hxx"

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
    kogmo_rtdb_subobj_rfb_ball_t    *m_objBall;
    RTDBObj                         *m_ballRTDBObj;



public:
	/** If the velocity of the ball is beneath this threshold the ball is assumed to stand still. */
	static const double BALL_MOVING_TRESHOLD = 0.0001; //0.00005;

	/** Gets the first ballInfo at construction */
    RawBall(RTDBConn &DBC)
	{
        m_objBall = new kogmo_rtdb_subobj_rfb_ball_t;
        kogmo_rtdb_objsize_t olen=sizeof(kogmo_rtdb_subobj_rfb_ball_t);
        m_ballRTDBObj = new RTDBObj(DBC,"a2_video_ball",0xFBBA11,olen,(char**) &m_objBall);
        m_ballRTDBObj->RTDBSearchWait("a2_video_ball");
        m_ballRTDBObj->RTDBRead();
        cout <<"ball connected"<<endl;
	}

	/** Get the current x-coordinate of the ball.
	 * @return X-coordinate in m.
	 */
    double GetX(void) 		{m_ballRTDBObj->RTDBRead(); return m_objBall->pos_x;}
	/** Get the current y-coordinate of the ball.
	 * @return Y-coordinate in m.
	 */
    double GetY(void) 		{m_ballRTDBObj->RTDBRead(); return m_objBall->pos_y;}
	/** Get the current moving direction of the ball.
	 * @return Moving direction as an Angle object.
	 */
    Angle GetPhi(void) 		{m_ballRTDBObj->RTDBRead(); return m_objBall->phi;}
	/** Get the current velocity of the ball.
	 * @return Velocity in m/s.
	 */
    double GetVelocity(void)	{m_ballRTDBObj->RTDBRead(); return m_objBall->velocity;}
	/** Get the current position of the ball
	 * @return Position of the ball as a Position object.
	 */
	Position GetPos(void)
	{
        m_ballRTDBObj->RTDBRead();
        return Position(m_objBall->pos_x, m_objBall->pos_y);
	}

};

#endif /* RAW_BALL_H_ */
