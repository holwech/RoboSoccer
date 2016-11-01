/*! \file kogmo_rtdb_obj_c3_misc.h
 * \brief Misc. Objects from C3
 *
 * Copyright (c) 2003-2006 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 */

#ifndef KOGMO_RTDB_OBJ_C3_MISC_H
#define KOGMO_RTDB_OBJ_C3_MISC_H

#include "kogmo_rtdb_obj_packing.h"
#include "kogmo_rtdb_obj_base.h"

#ifdef __cplusplus
 extern "C" {
 namespace KogniMobil {
#endif

/*! \addtogroup kogmo_rtdb_objects */
/*@{*/



/*! \brief Raw GPS Data, needs to be processed first to be usable for other projects
 * This is primarily for recording raw GPS data
 */
typedef PACKED_struct
{
  int32_t       valid;               // GPS-Data Validity:
    // 0=invalid(no fix), 1=2D(good for latitude/longitude), 2=3D(good for altitude too)
#define C3_GPSDATA_UTCSIZE 28
  char          utc[C3_GPSDATA_UTCSIZE]; // UTC date/time as "yyy-mm-ddThh:mm:ss.sssZ"
  double        latitude;            // Latitude in degrees
  double        longitude;           // Longitude in degrees
  double        altitude;            // Altitude in meters  
  double        speed;               // Speed over ground (knots? m/s?)
  double        track;               // Course made good (relative to true north)
  int32_t       nsat;                // Number of satellites used in last fix
  double        pdop;                // p-Dilution of precision
  double        hdop;                // h-Dilution of precision
  double        vdop;                // v-Dilution of precision
} kogmo_rtdb_subobj_c3_gpsdata_t;

/*! \brief Full Object with Raw GPS data
 */
typedef PACKED_struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_gpsdata_t gpsdata;
} kogmo_rtdb_obj_c3_gpsdata_t;








/*! \brief Type of a pre-allocated String to receive the Output of
 * kogmo_rtdb_obj_c3_process_getprocessinfo()
 */
typedef char kogmo_rtdb_obj_c3_process_info_t[KOGMO_RTDB_OBJMETA_NAME_MAXLEN+100];



/*! \brief Simulation-Time Control Object
 */
typedef PACKED_struct
{
 double delta_sim_time;
 int stop;
 int cycle;
 double runtime;
} kogmo_rtdb_subobj_c3_simtime_t;

typedef struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_simtime_t simtime;
} kogmo_rtdb_obj_c3_simtime_t;



/*! \brief Plain ASCII-Text Object
 */
typedef struct
{
 // String does not need to be null-terminated, but it can be.
 // DO NOT use the last character! The reader might use it
 // for its own null-termination.
#define C3_TEXT_MAXCHARS (1024*100)
 char data[C3_TEXT_MAXCHARS];
} kogmo_rtdb_subobj_c3_text_t;

typedef struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_text_t text;
} kogmo_rtdb_obj_c3_text_t;

/*! \brief Small (1k) Plain ASCII-Text Object
 * (can share the same type-id with kogmo_rtdb_obj_c3_text_t)
 */

typedef struct
{
 char data[1024];
} kogmo_rtdb_subobj_c3_text1k_t;

typedef struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_text1k_t text;
} kogmo_rtdb_obj_c3_text1k_t;

/*@}*/

#ifdef __cplusplus
 }; /* namespace KogniMobil */
 }; /* extern "C" */
#endif

#endif /* KOGMO_RTDB_OBJ_C3_MISC_H */
