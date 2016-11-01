#ifndef KOGMO_RTDB_OBJ_RFB_H
#define KOGMO_RTDB_OBJ_RFB_H

#define KOGMO_RTDB_DONT_INCLUDE_ALL_OBJECTS
#include "kogmo_rtdb_obj_a2.h"

#ifdef __cplusplus
using namespace KogniMobil;
#endif

#define MIN_BALLS 0
#define MAX_BALLS 100
#define MIN_TEAMSIZE 0
#define MAX_TEAMSIZE 11
#define NUM_TEAMS 2
#define INVALID_DATA 5000

enum kogmo_rtdb_objtype_rfb
{
  KOGMO_RTDB_OBJTYPE_RFB_REDTEAM                    = 0xFB0007,
  KOGMO_RTDB_OBJTYPE_RFB_BLUETEAM                   = 0xFBC001,
  KOGMO_RTDB_OBJTYPE_RFB_BALL                       = 0xFBBA11,
};



typedef struct
{
    int members;
    double pos_x[MAX_TEAMSIZE];
    double pos_y[MAX_TEAMSIZE];
    double phi[MAX_TEAMSIZE];
} kogmo_rtdb_subobj_rfb_team_t;

typedef struct
{
    int found;
    double pos_x;
    double pos_y;
    double phi;
    double velocity;
} kogmo_rtdb_subobj_rfb_ball_t;

typedef struct
{
    kogmo_rtdb_subobj_base_t base;
    kogmo_rtdb_subobj_rfb_team_t team;
} kogmo_rtdb_obj_rfb_redteam_t;

typedef struct
{
    kogmo_rtdb_subobj_base_t base;
    kogmo_rtdb_subobj_rfb_team_t team;
} kogmo_rtdb_obj_rfb_blueteam_t;

typedef struct
{
    kogmo_rtdb_subobj_base_t base;
    kogmo_rtdb_subobj_rfb_ball_t ball;
} kogmo_rtdb_obj_rfb_ball_t;


#endif /* KOGMO_RTDB_OBJ_RFB_H */