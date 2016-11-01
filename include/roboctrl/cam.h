#ifndef CAM_H_
#define CAM_H_

#include <sstream> // stringstream
#include <stdexcept> // runtime_error
#include <cstring> // strings
#include <cstdio> // printf

#include "angle.h"
#include "position.h"

#include "share.h"
#include "types.h"

//kogmo_rtdb includes
#include "kogmo_rtdb_obj_base_classes.hxx"
#include "kogmo_rtdb_obj_typeids.h"
#include "kogmo_rtdb_obj_base.h"
#include "kogmo_rtdb_obj_a2.h"

// defines for the Cam
#define MIN_BALLS 0
#define MAX_BALLS 100
#define MIN_TEAMSIZE 0
#define MAX_TEAMSIZE 11
#define NUM_TEAMS 2
#define INVALID_DATA 5000

using namespace KogniMobil;

/** Struct for the redteam position information in the RTDB.*/
typedef struct {
    KogniMobil::kogmo_rtdb_subobj_base_t base;
    kogmo_rtdb_subobj_rfb_team_t team;
} kogmo_rtdb_obj_rfb_redteam_t;

/** Struct for the blueteam position information in the RTDB.*/
typedef struct {
    KogniMobil::kogmo_rtdb_subobj_base_t base;
    kogmo_rtdb_subobj_rfb_team_t team;
} kogmo_rtdb_obj_rfb_blueteam_t;

/** Struct for the ball information in the RTDB.*/
typedef struct {
    KogniMobil::kogmo_rtdb_subobj_base_t base;
    kogmo_rtdb_subobj_rfb_ball_t ball;
} kogmo_rtdb_obj_rfb_ball_t;

/** Class for error handling within the Cam class.
 *	Only used in the Cam class...
 */
class GetData {
public:
    static const int SUCCESS = 0;
    static const int NO_CONNECTION = 1;
    static const int TIMED_OUT = 2;
    static const int READ_TABLE_FAILED = 3;
    static const int READ_ENTRY_FAILED = 4;
    static const int TOO_FEW_OBJECTS = 5;
    static const int TOO_MANY_OBJECTS = 6;
};

/** Struct which stores all information of the ball. */
struct BallInfo {
    /** x-coordinate of the ball. [m]*/
    double pos_x;
    /** y-coordinate of the ball. [m]*/
    double pos_y;
    /** position as a Position object.*/
    Position pos;
    /** Absolute angle of the movement of the ball as an Angle object. */
    Angle phi;
    /** Velocity of the ball. [m/s]*/
    double velocity;
    /** Timestamp of the BallInfo.*/
    Timestamp timestamp;
};

/** Struct which stores the positions of a team.
 *  Note that in the pos_x, pos_y, pos and phi arrays the order of the
 *  robots found on the playground is random. But the index of a single robot in the arrays
 *  is always the same.
 */
struct TeamInfo
{
    /** The number of robots of that team on the playground.*/
    int members;
    /** Unsorted x-positions of the robots. [m]*/
    double pos_x[MAX_TEAMSIZE];
    /** Unsorted y-positions of the robots. [m]*/
    double pos_y[MAX_TEAMSIZE];
    /** Unsorted positions of the robots as a Position object.*/
    Position pos[MAX_TEAMSIZE];
    /** Unsorted absolute orientation of the robots as an Angle object. */
    Angle phi[MAX_TEAMSIZE];
    /** Timestamp of the TeamInfo.*/
    Timestamp timestamp;
};

/** Struct which stores the positions of the red team.*/
struct RedTeamInfo : TeamInfo{};

/** Struct which stores the positions of the blue team.*/
struct BlueTeamInfo : TeamInfo{};

/** Class for reading all positions extracted by the soccerlab_vision process
 * The Cam class gives you access to all information about robot-positions and orientations
 * as well as access to all information about the balls position, moving direction and speed.
 * The robot-positions and orientations are stored in a random order.
 * Note that the position of a robot is tracked by the corresponding pololu_rtdb_module
 * and can be accessed via the status of the robot.
 * Thus this class is mainly of interest if you want to get all robot positions to for
 * example figure out a specific constellation of the robots on the playground.
 * And the information of the ball can be found in the RawBall class which uses the Cam
 * class to get the information about the ball. So it is recommended to use the RawBall class
 * if you want to get information about the ball.
 *
 */
class Cam {
public:
    /** Initiate connection to the information provided by the cam.*/
    Cam(class RTDBConn &DBC);
    ~Cam();

    /** Gets the information about the ball from the RTDB. */
    int getBallPosition(BallInfo &ballInfo);
    /** Gets the information about the red team from the RTDB.
     *  @param[in] teamInfo 	RedTeamInfo struct in which the information of the red team should be stored.
     */
    int getPositions(RedTeamInfo &teamInfo);
    /** Gets the information about the blue team from the RTDB.
     *  @param[in] teamInfo 	BlueTeamInfo struct in which the information of the blue team should be stored.
     */
    int getPositions(BlueTeamInfo &teamInfo);
    /** Gets the information about the specified team from the RTDB.
     *  @param[in] teamInfo 	TeamInfo struct in which the information should be stored.
     * 	@param[in] team 		specifies the Team of which the data should be read out of the RTDB.
     */
    int getPositions(TeamInfo& teamInfo, eTeam team);

    // helper functions
    // TODO needed?
    static std::string toString(long value) {
        std::stringstream out;
        out << value;
        return out.str();
    }
    static std::string toString(double value) {
        std::stringstream out;
        out << value;
        return out.str();
    }
    static void throwOnErr(long value, const char* message) {
        if (value < 0) throw std::runtime_error(message + toString(value));
    }

private:
    char* ball_name;
    char* redteam_name;
    char* blueteam_name;

    // rtdb connection variables
    kogmo_rtdb_handle_t *dbc;
    kogmo_rtdb_connect_info_t dbinfo;
    kogmo_rtdb_objid_t oid;
    kogmo_rtdb_objsize_t olen;
    // rfb objects
    kogmo_rtdb_obj_info_t ballobj_info;
    kogmo_rtdb_obj_info_t redteamobj_info;
    kogmo_rtdb_obj_info_t blueteamobj_info;

};


//TODO return type auch in Enum umwandeln ?
#endif /*CAM_H_*/
