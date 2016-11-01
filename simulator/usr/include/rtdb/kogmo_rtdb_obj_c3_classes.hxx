/*! \file kogmo_rtdb_obj_c3_classes.hxx
 * \brief Objects from C3
 *
 * Copyright (c) 2003-2006 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 */

#ifndef KOGMO_RTDB_OBJ_C3_CLASSES_HXX
#define KOGMO_RTDB_OBJ_C3_CLASSES_HXX

#include "kogmo_rtdb_obj_base_classes.hxx"
#include "kogmo_rtdb_obj_typeids.h"
#include "kogmo_rtdb_obj_c3.h"
#include <sstream> // std::ostringstream

namespace KogniMobil {

/*! \addtogroup kogmo_rtdb_objects_cxx
 */
/*@{*/

class C3_SixDoF : public RTDBObj
{
  protected:
    kogmo_rtdb_subobj_c3_sixdof_t *objsixdof_p;
  public:
    C3_SixDoF (class RTDBConn& DBC,
               const char* name = "",
               const int& otype = KOGMO_RTDB_OBJTYPE_C3_SIXDOF,
               const int32_t& child_size = 0, char** child_dataptr = NULL)
        : RTDBObj (DBC, name, otype, sizeof(kogmo_rtdb_subobj_c3_sixdof_t) +
                   child_size, (char**)&objsixdof_p)
      {
        // Pass a Pointer pointing right after the base data to our child 
        if ( child_size && child_dataptr != NULL )
          *child_dataptr = (char*)objsixdof_p + sizeof (kogmo_rtdb_subobj_c3_sixdof_t);

        // Init Data
        objsixdof_p -> x   = objsixdof_p -> y     = objsixdof_p -> z    = 0;
        objsixdof_p -> yaw = objsixdof_p -> pitch = objsixdof_p -> roll = 0;
      };

    void setSixDoF (const float& x, const float& y, const float& z,
                    const float& yaw, const float& pitch, const float& roll)
      {
        objsixdof_p -> x = x;
        objsixdof_p -> y = y;
        objsixdof_p -> z = z;
        objsixdof_p -> yaw = yaw;
        objsixdof_p -> pitch = pitch;
        objsixdof_p -> roll = roll;
      };

    void setLat (const float& x, const float& y, const float& z)
      {
        objsixdof_p -> x = x;
        objsixdof_p -> y = y;
        objsixdof_p -> z = z;
      };

    void setRot (const float& yaw, const float& pitch, const float& roll)
      {
        objsixdof_p -> yaw = yaw;
        objsixdof_p -> pitch = pitch;
        objsixdof_p -> roll = roll;
      };

    float getX     (void) const { return objsixdof_p -> x;     }
    float getY     (void) const { return objsixdof_p -> y;     }
    float getZ     (void) const { return objsixdof_p -> z;     }
    float getYaw   (void) const { return objsixdof_p -> yaw;   }
    float getPitch (void) const { return objsixdof_p -> pitch; }
    float getRoll  (void) const { return objsixdof_p -> roll;  }

#define C3_PI 3.141592653589793238
#define C3_RAD2DEG(a) ((a)/2/C3_PI*360)

