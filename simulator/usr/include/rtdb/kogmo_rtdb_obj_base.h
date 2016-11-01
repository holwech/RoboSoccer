/*! \file kogmo_rtdb_obj_base.h
 * \brief Basis Object (C-Interface) and Essential RTDB-Objects.
 *
 * Copyright (c) 2003-2006 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 */
#ifndef KOGMO_RTDB_OBJ_BASE_H
#define KOGMO_RTDB_OBJ_BASE_H

#ifndef KOGMO_RTDB_H
	#ifndef KOGMO_RTDB_DONT_INCLUDE_ALL_OBJECTS
	#define KOGMO_RTDB_DONT_INCLUDE_ALL_OBJECTS //automatically don't include all objects, if an object file was included manually
	#endif
#endif

#include "kogmo_rtdb.h"
#include "kogmo_rtdb_obj_packing.h"

#ifdef __cplusplus
 extern "C" {
 namespace KogniMobil {
#endif

/*! \addtogroup kogmo_rtdb_objects Data Objects (Basis Object and Derivates)
 * \brief These Structs define the Data-Block of every Object.
 * The Data-Block ist the Part of the Object that normally
 * changes every cycle by issuing a commit.
 *
 * Every Object must start with a kogmo_rtdb_subobj_base_t.
 */
/*@{*/


/*! \brief Basis Object that every Data Object has to start with (has to inherit);
 * This is the Part of the Object that holds the basis data.
 * This Struct must be the Beginn of the Data-Block of every Object.
 *
 * Fields are marked as follows:
 *  - (U) Fields that can be filled by the user
 *  - (D) Fields that will be managed by the database and will be overwritten
 *
 * See kogmo_rtdb_obj_box_t for an example
 */

typedef PACKED_struct
{
  kogmo_timestamp_t	committed_ts;
    //!< (D) Time at which this Data has been committed
    //!< (set automatically on commit)
    // RTDB-internal usage:
    //  - 0 means, that this slot is empty or in preparation
    //  - if committed_ts changes, the slot has been overwritten
    //  - at the beginn of a commit committed_ts is set to 0,
    //    at the end committed_ts is set with the new value
    //    => as long as committed_ts is constant, the data is valid
  kogmo_rtdb_objid_t	committed_proc;
    //!< (D) Process-ID of the Process that commiting the data
    //!< (set automatically on commit)
  kogmo_timestamp_t	data_ts;
    //!< (U) Time at which this Data has been created;
    //!< will be filled with current Time if 0
  kogmo_rtdb_objsize_t	size;
    //!< (U) Size of the whole Data-Object, at least sizeof(kogmo_rtdb_subobj_base_t),
    //!< at maximum the size specified in the object-metadata;
    //!< This is necessary for variable-sized objects, e.g. containing
    //!< xml-data with knowledge descriptions
} kogmo_rtdb_subobj_base_t;

/*! \brief Full Object with only the basis data
 */
typedef PACKED_struct
{
  kogmo_rtdb_subobj_base_t base;
} kogmo_rtdb_obj_base_t;



/*! \brief An Objects that holds Information about the RTDB.
 * This is an essential object and is also used internally by the rtdb.
 */
typedef PACKED_struct
{
 kogmo_timestamp_t    started_ts;
 char                 dbhost[KOGMO_RTDB_OBJMETA_NAME_MAXLEN];
 uint32_t             version_rev; // 123
 char                 version_date[30]; // 2006-09-19 11:23:51 +01:00:00
 char                 version_id[402];
 uint32_t             objects_max;
 uint32_t             objects_free;
 uint32_t             processes_max;
 uint32_t             processes_free;
 kogmo_rtdb_objsize_t memory_max;
 kogmo_rtdb_objsize_t memory_free;
} kogmo_rtdb_subobj_c3_rtdb_t;

/*! \brief Full Object for RTDB Info
 */
typedef PACKED_struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_rtdb_t rtdb;
} kogmo_rtdb_obj_c3_rtdb_t;




typedef enum
{
  KOGMO_RTDB_PROCSTATUS_UNKNOWN   = 0,
  //!< Default beim Start, bedeutet dass der Prozess noch keinen Status selbst gesetzt hat
  KOGMO_RTDB_PROCSTATUS_CYCLEDONE, // == "KOGMO_RTDB_PROCSTATUS_OK"
  //!< Prozess hat gerade einen Zyklus beendet, und alle relevanten Objekte geschrieben.
  //!< Hierauf koennen andere Prozesse triggern.
  KOGMO_RTDB_PROCSTATUS_BUSY,
  //!< Prozess rechnet
  KOGMO_RTDB_PROCSTATUS_WAITING,
  //!< Prozess wartet (auf Objekt, I/O, Benutzer, etc.)
  KOGMO_RTDB_PROCSTATUS_FAILURE,
  //!< Prozess hat einen schweren Fehler selbst festgestellt, haengt und kann nicht weiterrechnen.
  //!< Koennte man vor dem exit() aufrufen.
  KOGMO_RTDB_PROCSTATUS_WARNING,
  //!< Prozess hat eine Warnung
  //
  // hier koennte man weitere relevante Zustaende ergaenzen (bitte erst fragen)
  // man kann auch einen Zusatz zu seinem Status als ASCII-Text setzen (->kogmo_rtdb_setstatus())
} kogmo_rtdb_obj_c3_process_status_t;

/*! \brief This object contains information about a process connected to the rtdb.
 * This is an essential object and is also used internally by the rtdb.
 */
typedef PACKED_struct {
 kogmo_rtdb_objid_t proc_oid;
   //!< Process-ID that is unique during runtime on the local system;
   //!< TODO: inter-Vehicles - Communication between Processes;
   //!< Processes have their own ID-space (so far)!
   //!< (this data is from ipc-connection-management
   //!< and will be partly copied into the object metadata)
 uint32_t           pid;
 uint32_t           tid;
 uint32_t           flags;
 uint32_t           status;
#define KOGMO_RTDB_PROCSTATUS_MSG_MAXLEN 160
 char               statusmsg[KOGMO_RTDB_PROCSTATUS_MSG_MAXLEN];
 uint32_t           version;
 uint32_t           uid;
 // kogmo_rtdb_objid_t wait_oid;
} kogmo_rtdb_subobj_c3_process_t;


/*! \brief Full Object for Process Info.
 */
typedef PACKED_struct
{
  kogmo_rtdb_subobj_base_t base;
  kogmo_rtdb_subobj_c3_process_t process;
} kogmo_rtdb_obj_c3_process_t;




/*@}*/

#ifdef __cplusplus
 }; /* namespace KogniMobil */
 }; /* extern "C" */
#endif

#endif /* KOGMO_RTDB_OBJ_BASE_H */
