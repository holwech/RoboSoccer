// #include "Euler.h"
// Created on: 30.03.2009
// Author    : Martin Schaefer (martin.schaefer_at_tum.de)
/* Description:
 */

#ifndef EULER_H_
#define EULER_H_

// SYSTEM INCLUDES
// PROJECT INCLUDES
// LOCAL INCLUDES

/*Sort Class as:
 public:
 //LIFECYCLE
 //OPERATORS
 //OPERATIONS
 //ACCESS
 //INQUIRY
 protected:
 private:
 */

#include "matrix.h"
#include "vector3d.h"

// M(f->g (Mgf), Psi = 0)
/*     cos(mTheta)   sin(mPhi)*sin(mTheta)   cos(mPhi)*sin(mTheta)
 *          0        cos(mPhi)              -sin(mPhi)
 *    -sin(mTheta)   sin(mPhi)*cos(mTheta)  cos(mPhi)*cos(mTheta)
 */


// M(g->f (Mfg), Psi = 0)
/*     cos(mTheta)              0            -sin(mTheta)
 *     sin(mPhi)sin(mTheta)     cos(mPhi)     sin(mPhi)*cos(mTheta)
 *    cos(mPhi)*sin(mTheta)   -sin(mPhi)     cos(mPhi)*cos(mTheta)
 */


//opt make non class functions as well
class Euler: public Matrix3d {
  public:
    Euler();
    virtual ~Euler();

    Matrix3d& upFtoG(const Vector3df& at) {
      if (!mFtoG || at != mAt)
      {
        mAt = at;
        mFtoG = true;

        fType cosPhi(cos(mAt.x()));
        fType cosTheta(cos(mAt.y()));

        fType sinPhi(sin(mAt.x()));
        fType sinTheta(sin(mAt.y()));

        operator()(0, 0) = cosTheta;
        operator()(0, 1) = sinPhi * sinTheta;
        operator()(0, 2) = cosPhi * sinTheta;
        operator()(1, 0) = 0;
        operator()(1, 1) = cosPhi;
        operator()(1, 2) = -sinPhi;
        operator()(2, 0) = -sinTheta;
        operator()(2, 1) = sinPhi * cosTheta;
        operator()(2, 2) = cosPhi * cosTheta;
      }
      return *this;
    }


    Matrix3d& upGtoF(Vector3df& at) {
      if (mFtoG || at != mAt)
      {
        mAt = at;
        mFtoG = false;

        fType cosPhi(cos(mAt.x()));
        fType cosTheta(cos(mAt.y()));

        fType sinPhi(sin(mAt.x()));
        fType sinTheta(sin(mAt.y()));

        operator()(0, 0) = cosTheta;
        operator()(0, 1) = 0;
        operator()(0, 2) = -sinTheta;
        operator()(1, 0) = sinPhi * sinTheta;
        operator()(1, 1) = cosPhi;
        operator()(1, 2) = sinPhi * cosTheta;
        operator()(2, 0) = cosPhi * sinTheta;
        operator()(2, 1) = -sinPhi;
        operator()(2, 2) = cosPhi * cosTheta;

      }
      return *this;
    }

  protected:
    bool mFtoG;
    Vector3df mAt;
};









// INLINE METHODS
// EXTERNAL REFERENCES

#endif /* EULER_H_ */