    std::string dump (void) const
      {
        std::ostringstream ostr;
        ostr << "* Relative Coordinates to Parent:" << std::endl;
        ostr << "rel K_CGP: x="     << objsixdof_p -> x
                     << ", y="     << objsixdof_p -> y
                     << ", z="     << objsixdof_p -> z
                     << ", yaw="   << objsixdof_p -> yaw   << " (" << C3_RAD2DEG(objsixdof_p -> yaw) << " deg)"
                     << ", pitch=" << objsixdof_p -> pitch << " (" << C3_RAD2DEG(objsixdof_p -> pitch) << " deg)"
                     << ", roll="  << objsixdof_p -> roll  << " (" << C3_RAD2DEG(objsixdof_p -> roll) << " deg)"
                     << std::endl;
        return RTDBObj::dump() + ostr.str();
      };
};





// for an example: see kogmo_rtdb_obj_a2_classes.hxx: template A2_Image
class C3_GPSData
         : public RTDBObj_T < kogmo_rtdb_subobj_c3_gpsdata_t, KOGMO_RTDB_OBJTYPE_C3_GPSDATA >
{
  public:
    C3_GPSData(class RTDBConn& DBC, const char* name = "")
            : RTDBObj_T < kogmo_rtdb_subobj_c3_gpsdata_t, KOGMO_RTDB_OBJTYPE_C3_GPSDATA > (DBC, name)
      {
      }
    std::string dump (void) const
      {
        std::ostringstream ostr;
        ostr << "* Position :" << std::endl
             << "LAT :" << subobj_p->latitude << std::endl
             << "LON :" << subobj_p->longitude << std::endl
             << "ALT :" << subobj_p->altitude << std::endl
             << "SPD :" << subobj_p->speed << std::endl
             << "Valid: " << subobj_p->valid << std::endl;
        return RTDBObj::dump() + ostr.str();
      };
};


enum C3_CAMERA_NUMBERS {
 C3_CAMERA_LEFT = 1,
 C3_CAMERA_RIGHT,
 C3_CAMERA_TELE,
 C3_CAMERA_MAXNUM
};

typedef RTDBObj_T < kogmo_rtdb_subobj_c3_camstate_t, KOGMO_RTDB_OBJTYPE_C3_CAMSTATE_V1, RTDBObj > C3_CamState_T;
class C3_CamState : public C3_CamState_T
{
  public:
    C3_CamState(class RTDBConn& DBC, const char* name = "") : C3_CamState_T (DBC, name)
      {
      }

    double readYaw ( int camnum )
      {
        RTDBRead();
        while ( ! isUpdate ( camnum ) )
          {
            RTDBReadDataOlder();
          }
        return getYaw(camnum);
/*
        RTDBConn DBC(getDBC());
        C3_CamState OlderCamState(DBC);
        OlderCamState.Copy(*this);
        OlderCamState.RTDBReadDataTime(data_ts);
        if ( first_ts != data_ts )
          throw DBError("wrap?");
        while ( ! OlderCamState.isUpdate ( camnum ) )
          {
            OlderCamState.RTDBReadDataOlder();
          }
        return OlderCamState.getYaw(camnum);
*/
      }

    double getYaw ( int camnum )
      {
        switch (camnum)
         {
          case C3_CAMERA_LEFT:  return (subobj_p->left_yaw);
          case C3_CAMERA_RIGHT: return (subobj_p->right_yaw);
          case C3_CAMERA_TELE:  return (subobj_p->tele_yaw);
          default: throw DBError("Unknown camera number");
         }
        return 0; // never reached
      }

    bool isValid ( int camnum )
      {
        switch (camnum)
         {
          case C3_CAMERA_LEFT:  return (subobj_p->status.left_valid);
          case C3_CAMERA_RIGHT: return (subobj_p->status.right_valid);
          case C3_CAMERA_TELE:  return (subobj_p->status.tele_valid);
          default: throw DBError("Unknown camera number");
         }
        return 0; // never reached
      }

    bool isUpdate ( int camnum )
      {
        switch (camnum)
         {
          case C3_CAMERA_LEFT:  return (subobj_p->status.left_update);
          case C3_CAMERA_RIGHT: return (subobj_p->status.right_update);
          case C3_CAMERA_TELE:  return (subobj_p->status.tele_update);
          default: throw DBError("Unknown camera number");
         }
        return 0; // never reached
      }

    double getStableInterval ( int camnum, double searchinterval = 1.0, double maxgaps = 0.1, Timestamp data_ts = 0)
      // 0..searchinterval  time interval within the camera was stable
      // ..  stopps on data invalid, stale or insufficient updates
      // -1  error accessing data
      // -2  cannot find camera state for given data-timestamp (must be exact!)
      {
        double interval = -1.0;
        Timestamp TS;
        if (!data_ts)
          data_ts = getDataTimestamp();
        double last_yaw = getYaw(camnum);
//cout <<"Check:";
        try
          {
            RTDBConn DBC(getDBC());
            C3_CamState OlderCamState(DBC);
            OlderCamState.Copy(*this);
            OlderCamState.RTDBReadDataTime(data_ts);
            Timestamp first_ts = OlderCamState.getDataTimestamp();
            if ( first_ts != data_ts )
              return -2.0;
            Timestamp last_ts = first_ts;
            interval = 0.0;
            do
              {
//cout <<"?";
                OlderCamState.RTDBReadDataOlder();
                if ( last_yaw != OlderCamState.getYaw(camnum) )
                  break; // the camera was not stable
                if ( ! OlderCamState.isUpdate(camnum) )
                  continue; // this camera was not updated in this commit -> skip to next
                Timestamp this_ts = OlderCamState.getDataTimestamp();
                if ( last_ts - this_ts > maxgaps )
                  break; // the gaps between the measurements are to wide
                last_ts = this_ts;
                interval = first_ts - last_ts;
//cout <<"!";
              }
            while(interval < searchinterval);
          }
        catch (DBError err)
          {
//cout <<"X";
          }
//cout <<". "<<interval<<"\n";
        return interval;
      }

