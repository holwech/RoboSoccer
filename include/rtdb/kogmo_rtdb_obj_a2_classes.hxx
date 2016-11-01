/*! \file kogmo_rtdb_obj_a2_classes.hxx
 * \brief Objects from A2
 *
 * Copyright (c) 2003-2006 Matthias Goebl <mg*tum.de>
 *     Lehrstuhl fuer Realzeit-Computersysteme (RCS)
 *     Technische Universitaet Muenchen (TUM)
 */

#ifndef KOGMO_RTDB_OBJ_A2_CLASSES_HXX
#define KOGMO_RTDB_OBJ_A2_CLASSES_HXX

#include "kogmo_rtdb_obj_base_classes.hxx"
#include "kogmo_rtdb_obj_typeids.h"
#include "kogmo_rtdb_obj_c3_classes.hxx"
#include "kogmo_rtdb_obj_a2.h"
#include <stdio.h>    /* snprintf() for hex conversion*/
#include <iomanip>  /* formated cout streams*/
namespace KogniMobil {

/*! \addtogroup kogmo_rtdb_objects_cxx
 */
/*@{*/


class A2_RoadKloth : public C3_SixDoF
{
  protected:
    kogmo_rtdb_subobj_a2_roadkloth_t *objroadkloth_p;
    A2_RoadKloth ( const A2_RoadKloth& ); // disable copy-constructor
  public:
    A2_RoadKloth (class RTDBConn& DBC,
               const char* name = "",
               const int& otype = KOGMO_RTDB_OBJTYPE_A2_ROADKLOTH_V1,
               const int32_t& child_size = 0, char** child_dataptr = NULL)
        : C3_SixDoF (DBC, name, otype, sizeof(kogmo_rtdb_subobj_a2_roadkloth_t) +
               child_size, (char**)&objroadkloth_p)
      {
        // Pass a Pointer pointing right after the base data to our child 
        if ( child_size && child_dataptr != NULL )
          *child_dataptr = (char*)objroadkloth_p + sizeof (kogmo_rtdb_subobj_a2_roadkloth_t);

        // Init Data
        objroadkloth_p->n = 0;
        // ...
      }

    void setKlothSeg (const int& n, const float& l, const float& c0,
                      const float& c1, const float& b0, const float& b1)
      {
        if ( n > objroadkloth_p -> n || n < 1)
          throw DBError(-KOGMO_RTDB_ERR_INVALID);
        objroadkloth_p -> l [n-1] = l;
        objroadkloth_p -> c0[n-1] = c0;
        objroadkloth_p -> c1[n-1] = c1;
        objroadkloth_p -> b0[n-1] = b0;
        objroadkloth_p -> b1[n-1] = b1;
      }

    void setY (const float& y)
      {
        objroadkloth_p -> y = y;
      }

    void setPhi (const float& phi)
      {
        objroadkloth_p -> phi = phi;
      }

    void setNumKlothSegs (const int& n)
      {
        if ( n >= A2_ROADKLOTH_SEGMAX )
          throw DBError(-KOGMO_RTDB_ERR_INVALID);
        objroadkloth_p->n = n;
      }

    int getNumKlothSegs ()
      {
        return objroadkloth_p->n;
      }

    float getY ()
      {
        return objroadkloth_p -> y;
      }

    float getPhi ()
      {
        return objroadkloth_p -> phi;
      }

    float getKlothSegL (const int& n)
      {
        if ( n > objroadkloth_p -> n || n < 1)
          throw DBError(-KOGMO_RTDB_ERR_INVALID);
        return objroadkloth_p -> l [n-1];
      }

    float getKlothSegC0 (const int& n)
      {
        if ( n > objroadkloth_p -> n || n < 1)
          throw DBError(-KOGMO_RTDB_ERR_INVALID);
        return objroadkloth_p ->c0 [n-1];
      }

    float getKlothSegC1 (const int& n)
      {
        if ( n > objroadkloth_p -> n || n < 1)
          throw DBError(-KOGMO_RTDB_ERR_INVALID);
        return objroadkloth_p -> c1 [n-1];
      }

    float getKlothSegB0 (const int& n)
      {
        if ( n > objroadkloth_p -> n || n < 1)
          throw DBError(-KOGMO_RTDB_ERR_INVALID);
        return objroadkloth_p -> b0 [n-1];
      }

    float getKlothSegB1 (const int& n)
      {
        if ( n > objroadkloth_p -> n || n < 1)
          throw DBError(-KOGMO_RTDB_ERR_INVALID);
        return objroadkloth_p -> b1 [n-1];
      }

