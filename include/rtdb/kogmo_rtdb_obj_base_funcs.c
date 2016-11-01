#include <kogmo_rtdb_internal.h>

char*
kogmo_rtdb_obj_dumpbase_str (kogmo_rtdb_handle_t *db_h, void *data_p)
{
  kogmo_rtdb_subobj_base_t *objbase;
  kogmo_timestamp_string_t tstr, data_tstr;
  kogmo_rtdb_obj_c3_process_info_t pi;
  NEWSTR(buf);

  objbase = (kogmo_rtdb_subobj_base_t*) data_p;

  if ( objbase->size < sizeof(kogmo_rtdb_subobj_base_t) )
    {
      STRPRINTF(buf,"* NO DATA\n");
      return buf;
    }

  kogmo_timestamp_to_string(objbase->committed_ts, tstr);
  kogmo_timestamp_to_string(objbase->data_ts, data_tstr);
  kogmo_rtdb_obj_c3_process_getprocessinfo (db_h, objbase->committed_proc,
                                            objbase->committed_ts, pi);
  STRPRINTF(buf,"* DATA:\n");
  STRPRINTF(buf,"Committed: %s by %s, %lli bytes\n", tstr,
                pi, (long long int)objbase->size);
  STRPRINTF(buf,"Data created: %s\n", data_tstr);

  return buf;
}


char*
kogmo_rtdb_obj_dumphex_str (kogmo_rtdb_handle_t *db_h, void *data_p)
{
  kogmo_rtdb_objsize_t size;
  int i;
  NEWSTR(buf);
  size = ( (kogmo_rtdb_subobj_base_t*) data_p ) -> size;
  STRPRINTF(buf, "* BINARY DATA:\n");
  for(i=0;i<size;i++) {
   STRPRINTF(buf,"%02x ", ((unsigned char*)data_p)[i]);
  }
  STRPRINTF(buf,"\n");
  return buf;
}