    std::string dump(void) const
      {
        std::ostringstream ostr;
        ostr << "* Position :" << std::endl
             << "left_yaw: " <<  subobj_p->left_yaw    << " (" << C3_RAD2DEG(subobj_p->left_yaw) << " deg)" << std::endl
             << "right_yaw: " << subobj_p->right_yaw   << " (" << C3_RAD2DEG(subobj_p->right_yaw) << " deg)" << std::endl
             << "tele_yaw: " <<  subobj_p->tele_yaw    << " (" << C3_RAD2DEG(subobj_p->tele_yaw) << " deg)" << std::endl
             << "tele_pitch: " << subobj_p->tele_pitch << " (" << C3_RAD2DEG(subobj_p->tele_pitch) << " deg)" << std::endl

             << "valid: "  	<< subobj_p->status.right_valid 
				<< subobj_p->status.left_valid 
				<< subobj_p->status.tele_valid
				<< subobj_p->status.pitch_valid << std::endl

             << "update: "   	<< subobj_p->status.right_update
				<< subobj_p->status.left_update
				<< subobj_p->status.tele_update
				<< subobj_p->status.pitch_update << std::endl

             << "moving: "   	<< subobj_p->status.right_ismoving 
				<< subobj_p->status.left_ismoving
				<< subobj_p->status.tele_ismoving
				<< subobj_p->status.pitch_ismoving << std::endl
	;

        return RTDBObj::dump() + ostr.str();
      };
};

typedef RTDBObj_T < kogmo_rtdb_subobj_c3_camcommand_t, KOGMO_RTDB_OBJTYPE_C3_CAMCOMMAND_V1, RTDBObj > C3_CamCommand_T;

class C3_CamCommand : public C3_CamCommand_T
{
  public:
    C3_CamCommand(class RTDBConn& DBC, const char* name = "") : C3_CamCommand_T (DBC, name)
      {
      }

    double getYaw ( int camnum )
      {
        switch (camnum)
         {
          case C3_CAMERA_LEFT:  return (subobj_p->left_yaw);
          case C3_CAMERA_RIGHT: return (subobj_p->right_yaw);
          case C3_CAMERA_TELE:  return (subobj_p->tele_yaw);
          default: throw DBError("Unknown camera number");
         }
        return 0; // never reached
      }

    void moveToYaw ( int camnum, double pos )
      {
        RTDBRead();
        subobj_p->command.left_update=0;
        subobj_p->command.right_update=0;
        subobj_p->command.tele_update=0;
        subobj_p->command.pitch_update=0;
        subobj_p->left_init_mode=0;
        subobj_p->right_init_mode=0;

        if ( camnum & 1 )
          {
            subobj_p->command.left_update=1;
            subobj_p->left_yaw=pos;
          }
        if ( camnum & 2 )
          {
            subobj_p->command.right_update=1;
            subobj_p->right_yaw=pos;
          }
        setDataTimestamp(0);
        RTDBWrite ();
      }

