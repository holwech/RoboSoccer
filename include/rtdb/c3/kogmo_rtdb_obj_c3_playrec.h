/*! \file kogmo_rtdb_obj_playrec_c3.h
 * \brief Recorder and Player Objects from C3
 *
 * Copyright (c) 2003-2006 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 */

#ifndef KOGMO_RTDB_OBJ_C3_PLAYREC_H
#define KOGMO_RTDB_OBJ_C3_PLAYREC_H

#include "kogmo_rtdb_obj_packing.h"
#include "kogmo_rtdb_obj_base.h"

#ifdef __cplusplus
 extern "C" {
 namespace KogniMobil {
#endif

/*! \addtogroup kogmo_rtdb_objects */
/*@{*/


/*! \brief Control Object for the RTDB-Player
 */
typedef PACKED_struct
{
  struct
  {
    uint32_t log : 1;
    uint32_t pause : 1;
    uint32_t loop : 1;
    uint32_t keepcreated : 1;
    uint32_t scan : 1;
  } flags;
  float speed;
  kogmo_timestamp_t begin_ts; // begin playing here after loop
  kogmo_timestamp_t end_ts;   // end playing here (and loop if desired)
  kogmo_timestamp_t goto_ts; // != 0 => seek to this position
  kogmo_rtdb_objname_t frame_objname;
  int16_t frame_go; // -+X > 0 ==> play until X objects with name "frame_objname" have passed
} kogmo_rtdb_subobj_c3_playerctrl_t;

typedef PACKED_struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_playerctrl_t playerctrl;
} kogmo_rtdb_obj_c3_playerctrl_t;


/*! \brief Status Object for the RTDB-Player
 */
typedef PACKED_struct
{
  kogmo_timestamp_t current_ts;
  kogmo_timestamp_t first_ts;
  kogmo_timestamp_t last_ts;
} kogmo_rtdb_subobj_c3_playerstat_t;

typedef PACKED_struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_playerstat_t playerstat;
} kogmo_rtdb_obj_c3_playerstat_t;




/*! \brief Status Object for the RTDB-Recorder
 * TEMP! will be extended without notice!
 */
typedef PACKED_struct
{
  kogmo_timestamp_t begin_ts;
  uint64_t bytes_written;
  uint32_t events_written;
  uint32_t events_total;
  uint32_t events_lost;
  int16_t event_buffree;
  uint16_t event_buflen;
} kogmo_rtdb_subobj_c3_recorderstat_t;

/*! \brief Full Object with RTDB-Recorder Status
 */
typedef PACKED_struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_recorderstat_t recorderstat;
} kogmo_rtdb_obj_c3_recorderstat_t;


/*! \brief Control Object for the RTDB-Recorder
 * ==> NOT IN USE YET!
 */
typedef PACKED_struct
{
#define C3_RECORDERCTRL_MAXLIST 50
  // include those objects
  kogmo_rtdb_objid_t     oid_list[C3_RECORDERCTRL_MAXLIST];
  kogmo_rtdb_objtype_t   tid_list[C3_RECORDERCTRL_MAXLIST];
  char                   name_list[C3_RECORDERCTRL_MAXLIST][KOGMO_RTDB_OBJMETA_NAME_MAXLEN];
  // exclude those objects
  kogmo_rtdb_objid_t     xoid_list[C3_RECORDERCTRL_MAXLIST];
  kogmo_rtdb_objtype_t   xtid_list[C3_RECORDERCTRL_MAXLIST];
  char                   xname_list[C3_RECORDERCTRL_MAXLIST][KOGMO_RTDB_OBJMETA_NAME_MAXLEN];
  // diese werden als "richtige" bilder ins avi geschrieben (kann waehrend der aufnahme nicht geaendert werden)
  kogmo_rtdb_objid_t     oid_stream[10];
  struct
  {
    uint32_t select_all : 1;
    uint32_t recorder_on : 1;
//    uint32_t record_pause : 1; // gibt's noch nicht. wird das file nicht schliessen
    uint32_t logging_on : 1;
  } switches;
} kogmo_rtdb_subobj_c3_recorderctrl_t;

/*! \brief Full Object with RTDB-Recorder Control
 */
typedef PACKED_struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_recorderctrl_t recorderctrl;
} kogmo_rtdb_obj_c3_recorderctrl_t;


/*@}*/

#ifdef __cplusplus
 }; /* namespace KogniMobil */
 }; /* extern "C" */
#endif

#endif /* KOGMO_RTDB_OBJ_C3_PLAYREC_H */
