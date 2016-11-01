#include <kogmo_rtdb_internal.h>

char*
kogmo_rtdb_obj_c3_sixdof_dump_str (kogmo_rtdb_handle_t *db_h,
                                   void *data_p)
{
  kogmo_rtdb_obj_c3_sixdof_t *obj;
  char *p;
  NEWSTR(buf);

  obj = (kogmo_rtdb_obj_c3_sixdof_t*) data_p;

  p = kogmo_rtdb_obj_dumpbase_str ( db_h, &obj->base);
  if ( p != NULL)
    {
      STRPRINTF(buf,"%s",p);
      free (p);
    }
  
  STRPRINTF(buf,"Position: x=%f, y=%f, z=%f\n",
                 obj->sixdof.x,
                 obj->sixdof.y,
                 obj->sixdof.z);
  STRPRINTF(buf,"Orientation: yaw=%f, pitch=%f, roll=%f\n",
                 obj->sixdof.yaw,
                 obj->sixdof.pitch,
                 obj->sixdof.roll);

  return buf;
}


// Functions for Process-Object (internal):


#include <kogmo_rtdb_internal.h>

kogmo_rtdb_objid_t
kogmo_rtdb_obj_c3_process_searchprocessobj (kogmo_rtdb_handle_t *db_h,
                                            kogmo_timestamp_t ts,
                                            kogmo_rtdb_objid_t proc_oid)
{
  kogmo_rtdb_objid_t proclistoid,procoid;

  proclistoid = kogmo_rtdb_obj_searchinfo (db_h, "processes",
                                           KOGMO_RTDB_OBJTYPE_C3_PROCESSLIST,
                                           0, 0, ts, NULL, 1);
DBG("%lli",(long long)proclistoid);
  if ( proclistoid < 0 )
    return 0;

  procoid = kogmo_rtdb_obj_searchinfo (db_h, "",
                                       KOGMO_RTDB_OBJTYPE_C3_PROCESS,
                                       proclistoid, proc_oid, ts, NULL, 1);
  if ( procoid < 0 )
    return 0;

  return procoid;
}

kogmo_rtdb_objid_t
kogmo_rtdb_obj_c3_process_getprocessinfo (kogmo_rtdb_handle_t *db_h,
                                          kogmo_rtdb_objid_t proc_oid,
                                          kogmo_timestamp_t ts,
                                          kogmo_rtdb_obj_c3_process_info_t str)
{
  int err;
  kogmo_rtdb_obj_info_t om;
  kogmo_rtdb_obj_c3_process_t po;
  kogmo_rtdb_objid_t oid;

  memset(str, 0, sizeof(kogmo_rtdb_obj_c3_process_info_t));
  strcpy(str, "?");

  oid = kogmo_rtdb_obj_c3_process_searchprocessobj (db_h, ts, proc_oid);

  if ( oid < 0 )
    return oid;

  err = kogmo_rtdb_obj_readinfo ( db_h, oid, ts, &om);
  if ( err < 0 )
    return err;

  err = kogmo_rtdb_obj_readdata (db_h, oid, ts, &po, sizeof (po) );
  if ( err < 0 )
    return err;

  sprintf(str, "%s%s(%lli)", om.deleted_ts ? "D!":"", om.name,
               (long long int) om.oid);

  //sprintf(str, "%s%s [OID %lli, PID %i]", om.deleted_ts ? "D!":"", om.name,
  //             (long long int) om.oid,
  //             po.process.pid);

  return oid;
}