    std::string dump (void) const
      {
        std::ostringstream ostr;
        ostr << "* Road Model Parameters:" << std::endl;
        for (int i=0; i < objroadkloth_p -> n; i++)
          {
            ostr << "Segment " <<i <<":"
                     << " l="  << objroadkloth_p -> l  [i]
                     << " c0=" << objroadkloth_p -> c0 [i]
                     << " c1=" << objroadkloth_p -> c1 [i]
                     << " b0=" << objroadkloth_p -> b0 [i]
                     << " b1=" << objroadkloth_p -> b1 [i] << std::endl;
          }
          ostr       << " y="  << objroadkloth_p -> y
                     << " phi=" << objroadkloth_p -> phi << std::endl;
        return C3_SixDoF::dump() + ostr.str();
      };
};

// for an example: see kogmo_rtdb_obj_a2_classes.hxx: template A2_Image
template < int width = 1024, int height = 768, int channels = 1 > class A2_Image
         : public RTDBObj_T < kogmo_rtdb_subobj_a2_image_t, KOGMO_RTDB_OBJTYPE_A2_IMAGE, RTDBObj, width*height*channels >
{
  public:
    A2_Image(class RTDBConn& DBC, const char* name = "")
            : RTDBObj_T < kogmo_rtdb_subobj_a2_image_t, KOGMO_RTDB_OBJTYPE_A2_IMAGE, RTDBObj, width*height*channels > (DBC, name)
      {
        this->subobj_p->width=width;
        this->subobj_p->height=height;
        this->subobj_p->depth=A2_IMAGE_DEPTH_8U;
        this->subobj_p->channels=channels;
        this->subobj_p->colormodell=0;
        this->subobj_p->widthstep=abs(width)*channels;
      }
    unsigned char * getData(void)
      {
        return &this->subobj_p->data[0];
      }
    int getWidth(void)
      {
        return abs(this->subobj_p->width);
      }
    int getHeight(void)
      {
        return abs(this->subobj_p->height);
      }
    int getChannels(void)
      {
        return this->subobj_p->channels;
      }
    int getDepth(void)
      {
        return this->subobj_p->depth;
      }
    int getWidthstep(void)
      {
        return this->subobj_p->widthstep;
      }
    int getColormodel(void)
      {
        return this->subobj_p->colormodel;
      }
    int getSize(void)
      {
        return getWidthstep() * getHeight();
      }
    std::string dump(void) const
    {
        std::ostringstream ostr;
        ostr << "* Image:" << std::endl
             << "Size (width x height) [pixel]: " << this->subobj_p->width << " x " << this->subobj_p->height << std::endl
             << "Width-Step: " << this->subobj_p->widthstep << std::endl
             << "Color Channels [1 for greyscale, 3 for color]: " << this->subobj_p->channels << std::endl
             << "Color Modell [0=rgb interleaved, 1=yuv411]: " << this->subobj_p->colormodell<< std::endl
             << "Depth [8 = 8 Bits/Pixel, see source]: " << this->subobj_p->depth << std::endl;
        return RTDBObj::dump() + ostr.str();
      }; 
};

//old, but generic: typedef RTDBObj_T < kogmo_rtdb_subobj_a2_image_t, KOGMO_RTDB_OBJTYPE_A2_IMAGE, RTDBObj,  640*480 > A2_Image_Gray640x480;
//better:
typedef A2_Image <  640, 480, 1 > A2_Image_Gray640x480;
typedef A2_Image < 1024, 768, 1 > A2_Image_Gray1024x768;

typedef A2_Image < 768, 400, 1 > A2_Image_Gray768x400;
typedef A2_Image < 748, 400, 1 > A2_Image_Gray748x400;

typedef A2_Image < 1024, 768, 3 > A2_Image_RGB1024x768;

typedef A2_Image_RGB1024x768 A2_BiggestImage; // groesstes zu erwartendes bild (bei bedarf vergroessern)

class A2_AnyImage : public A2_BiggestImage
{
  public:
    A2_AnyImage(class RTDBConn& DBC, const char* name = "") : A2_BiggestImage (DBC, name)
      {
        setMinSize( - getSize() );
      }
};


typedef RTDBObj_T < kogmo_rtdb_subobj_a2_imageannot_t, KOGMO_RTDB_OBJTYPE_A2_IMAGEANNOT, RTDBObj > A2_ImageAnnot_T;
class A2_ImageAnnot : public A2_ImageAnnot_T
{
  public:
    A2_ImageAnnot(class RTDBConn& DBC, const char* name = "") : A2_ImageAnnot_T (DBC, name)
      {
        subobj_p->numlines = 0;
        setMinSize( - sizeof(annotline_t) * A2_IMAGEANNOT_MAXLINES );
      }

    void deleteLines (void)
      {
        subobj_p->numlines = 0;
        calcSize();
      }

    void calcSize (void)
      {
        setSize( - sizeof(annotline_t) * ( A2_IMAGEANNOT_MAXLINES - subobj_p->numlines ) );
      }

    void addPoint ( int x1, int y1,
                   unsigned int color = 1, unsigned int width = 1, unsigned int flags = 0 )
      {
        addLine ( x1, y1, x1, y1, color, width, flags);
      }

