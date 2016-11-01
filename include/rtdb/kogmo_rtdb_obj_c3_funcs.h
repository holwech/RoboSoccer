/*! \file kogmo_rtdb_obj_c3_funcs.h
 * \brief Functions for C3 Objects (C-Interface).
 *
 * Copyright (c) 2003-2006 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 */

#ifndef KOGMO_RTDB_H
# error "Never use <objects/*.h> directly; include <kogmo_rtdb.h> instead."
#endif

#ifdef __cplusplus
 extern "C" {
 namespace KogniMobil {
#endif

/*! \addtogroup kogmo_rtdb_objects */
/*@{*/


/*! \brief Dump the Position/Orientation data of an SixDoF-Object into a string
 *
 * \param db_h        database handle
 * \param data_p      Pointer to a Object-Data-Struct
 * \returns           Pointer to a string that will contain the dump in ASCII;
 *                    YOU have to FREE it after usage!\n
 *                    NULL on errors
 */
char*
kogmo_rtdb_obj_c3_sixdof_dump_str (kogmo_rtdb_handle_t *db_h,
                                   void *data_p);


// Functions for Process-Object (internal):

inline kogmo_rtdb_objid_t
kogmo_rtdb_obj_c3_process_searchprocessobj (kogmo_rtdb_handle_t *db_h,
                                            kogmo_timestamp_t ts,
                                            kogmo_rtdb_objid_t proc_oid);

/*! \brief This gives more information about a process into a
 * struct kogmo_rtdb_obj_c3_process_info_t
 *
 * \returns       <0 on errors, on errors the string is set to "?".
 */
inline int
kogmo_rtdb_obj_c3_process_getprocessinfo (kogmo_rtdb_handle_t *db_h,
                                          kogmo_rtdb_objid_t proc_oid,
                                          kogmo_timestamp_t ts,
                                          kogmo_rtdb_obj_c3_process_info_t str);


/*@}*/

#ifdef __cplusplus
 }; /* namespace KogniMobil */
 }; /* extern "C" */
#endif
