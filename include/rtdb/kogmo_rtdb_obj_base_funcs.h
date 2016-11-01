/*! \file kogmo_rtdb_obj_base_funcs.h
 * \brief Functions for Basis Object (C-Interface).
 *
 * Copyright (c) 2003-2006 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 */

#ifndef KOGMO_RTDB_OBJ_BASE_FUNCS_H
#define KOGMO_RTDB_OBJ_BASE_FUNCS_H


#ifdef __cplusplus
 extern "C" {
 namespace KogniMobil {
#endif

/*! \addtogroup kogmo_rtdb_objects
 */
/*@{*/

/*! \brief Dump the Basisdata of an Object into a string
 *
 * \param db_h        database handle
 * \param data_p      Pointer to a Object-Data-Struct
 * \returns           Pointer to a string that will contain the dump in ASCII;
 *                    YOU have to FREE it after usage!\n
 *                    NULL on errors
 *
 * Example: \code
 *   char *p = kogmo_rtdb_obj_dumpbase_str (&objdata);
 *   printf ("%s", p); free(p); \endcode
 */
char*
kogmo_rtdb_obj_dumpbase_str (kogmo_rtdb_handle_t *db_h, void *data_p);


/*! \brief Dump the Data of an Object as Hex to a string
 *
 * \param db_h        database handle
 * \param data_p      Pointer to a Object-Data-Struct
 * \returns           Pointer to a string that will contain the dump in ASCII;
 *                    YOU have to FREE it after usage!\n
 *                    NULL on errors
 *
 * Example: \code
 *   char *p = kogmo_rtdb_obj_dumphex_str (&objdata);
 *   printf ("%s", p); free(p); \endcode
 */
char*
kogmo_rtdb_obj_dumphex_str (kogmo_rtdb_handle_t *db_h, void *data_p);


/*@}*/

#ifdef __cplusplus
 }; /* namespace KogniMobil */
 }; /* extern "C" */
#endif

#endif /* KOGMO_RTDB_OBJ_BASE_FUNCS_H */
