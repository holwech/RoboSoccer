#ifndef SYMRAD_H_
#define SYMRAD_H_

#include "settings.h"
#include "rmath.h"
#include <iostream>  //for printing

/*
 * angle class which makes sure that the angles are in the range of -PI and PI.
 * Another feature of this class is that you can compare two angles
 * and that you have simple arithmetic operations like minus and plus.
 *
 * It is also possible to convert between radian and degree and it is possible to
 * use the stream output << with angles (they are output as "DEG°")
 *
 */

class SymRad {
  protected:
    fType mAngle;

  public:

    explicit SymRad(const fType angle  = 0.0) :
      mAngle(toSymRad(angle)){};

    SymRad(const SymRad &angle) :
      mAngle(angle.toFloat()){};


    fType toFloat() const {
      return mAngle;
    }

    fType abs() const {
      return fabs(mAngle);
    }

    fType deg() const {
      return (toPosDeg(mAngle));
    }

    fType symdeg() const {
          return (toSymDeg(mAngle));
    }

    void set(fType angle) {
      mAngle = toSymRad(angle);
    }

    SymRad& operator=(const fType op2) {
      mAngle = toSymRad(op2);
      return *this;
    }

    SymRad& operator=(const SymRad op2) {
      mAngle = op2.toFloat();
      return *this;
    }

    SymRad& operator+=(const SymRad op2) {
      mAngle = toSymRad(mAngle + op2.toFloat());
      return *this;
    }

    SymRad& operator-=(const SymRad op2) {
      mAngle = toSymRad(mAngle - op2.toFloat());
      return *this;
    }

    SymRad& operator+=(const fType op2) {
      mAngle = toSymRad(mAngle + op2);
      return *this;
    }

    SymRad& operator-=(const fType op2) {
      mAngle = toSymRad(mAngle - op2);
      return *this;
    }

    const SymRad operator-(void) {
      return SymRad(-mAngle);
    }
};

/**
 *  @brief add two Angles and return the result as a temporary sym rad
 */
inline const SymRad operator+(const SymRad op1, const SymRad op2) {
  return SymRad(op1.toFloat() + op2.toFloat());
}

/**
 *  @brief add a fType to an Angle and return the result
 *  as a temporary sym rad (commutative)
 */
inline const SymRad operator+(const SymRad op1, const fType op2) {
  return SymRad(op1.toFloat() + op2);
}
inline const SymRad operator+(const fType op1, const SymRad op2) {
  return (op2 + op1);
}

/**
 *  @brief multiply an Angle with a fType and return the result
 *   as a temporary sym rad (commutative)
 */
inline const SymRad operator*(const SymRad op1, const fType op2) {
  return SymRad(toSymRad(op1.toFloat() * op2));
}
inline const SymRad operator*(const fType op1, const SymRad op2) {
  return (op2*op1);
}

/**
 *  @brief divide an Angle by a fType and return the result as a temporary sym rad
 */
inline const SymRad operator/(const SymRad op1, const fType op2) {
  return SymRad(toSymRad(op1.toFloat() / op2));
}

/**
 *  @brief sub two Angles and return the result as a temporary sym rad
 */
inline const SymRad operator-(const SymRad op1, const SymRad op2) {
  return SymRad(op1.toFloat() - op2.toFloat());
}

/**
 *  @brief sub a fType from an Angle and return the result as a temporary sym rad
 */
inline const SymRad operator-(const SymRad op1, const fType op2) {
  return SymRad(op1.toFloat() - op2);
}

/**
 *  @brief Compare two Angles or an Angle and a fType.
 *  Equality is checked with isZero.
 */
inline bool operator==(const SymRad op1, const SymRad op2) {
  return (isZero(op1.toFloat() - op2.toFloat()));
}
inline bool operator==(const SymRad op1, const fType op2) {
  return (op1 == SymRad(op2));
}
inline bool operator==(const fType op1, const SymRad op2) {
  return (SymRad(op2) == op1);
}

inline bool operator!=(const SymRad op1, const SymRad op2) {
  return !(op1==op2);
}
inline bool operator!=(const SymRad op1, const fType op2) {
  return !(op1 == SymRad(op2));
}
inline bool operator!=(const fType op1, const SymRad op2) {
  return !(SymRad(op2) == op1);
}

inline bool operator>(const SymRad op1, const SymRad op2) {
  return (op1.toFloat() > op2.toFloat());
}
inline bool operator>(const SymRad op1, const fType op2) {
  return (op1 > SymRad(op2));
}
inline bool operator>(const fType op1, const SymRad op2) {
  return (SymRad(op1) > op2);
}

inline bool operator>=(const SymRad op1, const SymRad op2) {
  return (op1.toFloat() >= op2.toFloat());
}
inline bool operator>=(const SymRad op1, const fType op2) {
  return (op1 >= SymRad(op2));
}

inline bool operator>=(const fType op1, const SymRad op2) {
  return (SymRad(op1) >= op2);
}

inline bool operator<(const SymRad op1, const SymRad op2) {
  return (op1.toFloat() < op2.toFloat());
}
inline bool operator<(const SymRad op1, const fType op2) {
  return (op1 < SymRad(op2));
}
inline bool operator<(const fType op1, const SymRad op2) {
  return (SymRad(op1) < op2);
}

inline bool operator<=(const SymRad op1, const SymRad op2) {
  return (op1.toFloat() <= op2.toFloat());
}
inline bool operator<=(const SymRad op1, const fType op2) {
  return (op1 <= SymRad(op2));
}
inline bool operator<=(const fType op1, const SymRad op2) {
  return (SymRad(op1) <= op2);
}

inline fType cos(SymRad val){
  return cos(val.toFloat());
}

inline fType sin(SymRad val){
  return sin(val.toFloat());
}

inline fType tan(SymRad val){
  return tan(val.toFloat());
}

inline fType abs (SymRad val) {
  return val.abs();
}

//template<typename U>
//U& operator<<(U& out, const SymRad& angle)
//{
//  return out << angle.deg();
//}

inline std::ostream& operator<<(std::ostream& out, const SymRad& angle){
  return out << angle.deg();
}

inline std::istream& operator>>(std::istream& in, SymRad& angle) {
  fType temp;
  in >> temp;
  angle = temp;
  return in;
}


#endif /* ANGLE_H_ */
