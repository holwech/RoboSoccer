/*! \file kogmo_rtdb_funcs.h
 * \brief Functions to Access the Realtime Vehicle Database (C-Interface).
 *
 * Copyright (c) 2003-2006 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 */

#ifndef KOGMO_RTDB_FUNCS_H
#define KOGMO_RTDB_FUNCS_H

#include <inttypes.h>   /* (u)int??_t */
#include <errno.h>      /* error codes are based on them so far, see kogmo_rtdb_error */
#include <pthread.h>
#include "kogmo_time.h" /* kogmo_timestamp_* */

#ifdef __cplusplus
 extern "C" {
 namespace KogniMobil {
 #define _const const   //!< constant argument for inclusion in C++
#else /* no C++ */
 #define _const         //!< constant argument, but irrelevant in C
#endif



/*! \defgroup kogmo_rtdb_meta Adding and Removing Object Metadata (Containers)
 * The Object Metadata is defined in kogmo_rtdb_obj_info_t.
 *
 * See also kogmo_rtdb_objects for object definitions and
 * kogmo_rtdb_data_funcs for functions to handle it.
 */
/*@{*/



/*! \brief Init Metadata for a new Object
 * This does not create the object and does not modify the database.
 *
 * \param db_h        database handle
 * \param metadata_p  Pointer to Object-Metadata
 * \param name        Name for Object; not empty,
 *                    allowed characters: [-_a-zA-Z0-9], no [*?~]!!
 * \param otype       Type-Identifier for Object
 * \param size_max    Maximum size for object data, use sizeof(your_object_t)
 * \returns           <0 on errors
 *
 * This function is for convenience and only helps to initialize the
 * Metadata-Struct.
 *
 * See also kogmo_rtdb_obj_info_t
 */
int
kogmo_rtdb_obj_initinfo (kogmo_rtdb_handle_t *db_h,
                         kogmo_rtdb_obj_info_t *metadata_p,
                         _const char* name, kogmo_rtdb_objtype_t otype,
                         kogmo_rtdb_objsize_t size_max);


/*! \brief Insert a new Object into the Database
 * This inserts the metadata and allocates space for object data blocks.
 *
 * \param db_h        database handle
 * \param metadata_p  Pointer to previously set up Object-Metadata
 * \returns           <0 on errors, new Object-ID on success
 *                    (the new Object-ID will also be written into metadata_p)
 */
kogmo_rtdb_objid_t
kogmo_rtdb_obj_insert (kogmo_rtdb_handle_t *db_h,
                       kogmo_rtdb_obj_info_t *metadata_p);


/*! \brief Delete an existing Object
 * This removes an object, including its metadata and object data blocks.
 * In the history it will still remain for the time given at creation.
 *
 * \param db_h        database handle
 * \param metadata_p  Pointer to its Object-Metadata (only the Object-ID
 *                    will be used)
 * \returns           <0 on errors
 */
int
kogmo_rtdb_obj_delete (kogmo_rtdb_handle_t *db_h,
                       kogmo_rtdb_obj_info_t *metadata_p);


/*MAYBETODO \brief Replace an existing Object
 * This removes an object, including its metadata and object data blocks,
 * and creates another object, with a new metadata blocks and new
 * data blocks (the new object might have a different type and may therefore
 * require data blocks of a different size).
 * The main reason for replacing is that it notifies other processes what
 * happend and to maintain links (possibly update parent relationsships).
 *
 * This can be regarded as kogmo_rtdb_obj(meta)_modify().
 *
 * Intended use: You discovered a new object, first you thought it is a car,
 * then you realized that it is somethink other.
int
kogmo_rtdb_obj_replace (kogmo_rtdb_handle_t *db_h,
                       kogmo_rtdb_obj_info_t *old_metadata_p,
                       kogmo_rtdb_obj_info_t *new_metadata_p);
 */


/*! \brief Select an Object by its Object-ID
 * Selecting an object copies its metadata into the given metadata_p.
 *
 * \param db_h        database handle
 * \param oid         The desired Object-ID
 * \param ts          Timestamp at which the Object must be alive (and not
 *                    deleted); 0 for "now"
 * \param metadata_p  Pointer to a Object-Metadata-Struct that will get a copy
 *                    of the Metadata of the found Object
 * \returns           <0 on errors
 */
int
kogmo_rtdb_obj_readinfo (kogmo_rtdb_handle_t *db_h,
                                kogmo_rtdb_objid_t oid,
                                kogmo_timestamp_t ts,
                                kogmo_rtdb_obj_info_t *metadata_p);


/*! \brief Find an Object by its Name, Parent, creating Process and Time
 * This one searches only in the object metadata.
 *
 * \param db_h        database handle
 * \param name        The desired Object Name; NULL or "" for any name;
 *                    start with ~ to use a Regular Expression, e.g. "~^c3_.*$"
 * \param otype       If !=0, find only Objects with this Type
 * \param parent_oid  If !=0, find only Objects with this Parent's Object-ID
 * \param proc_oid    If !=0, find only Objects created by this Process-ID,
 *                    see kogmo_rtdb_process
 * \param ts          Timestamp at which the Object must be alive (and not
 *                    deleted); 0 for "now"
 * \param idlist      If not NULL, the list of found Object-IDs will be written
 *                    into this pre-allocated Array, terminated by 0. The
 *                    maximum size is fixed, see kogmo_rtdb_objid_list.
 * \param nth         If !=0, stop the search after the n-th occurence
 *                    (starting with 1)
 *                    So if you want the oid of the first hit, set idlist=NULL and nth=1.
 *                    Set nth=2 for the second hit, and so on.
 * \returns           <0 on error;\n
 *                    if no idlist is given, the Object-IDs of the nth result;\n
 *                    if an idlist is given, the real amount of matches (can be
 *                    higher than the length of idlist!).
 */
kogmo_rtdb_objid_t
kogmo_rtdb_obj_searchinfo (kogmo_rtdb_handle_t *db_h,
                           _const char *name,
                           kogmo_rtdb_objtype_t otype,
                           kogmo_rtdb_objid_t parent_oid,
                           kogmo_rtdb_objid_t proc_oid,
                           kogmo_timestamp_t ts,
                           kogmo_rtdb_objid_list_t idlist,
                           int nth);

/*! \brief Search for an Object specified by its Name, Parent, creating Process and Time, and if it doesn't exist, wait for its creation
 * This function searches only in the object metadata.
 *
 * \param db_h        database handle
 * \param name        The desired Object Name; NULL or "" for any name;
 *                    start with ~ to use a Regular Expression, e.g. "~^c3_.*$"
 * \param otype       If !=0, find only Objects with this Type
 * \param parent_oid  If !=0, find only Objects with this Parent's Object-ID
 * \param proc_oid    If !=0, find only Objects created by this Process-ID,
 *                    see kogmo_rtdb_process
 * \returns           <0 on error, -KOGMO_RTDB_ERR_INVALID if Parent-OID does not exist;\n
 *                    the Object-IDs of the first result
 */
kogmo_rtdb_objid_t
kogmo_rtdb_obj_searchinfo_wait(kogmo_rtdb_handle_t *db_h,
                           _const char *name,
                           kogmo_rtdb_objtype_t otype,
                           kogmo_rtdb_objid_t parent_oid,
                           kogmo_rtdb_objid_t proc_oid);




/*! \brief Wait until an object gets created or deleted that matches certain criteria, compare with given list
 * This one searches only in the object metadata.
 *
 * \param db_h        database handle
 * \param name        The desired Object Name; NULL or "" for any name;
 *                    start with ~ to use a Regular Expression, e.g. "~^c3_.*$"
 * \param otype       If !=0, find only Objects with this Type
 * \param parent_oid  If !=0, find only Objects with this Parent's Object-ID
 * \param proc_oid    If !=0, find only Objects created by this Process-ID,
 *                    see kogmo_rtdb_process
 * \param knownidlist  The list of previously found Object-IDs from kogmo_rtdb_obj_searchinfo().
 *                    This list will be updated.
 * \param added_idlist    The list of newly created objects.
 * \param deleted_idlist  The list of newly deleted objects.
 * \returns           <0 on error, -KOGMO_RTDB_ERR_INVALID if Parent-OID does not exist;\n
 *                    the number of created+deleted object.
 */
int
kogmo_rtdb_obj_searchinfo_waitnext(kogmo_rtdb_handle_t *db_h,
                           _const char *name,
                           kogmo_rtdb_objtype_t otype,
                           kogmo_rtdb_objid_t parent_oid,
                           kogmo_rtdb_objid_t proc_oid,
                           kogmo_rtdb_objid_list_t known_idlist,
                           kogmo_rtdb_objid_list_t added_idlist,
                           kogmo_rtdb_objid_list_t deleted_idlist);




/*! \brief Dump the Metadata of an Object to a string
 *
 * \param db_h        database handle
 * \param metadata_p  Pointer to a Object-Metadata-Struct
 * \returns           Pointer to a string that will contain the dump in ASCII;
 *                    YOU have to FREE it after usage!\n
 *                    NULL on errors
 *
 * Example: \code
 *   char *p = kogmo_rtdb_obj_dumpinfo_str (&objmeta);
 *   printf ("%s", p); free(p); \endcode
 */
char *
kogmo_rtdb_obj_dumpinfo_str (kogmo_rtdb_handle_t *db_h,
                                  kogmo_rtdb_obj_info_t *metadata_p);

/*@}*/






/*! \defgroup kogmo_rtdb_data Writing and Reading Object Data (Contents)
 * \brief This are the functions to write the Data-Block of an Object
 * to the database; normally this is done at every cycle.
 */
/*@{*/

/*! \brief Init Datablock for a new Object
 * This does not write the object and does not modify the database.
 *
 * \param db_h        database handle
 * \param metadata_p  Pointer to Object-Metadata
 * \param data_p  Pointer to a Object-Data-Struct
 * \returns           <0 on errors
 *
 * This function is for convenience and only helps to initialize the
 * Data-Struct.
 *
 * See also kogmo_rtdb_obj_info_t
 */
int
kogmo_rtdb_obj_initdata (kogmo_rtdb_handle_t *db_h,
                         kogmo_rtdb_obj_info_t *metadata_p,
                         void *data_p);



/*! \brief Write the current Data of an Objects to the Database
 * This function writes the Data-Block of an Object to the database;
 * this is used to update an object and is done every cycle.
 *
 * \param db_h    database handle
 * \param oid     Object-ID of the Object
 * \param data_p  Pointer to a Object-Data-Struct
 * \returns       <0 on errors
 *
 * Please note:
 *  - The data_p should point to a struct, that starts with a sub-struct
 *    kogmo_rtdb_subobj_base_t
 *  - Set your kogmo_rtdb_subobj_base_t.size to the correct size of your
 *    whole data-struct
 *  - Set kogmo_rtdb_obj_subbase_t.data_ts
 *    Do not commit data with timestamps old than in your last commit.\n
 *    (TODO: check for it, maybe refuse it)
 */
int
kogmo_rtdb_obj_writedata (kogmo_rtdb_handle_t *db_h,
                       kogmo_rtdb_objid_t oid, void *data_p);

/*! \brief Start pointer-based write (fast but difficult)
 * Call this function to receive a pointer where you can
 * write your Data. If you are done, call kogmo_rtdb_obj_writedata_ptr_commit.
 *
 * \param db_h    database handle
 * \param oid     Object-ID of the Object
 * \param data_pp Pointer to a Pointer to a Object-Data-Struct
 * \returns       <0 on errors
 *
 *
 * Please note:
 *  - The object must be created by you.
 *  - The object must not be public writable.
 *  - The maximum length is objmeta.size_max.
 *  - You must manually set base.size before each write.
 *  - Be cautious! You can damage the whole database.
 *  - DO NOT TOUCH base.committed_ts !!!
 *  - Pointer operations are useful for object with >= 10KB size.
 */
kogmo_rtdb_objsize_t
kogmo_rtdb_obj_writedata_ptr_begin (kogmo_rtdb_handle_t *db_h,
                                    kogmo_rtdb_objid_t oid,
                                    void *data_pp);

/*! \brief Finish pointer-based write and publish data
 * Call this function when you are done.
 * DO NOT MODIFY THE DATA AFTERWARDS!!!
 *
 * \param db_h    database handle
 * \param oid     Object-ID of the Object
 * \param data_pp Pointer to a Pointer to a Object-Data-Struct
 * \returns       <0 on errors
 *
 */
kogmo_rtdb_objsize_t
kogmo_rtdb_obj_writedata_ptr_commit (kogmo_rtdb_handle_t *db_h,
                                     kogmo_rtdb_objid_t oid,
                                     void *data_pp);



/*! \brief Read the Data of an Object from the Database (last Commit)
 * This function reads the Data-Block of an Object from the database.\n
 * It takes the block from the latest commit.
 *
 * \param db_h    database handle
 * \param oid     Object-ID of the desired Object
 * \param ts      Timestamp at which the Object must be "the last committed";
 *                0 for "now"
 * \param data_p  Pointer to a Object-Data-Struct where the found Data will
 *                be copied
 * \param size    Maximum size the Object-Data-Struct at data_p can absorb
 * \returns       <0 on errors, size of found Object on success
 */
kogmo_rtdb_objsize_t
kogmo_rtdb_obj_readdata (kogmo_rtdb_handle_t *db_h, kogmo_rtdb_objid_t oid,
                       kogmo_timestamp_t ts,
                       void *data_p, kogmo_rtdb_objsize_t size);

/*! \brief Experimental: Return Pointer to the Data of an Object from the Database (last Commit)
 * At the end of your Calculations you SHOULD call this function again and check
 * that your pointer is still valid and the same!
 * It takes the block from the latest commit.
 *
 * \param db_h    database handle
 * \param oid     Object-ID of the desired Object
 * \param ts      Timestamp at which the Object must be "the last committed";
 *                0 for "now"
 * \param data_pp Pointer to A POINTER TO a Object-Data-Struct where the found Data will
 *                be copied. Example: \n
 *                kogmo_rtdb_obj_c3_blaobj_t *myobj_p; kogmo_rtdb_obj_readdata_ptr(..,&myobj_p); access data with e.g. myobj_p->base.data_ts
 * \param size    Maximum size the Object-Data-Struct at data_p can absorb
 * \returns       <0 on errors, size of found Object on success
 */
kogmo_rtdb_objsize_t
kogmo_rtdb_obj_readdata_ptr (kogmo_rtdb_handle_t *db_h, kogmo_rtdb_objid_t oid,
                       kogmo_timestamp_t ts, void *data_pp);


/*! \brief Read the Data of Object committed before a after given Timestamp
 * This function reads the Data-Block of an Object from the database,\n
 * whose Commit Timestamp is less than the given one.
 *
 * \param db_h    database handle
 * \param oid     Object-ID of the desired Object
 * \param ts      Timestamp to compare with the Object-Data.
 *                (the timestamp in the returned object will be less
 *                than this timestamp; no interpolation will be done!)
 * \param data_p  Pointer to a Object-Data-Struct where the found Data will
 *                be copied
 * \param size    Maximum size the Object-Data-Struct at data_p can absorb
 * \returns       <0 on errors, size of found Object on success
 */
kogmo_rtdb_objsize_t
kogmo_rtdb_obj_readdata_older (kogmo_rtdb_handle_t *db_h,
                             kogmo_rtdb_objid_t oid,
                             kogmo_timestamp_t ts,
                             void *data_p, kogmo_rtdb_objsize_t size);


/*! \brief Read the Data of Object committed after a given Timestamp
 * This function reads the Data-Block of an Object from the database,\n
 * whose Commit Timestamp is greater than the given one.
 *
 * \param db_h    database handle
 * \param oid     Object-ID of the desired Object
 * \param ts      Timestamp to compare with the Object-Data.
 *                (the timestamp in the returned object will be greater
 *                than this timestamp; no interpolation will be done!)
 * \param data_p  Pointer to a Object-Data-Struct where the found Data will
 *                be copied
 * \param size    Maximum size the Object-Data-Struct at data_p can absorb
 * \returns       <0 on errors, size of found Object on success
 */
kogmo_rtdb_objsize_t
kogmo_rtdb_obj_readdata_younger (kogmo_rtdb_handle_t *db_h,
                               kogmo_rtdb_objid_t oid,
                               kogmo_timestamp_t ts,
                               void *data_p, kogmo_rtdb_objsize_t size);


/*! \brief Read the Data of an Object from the Database that was valid for the given Data Timestamp.
 * This function reads the Data-Block of an Object from the database,\n
 * whose Data Timestamp is equal or older than the given one.
 *
 * \param db_h    database handle
 * \param oid     Object-ID of the desired Object
 * \param ts      Timestamp at which the Object must be "the last committed";
 *                0 for "now"
 * \param data_p  Pointer to a Object-Data-Struct where the found Data will
 *                be copied
 * \param size    Maximum size the Object-Data-Struct at data_p can absorb
 * \returns       <0 on errors, size of found Object on success
 */
kogmo_rtdb_objsize_t
kogmo_rtdb_obj_readdata_datatime (kogmo_rtdb_handle_t *db_h, kogmo_rtdb_objid_t oid,
                       kogmo_timestamp_t ts,
                       void *data_p, kogmo_rtdb_objsize_t size);


/*! \brief Read the Data of Object that is older than a given Data Timestamp
 * This function reads the Data-Block of an Object from the database,\n
 * whose Data Timestamp is less than the given one.
 *
 * \param db_h    database handle
 * \param oid     Object-ID of the desired Object
 * \param ts      Timestamp to compare with the Object-Data.
 *                (the timestamp in the returned object will be less
 *                or equal to this timestamp; no interpolation will be done!)
 * \param data_p  Pointer to a Object-Data-Struct where the found Data will
 *                be copied
 * \param size    Maximum size the Object-Data-Struct at data_p can absorb
 * \returns       <0 on errors, size of found Object on success
 */
kogmo_rtdb_objsize_t
kogmo_rtdb_obj_readdata_dataolder (kogmo_rtdb_handle_t *db_h,
                             kogmo_rtdb_objid_t oid,
                             kogmo_timestamp_t ts,
                             void *data_p, kogmo_rtdb_objsize_t size);


/*! \brief Read the Data of Object that is younger than a given Data Timestamp
 * This function reads the Data-Block of an Object from the database,\n
 * whose Data Timestamp is greater than the given one.
 *
 * \param db_h    database handle
 * \param oid     Object-ID of the desired Object
 * \param ts      Timestamp to compare with the Object-Data.
 *                (the timestamp in the returned object will be greater
 *                or equal to this timestamp; no interpolation will be done!)
 * \param data_p  Pointer to a Object-Data-Struct where the found Data will
 *                be copied
 * \param size    Maximum size the Object-Data-Struct at data_p can absorb
 * \returns       <0 on errors, size of found Object on success
 */
kogmo_rtdb_objsize_t
kogmo_rtdb_obj_readdata_datayounger (kogmo_rtdb_handle_t *db_h,
                               kogmo_rtdb_objid_t oid,
                               kogmo_timestamp_t ts,
                               void *data_p, kogmo_rtdb_objsize_t size);



/*! \brief Get the latest Data for an Object that has been committed after a given timestamp, and wait if there is no newer data
 * This function waits until the Data-Block of an Object has an 
 * Commit-Timestamp greater than a given Timestamp (normally the last
 * known Timestamp).
 *
 * \param db_h    database handle
 * \param oid     Object-ID of the desired Object
 * \param old_ts  Timestamp to compare with the Object-Data,
 *                the commit-timestamp in the returned object will be greater than this one
 * \param data_p  Pointer to a Object-Data-Struct where the found Data will
 *                be copied
 * \param size    Maximum size the Object-Data-Struct at data_p can absorb
 * \returns       <0 on errors, size of found Object on success
 */
kogmo_rtdb_objsize_t
kogmo_rtdb_obj_readdata_waitnext (kogmo_rtdb_handle_t *db_h,
                            kogmo_rtdb_objid_t oid, kogmo_timestamp_t old_ts,
                            void *data_p, kogmo_rtdb_objsize_t size);


/*! \brief kogmo_rtdb_obj_readdata_waitnext() with pointer.
 * see: kogmo_rtdb_obj_readdata_waitnext() and kogmo_rtdb_obj_readdata_ptr()
 */
kogmo_rtdb_objsize_t
kogmo_rtdb_obj_readdata_waitnext_ptr (kogmo_rtdb_handle_t *db_h,
                            kogmo_rtdb_objid_t oid, kogmo_timestamp_t old_ts,
                            void *data_p, kogmo_rtdb_objsize_t size);



/*@}*/



/*! \defgroup kogmo_rtdb_until Waiting for Data with Timeouts (Data and Metadata)
 * These functions contain an additional parameter wakeup_ts that
 * specifies an *absolute* timestamp when to wakeup from a blocking call.
 * They return -KOGMO_RTDB_ERR_TIMEOUT in this case.
 * With 0 as wakeup_ts they behave like the function that doen't have
 * a trailing _until in its name. \n
 * kogmo_rtdb_obj_readdata_waitnext_until(..,0) = kogmo_rtdb_obj_readdata_waitnext()
 */
/*@{*/
#define HAVE_wait_until
/*! \brief kogmo_rtdb_obj_searchinfo_wait() with wakeup.
 * see: kogmo_rtdb_obj_searchinfo_wait()
 * \param wakeup_ts time at which to wake up if there is no data available.
 *        returns -KOGMO_RTDB_ERR_TIMEOUT in this case. 0=infinite, same as kogmo_rtdb_obj_searchinfo_wait().
 */
kogmo_rtdb_objid_t
kogmo_rtdb_obj_searchinfo_wait_until(kogmo_rtdb_handle_t *db_h,
                           _const char *name,
                           kogmo_rtdb_objtype_t otype,
                           kogmo_rtdb_objid_t parent_oid,
                           kogmo_rtdb_objid_t proc_oid,
                           kogmo_timestamp_t wakeup_ts);

/*! \brief kogmo_rtdb_obj_searchinfo_waitnext() with wakeup.
 * see: kogmo_rtdb_obj_searchinfo_waitnext()
 * \param wakeup_ts time at which to wake up if there is no data available.
 *        returns -KOGMO_RTDB_ERR_TIMEOUT in this case.
 */
int
kogmo_rtdb_obj_searchinfo_waitnext_until(kogmo_rtdb_handle_t *db_h,
                           _const char *name,
                           kogmo_rtdb_objtype_t otype,
                           kogmo_rtdb_objid_t parent_oid,
                           kogmo_rtdb_objid_t proc_oid,
                           kogmo_rtdb_objid_list_t known_idlist,
                           kogmo_rtdb_objid_list_t added_idlist,
                           kogmo_rtdb_objid_list_t deleted_idlist,
                           kogmo_timestamp_t wakeup_ts);

/*! \brief kogmo_rtdb_obj_readdata_waitnext() with wakeup.
 * see: kogmo_rtdb_obj_readdata_waitnext()
 * \param wakeup_ts time at which to wake up if there is no data available.
 *        returns -KOGMO_RTDB_ERR_TIMEOUT in this case.
 */
kogmo_rtdb_objsize_t
kogmo_rtdb_obj_readdata_waitnext_until(
                            kogmo_rtdb_handle_t *db_h,
                            kogmo_rtdb_objid_t oid, kogmo_timestamp_t old_ts,
                            void *data_p, kogmo_rtdb_objsize_t size,
                            kogmo_timestamp_t wakeup_ts);

/*! \brief kogmo_rtdb_obj_readdata_waitnext_ptr() with wakeup.
 * see: kogmo_rtdb_obj_readdata_waitnext_ptr()
 * \param wakeup_ts time at which to wake up if there is no data available.
 *        returns -KOGMO_RTDB_ERR_TIMEOUT in this case.
 */
kogmo_rtdb_objsize_t
kogmo_rtdb_obj_readdata_waitnext_until_ptr(
                            kogmo_rtdb_handle_t *db_h,
                            kogmo_rtdb_objid_t oid, kogmo_timestamp_t old_ts,
                            void *data_p, kogmo_rtdb_objsize_t size,
                            kogmo_timestamp_t wakeup_ts);
/*@}*/



/*! \defgroup kogmo_rtdb_misc Other Functions (Get Timestamp,..)
 */   
/*! \brief Get absolute Timestamp for current Time within the given
 * database (can be different to kogmo_timestamp_now() when in
 * simulation mode.
 * \param db_h    database handle
 * \returns zero on errors
 */
/*@{*/
kogmo_timestamp_t
kogmo_rtdb_timestamp_now (kogmo_rtdb_handle_t *db_h);

/*! \brief Set current Time to the given absolute Timestamp.
 * This has only an effect if the rtdb is in simulation mode
 * (kogmo_rtdb_man -s) and the clients use kogmo_rtdb_timestamp_now()
 * instead of kogmo_timestamp_now()).
 * \param db_h    database handle
 * \param new_ts  new timestamp (0: give client the real current time, kogmo_timestamp_now())
 * \returns 0 for OK, -KOGMO_RTDB_ERR_NOPERM if RTDB is not in simulation mode.
 */
int
kogmo_rtdb_timestamp_set (kogmo_rtdb_handle_t *db_h,
                          kogmo_timestamp_t new_ts);
/*@}*/



/*! \defgroup kogmo_rtdb_conn Connection Handling
 */
/*@{*/




  
/*! \brief Init Info for a new Database Connection
 * This does not connect. It's only for convenience.
 * See kogmo_rtdb_connect_info_t.
 *
 * \param conninfo  pointer to a Database Connection Info
 * \param dbhost    database specifier, "" for the local default database, not more than 25 characters
 * \param procname  choosen name, like a2_roadtracker
 * \param cycletime your internal cycle time in seconds (0 for system default);
 *                  you must signal your vitality every cycle with
 *                  kogmo_rtdb_cycle_done()
 * \returns         <0 on errors
 */
int
kogmo_rtdb_connect_initinfo (kogmo_rtdb_connect_info_t *conninfo,
                             _const char *dbhost,
                             _const char *procname, float cycletime);


/*! \brief Open Connection to Realtime Vehicle Database
 *
 * \param db_h      storage for the new database handle
 * \param conninfo  connection parameters and process infos
 * \returns         your process-ID (a positive number) on success,
 *                  <0 on errors
 *
 * Please note:
 *  - At process termination you should call kogmo_rtdb_disconnect()
 *  - By default there will be installed Signal-Handlers for SIG_TERM/QUIT/INT
 *    that call kogmo_rtdb_disconnect() automatically, but please don't
 *    rely on this
 */
kogmo_rtdb_objid_t
kogmo_rtdb_connect (kogmo_rtdb_handle_t **db_h,
                    kogmo_rtdb_connect_info_t *conninfo);


/* \brief Disconnect from Realtime Vehicle Database
 *
 * \param db_h      database handle
 * \param discinfo  normally NULL (might contain special flags later)
 * \returns         <0 on errors
 *
 * You should call this on process termination.
 */
int
kogmo_rtdb_disconnect (kogmo_rtdb_handle_t *db_h,
                       void *discinfo);

/* \brief Inform other Processes about your status and signal own Vitality to Realtime Vehicle Database
 *
 * \param db_h      database handle
 * \param status    your new status, one of kogmo_rtdb_obj_c3_process_status_t
 * \param msg       an arbitrary status message for humans, NULL to keep the last one,
 *                  at maximum KOGMO_RTDB_PROCSTATUS_MSG_MAXLEN characters.
 * \param flags     normally 0
 * \returns         <0 on errors
 *
 * You should call this regularly at the end of each of your cycles.
 */
int  
kogmo_rtdb_setstatus (kogmo_rtdb_handle_t *db_h, uint32_t status, _const char* msg, uint32_t flags);
// see kogmo_rtdb.hxx!
#define HAVE_DEFINED_setstatus

/* \brief Inform other Processes that you finished your cycle and signal own Vitality to Realtime Vehicle Database.
 * This is equal to kogmo_rtdb_setownstatus(db_h, KOGMO_RTDB_PROCSTATUS_CYCLEDONE, "", 0)
 *
 * \param db_h      database handle
 * \param flags     normally 0
 * \returns         <0 on errors
 *
 * You should call this (or kogmo_rtdb_setstatus()) regularly at the end of each of your cycles.
 */
int  
kogmo_rtdb_cycle_done (kogmo_rtdb_handle_t *db_h, uint32_t flags);



int
kogmo_rtdb_pthread_create(kogmo_rtdb_handle_t *db_h,
                          pthread_t *thread,
                          pthread_attr_t *attr,
                          void *(*start_routine)(void*), void *arg);
int
kogmo_rtdb_pthread_kill(kogmo_rtdb_handle_t *db_h,
                        pthread_t thread, int sig);
int
kogmo_rtdb_pthread_join(kogmo_rtdb_handle_t *db_h,
                        pthread_t thread, void **value_ptr);

// NOTE: kogmo_rtdb_sleep_until() in simulation mode:
//       => works only if simulation time runs slower than real-time! (true in most cases)
int
kogmo_rtdb_sleep_until(kogmo_rtdb_handle_t *db_h,
                       _const kogmo_timestamp_t wakeup_ts);

/*@}*/




/*! \defgroup kogmo_rtdb_error Errors and Debugging
 * \brief These values can be returned by Calls to the Database-API.
 * They are returned as an integer (int) or a type kogmo_rtdb_objid_t.\n
 * So far, they are based on errno.h for simplicity. \n
 *
 * Please note:
 *  - All error values returned are negative!
 */
/*@{*/

// angelehnt an /usr/include/asm/errno.h:
#define KOGMO_RTDB_ERR_NOPERM   EACCES //!< 13 Permission denied
#define KOGMO_RTDB_ERR_NOTFOUND ENOENT //!< 2  Object/Process not found
#define KOGMO_RTDB_ERR_NOMEMORY ENOMEM //!< 12 Out of Memory
#define KOGMO_RTDB_ERR_INVALID  EINVAL //!< 22 Invalid Argument(s)
#define KOGMO_RTDB_ERR_OUTOFOBJ EMFILE //!< 24 Out of Object/Process Slots
#define KOGMO_RTDB_ERR_NOTUNIQ  EEXIST //!< 17 Unique Object already exists
#define KOGMO_RTDB_ERR_UNKNOWN  EPERM  //!< 1  General/unspecified error (-1)
#define KOGMO_RTDB_ERR_CONNDENY ECONNREFUSED //!< 111 Connection refused           
#define KOGMO_RTDB_ERR_NOCONN   ENOTCONN //!< 107 Not connected
#define KOGMO_RTDB_ERR_HISTWRAP ESTALE //!< 116 History wrap-around, stale data
#define KOGMO_RTDB_ERR_TOOFAST  EAGAIN //!< 11 Updates too fast, try again
#define KOGMO_RTDB_ERR_TIMEOUT  ETIMEDOUT //!< Waiting for data timed out

/// current debug-level (internal)
extern int kogmo_rtdb_debug;
/*@}*/










/*! \brief Temporary solution for simulation because of thread-blocking (boost problem?)
 * similar to kogmo_rtdb_obj_searchinfo_waitnext() but non-blocking.
 */
int
kogmo_rtdb_obj_searchinfo_lists_nonblocking(kogmo_rtdb_handle_t *db_h,
                           _const char *name,
                           kogmo_rtdb_objtype_t otype,
                           kogmo_rtdb_objid_t parent_oid,
                           kogmo_rtdb_objid_t proc_oid,
                           kogmo_rtdb_objid_list_t known_idlist,
                           kogmo_rtdb_objid_list_t added_idlist,
                           kogmo_rtdb_objid_list_t deleted_idlist);


/*! \brief (beta) Function to access object history slot-by-slot
 * mode: 0=get latest + init objslot (use this first)
 *       1=check old objslot + use offset for reading (use this afterwards and set offset)
 *      -1=use offset for reading (don't check old objslot - will not detect a wrap-around) 
 */
kogmo_rtdb_objsize_t
kogmo_rtdb_obj_readdataslot_ptr (kogmo_rtdb_handle_t *db_h,
                                 int32_t mode, int32_t offset,  
                                 kogmo_rtdb_obj_slot_t *objslot,
                                 void *data_pp);



#ifdef __cplusplus
 }; /* namespace KogniMobil */
 }; /* extern "C" */
#endif


#endif /* KOGMO_RTDB_FUNCS_H */