    void addLine ( int x1, int y1, int x2 = -1, int y2 = -1,
                   unsigned int color = 1, unsigned int width = 1, unsigned int flags = 0 )
      {
        if ( subobj_p->numlines >= A2_IMAGEANNOT_MAXLINES )
          throw DBError("Too many Annotation Lines, try to increase A2_IMAGEANNOT_MAXLINES");

        annotline_t *a = &(subobj_p->line[subobj_p->numlines]);
        a->x1 = (unsigned int) x1;
        a->y1 = (unsigned int) y1;

        if ( x2==-1 && y2==-1 ) // no x2/y2 given -> draw only a point (x2=x1, y2=y1)
          {
            a->x2 = (unsigned int) x1;
            a->y2 = (unsigned int) y1;
          }
        else
          {
            a->x2 = (unsigned int) x2;
            a->y2 = (unsigned int) y2;
          }
        a->color = color;
        a->width = width;
        a->flags = flags;

        subobj_p->numlines++;
        calcSize();
      };

    int getNumLines (void) const
      {
        return subobj_p->numlines;
      }

    // diese Methode ist kein schoenes C++, man sollte ein annotline-objekt definieren..
    void getLine (const int& nr, annotline_t *a) const
      {
        if ( nr > getNumLines() )
          throw DBError("A annotation line with this number does not exist");
        *a = subobj_p->line[nr];
      }

    std::string dump(void) const
    {
        std::ostringstream ostr;
        ostr << "* Annotations :" << std::endl
             << "flags: " << subobj_p->flags << std::endl;
        for (int i = 0; i < subobj_p->numlines; i++ )
          {
             ostr << i+1 << ".: "
                  << subobj_p->line[i].x1 << "," << subobj_p->line[i].y1 << "-"
                  << subobj_p->line[i].x2 << "," << subobj_p->line[i].y2
                  << " color:" << (int)subobj_p->line[i].color << std::endl;
          }
        return RTDBObj::dump() + ostr.str();
      }; 
};



typedef RTDBObj_T < kogmo_rtdb_subobj_a2_camera_t, KOGMO_RTDB_OBJTYPE_A2_CAMERA, RTDBObj > A2_Camera_T;
class A2_Camera : public A2_Camera_T
{
  public:
    A2_Camera(class RTDBConn& DBC, const char* name = "") : A2_Camera_T (DBC, name)
      {
      }

    std::string dump(void) const
    {
        std::ostringstream ostr;
        char cstr[100];
        snprintf (cstr, sizeof(cstr), "0x%llX", subobj_p->euid );
        ostr << "* Camera Info:" << std::endl
             << "EUID:        " << cstr << std::endl
             << "Description: " << subobj_p->descr << std::endl;
        return RTDBObj::dump() + ostr.str();
      }; 
};


typedef RTDBObj_T < kogmo_rtdb_subobj_a2_intcal_t, KOGMO_RTDB_OBJTYPE_A2_INTCAL, RTDBObj > A2_IntCal_T;
class A2_IntCal : public A2_IntCal_T
{
  public:
    A2_IntCal(class RTDBConn& DBC, const char* name = "") : A2_IntCal_T (DBC, name)
      {
      }

    std::string dump(void) const
    {
        std::ostringstream ostr;
        ostr << "* Intrinsic Camera Calibration Data:" << std::endl
             << "Quality (0:invalid):       " << subobj_p->quality << std::endl
             << "Sensorsize DX, DY [pixel]: " << subobj_p->dx << ", " << subobj_p->dy << std::endl
             << "Pixelsize SX, SY [m]:      " << subobj_p->sx << ", " << subobj_p->sy << std::endl
             << "Center CX, CY [pixel]:     " << subobj_p->cx << ", " << subobj_p->cy << std::endl
             << "Focal lenght f [m]:        " << subobj_p->f << std::endl
             << "Distorsion k (0: unset):   " << subobj_p->k << std::endl;
        return RTDBObj::dump() + ostr.str();
     }; 
};

typedef RTDBObj_T < kogmo_rtdb_subobj_a2_extcal_t, KOGMO_RTDB_OBJTYPE_A2_EXTCAL, C3_SixDoF > A2_ExtCal_T;
class A2_ExtCal : public A2_ExtCal_T
{
  public:
    A2_ExtCal(class RTDBConn& DBC, const char* name = "") : A2_ExtCal_T (DBC, name)
      {
      }

    std::string dump(void) const
    {
        std::ostringstream ostr;
        ostr << "* Extrinsic Camera Calibration Data:" << std::endl
             << "Quality (0:invalid): " << subobj_p->quality << std::endl;
        return C3_SixDoF::dump() + ostr.str();
      }; 
};






/*@}*/
}; /* namespace KogniMobil */

#endif /* KOGMO_RTDB_OBJ_A2_CLASSES_HXX */