    std::string dump(void) const
      {
        std::ostringstream ostr;
        ostr << "* Position :" << std::endl
             << "left_yaw: " <<  subobj_p->left_yaw    << " (" << C3_RAD2DEG(subobj_p->left_yaw) << " deg)" << std::endl
             << "right_yaw: " << subobj_p->right_yaw   << " (" << C3_RAD2DEG(subobj_p->right_yaw) << " deg)" << std::endl
             << "tele_yaw: " <<  subobj_p->tele_yaw    << " (" << C3_RAD2DEG(subobj_p->tele_yaw) << " deg)" << std::endl
             << "tele_pitch: " << subobj_p->tele_pitch << " (" << C3_RAD2DEG(subobj_p->tele_pitch) << " deg)" << std::endl

             << "update: "   	<< subobj_p->command.right_update
				<< subobj_p->command.left_update
				<< subobj_p->command.tele_update
				<< subobj_p->command.pitch_update 
				<< subobj_p->command.cycle_update << std::endl
	     << "status_cycle: "<< subobj_p->status_cycle << std::endl
             << "tpu_reset: "  	<< subobj_p->command.right_tpu_zero
				<< subobj_p->command.left_tpu_zero
				<< subobj_p->command.tele_tpu_zero
				<< subobj_p->command.pitch_tpu_zero << std::endl;

        return RTDBObj::dump() + ostr.str();
      };
};



typedef RTDBObj_T < kogmo_rtdb_subobj_c3_text_t, KOGMO_RTDB_OBJTYPE_C3_TEXT > C3_Text_T;
class C3_Text : public C3_Text_T
{
  public:
    C3_Text(class RTDBConn& DBC, const char* name = "") : C3_Text_T (DBC, name)
      {
        setMinSize( - sizeof(char) * C3_TEXT_MAXCHARS );
        deleteText();
      }

    void deleteText (void)
      {
        setSize( getMinSize() );
      }

    int getLength (void) const
      {
        return getSize()-getMinSize();
      }

    void addText (std::string text)
      {
        if ( getLength() + text.length() >= C3_TEXT_MAXCHARS )
          throw DBError("Text too long, try to increase C3_TEXT_MAXCHARS");
        strncpy (&subobj_p->data[getLength()], text.c_str(), C3_TEXT_MAXCHARS - getLength() );
        setSize( getMinSize() + getLength() + text.length() );
      };

    void setText (std::string text)
      {
        deleteText();
        addText(text);
      };

    std::string getText () const
      {
        std::string text = "";
        if ( getLength() > 0 )
          text.append ( subobj_p->data, getLength() );
        return text;
      };
        
    std::string dump(void) const
    {
        std::ostringstream ostr;
        ostr << "* Text:" << std::endl
             << getText() << std::endl;
        return RTDBObj::dump() + ostr.str();
      }; 
};




typedef RTDBObj_T < kogmo_rtdb_subobj_c3_simtime_t, KOGMO_RTDB_OBJTYPE_SFB_CONTROLMATLABSIM, RTDBObj > SFB_SimTime_T;

class SFB_SimTime : public SFB_SimTime_T
{
  public:
    SFB_SimTime(class RTDBConn& DBC, const char* name = "") : SFB_SimTime_T (DBC, name)
      {
      }
    void setCycle ( int cycle )
      {
        subobj_p->cycle = cycle;
      }
    int getCycle ( void )
      {
        return subobj_p->cycle;
      }
    void setInterval ( float delta_sim_time )
      {
        subobj_p->delta_sim_time = delta_sim_time;
      }
    float getInterval ( void )
      {
        return subobj_p->delta_sim_time;
      }
    void setRuntime ( float runtime )
      {
        subobj_p->runtime = runtime;
      }
    float getRuntime ( void )
      {
        return subobj_p->runtime;
      }
    std::string dump(void) const
      {
        std::ostringstream ostr;
        ostr << "* Simulation Time :" << std::endl
             << "Current Time:  " << subobj_p->runtime << std::endl
             << "Current Cycle: " << subobj_p->cycle << std::endl
             << "Cycle Length:  " << subobj_p->delta_sim_time << std::endl
             << "Start/Stop: " << (subobj_p->stop?"STOP":"Start") << std::endl;
        return RTDBObj::dump() + ostr.str();
      };
};

class SFB_SimClient: public SFB_SimTime
{
  private:
    int verbose;
  public:
    SFB_SimClient(class RTDBConn& DBC) : SFB_SimTime (DBC)
      {
        RTDBSearchWait("simulation_time");
        RTDBReadWaitNext();
        verbose = 0;
      }
    void setVerbose ( int v = 1 )
      {
        verbose = v;
      }
    Timestamp getTime ( void )
      {
        return getDataTimestamp();
      }
    void skipToCurrentCycle ( void )
      {
        RTDBRead();
      }
    void waitCycle ( int cycle = 0 ) // default: next cycle
      {
        if ( cycle == 0 )
          cycle = getCycle() + 1;
        if ( verbose )
          printf("SimClient: waiting for cycle %i..\n",cycle);
        RTDBRead();
        for (;;)
          {
            if ( getCycle() >= cycle )
              break;
            if ( verbose )
              printf("SimClient: cycle %i too early, waiting..\n",getCycle());
            RTDBReadWaitNext();
          }
        while ( getCycle() > cycle )
          {
            if ( verbose )
              printf("SimClient: cycle %i too late, seeking back..\n",getCycle());
            RTDBReadPredecessor();
          }
        if ( verbose )
          printf("SimClient: reached cycle %i\n",getCycle());
      }
    bool waitObject ( RTDBObj& Obj )
      {
        int found = 0;
        while ( !found )
          {
            if ( verbose )
              printf("SimClient: waiting for Object '%s' in cycle %i..\n",Obj.getName().c_str(),getCycle());
            Obj.RTDBReadDataTime(getTime().timestamp());
            if ( Obj.getDataTimestamp() < getTime() )
              {
                if ( verbose )
                  printf("SimClient: Object '%s' in cycle %i not yet found, waiting for next commit..\n",Obj.getName().c_str(),getCycle());
                // TODO: das ist teuer(!), eigentlich muesste man nur den header lesen..
                Obj.RTDBRead();
                Obj.RTDBReadWaitNext();
                // continue;
              }
            else
              found = 1;
          }
        if ( verbose )
          printf("SimClient: found Object '%s' for cycle %i\n",Obj.getName().c_str(),getCycle());
        if ( Obj.getDataTimestamp() != getTime() )
          {
            if ( verbose )
              printf("SimClient: warning: Object '%s' for cycle %i has no exact datatimestamp==cycletimestamp match!\n",Obj.getName().c_str(),getCycle());
            return false;
          }
        else
          return true;
      }
};



typedef RTDBObj_T < kogmo_rtdb_subobj_c3_playerctrl_t, KOGMO_RTDB_OBJTYPE_C3_PLAYERCTRL, RTDBObj > C3_PlayerCtrl_T;
class C3_PlayerCtrl : public C3_PlayerCtrl_T
{
  public:
    C3_PlayerCtrl(class RTDBConn& DBC, const char* name = "") : C3_PlayerCtrl_T (DBC, name)
      {
      }
    std::string dump (void) const
      {
        std::ostringstream ostr;
        ostr << "* Player Control :" << std::endl
             << "Pause:      " << (subobj_p->flags.pause ? "ON" : "off") << std::endl
             << "Logging:    " << (subobj_p->flags.log ? "ON" : "off") << std::endl
             << "Loop:       " << (subobj_p->flags.loop ? "ON" : "off") << std::endl
             << "KeepCreated:" << (subobj_p->flags.keepcreated ? "ON" : "off") << std::endl
             << "Speed      :" << (subobj_p->speed*100.0) << "%" << std::endl
             << "Begin Time: " << Timestamp(subobj_p->begin_ts).string() << std::endl
             << "End Time:   " << Timestamp(subobj_p->end_ts).string() << std::endl
             << "GoTo Time:  " << Timestamp(subobj_p->goto_ts).string() << std::endl
             << "Frame Go:   " << subobj_p->frame_go << " " << subobj_p->frame_objname << std::endl;
        return RTDBObj::dump() + ostr.str();
      };
};

typedef RTDBObj_T < kogmo_rtdb_subobj_c3_playerstat_t, KOGMO_RTDB_OBJTYPE_C3_PLAYERSTAT, RTDBObj > C3_PlayerStat_T;
class C3_PlayerStat : public C3_PlayerStat_T
{
  public:
    C3_PlayerStat(class RTDBConn& DBC, const char* name = "") : C3_PlayerStat_T (DBC, name)
      {
      }
    std::string dump (void) const
      {
        std::ostringstream ostr;
        ostr << "* Player Status :" << std::endl
             << "First Time:   " << Timestamp(subobj_p->first_ts).string() << std::endl
             << "Last Time:    " << Timestamp(subobj_p->last_ts).string() << std::endl
             << "Current Time: " << Timestamp(subobj_p->current_ts).string() << std::endl;
        return RTDBObj::dump() + ostr.str();
      };
};

/*@}*/
}; /* namespace KogniMobil */

#endif /* KOGMO_RTDB_OBJ_C3_CLASSES_HXX */
