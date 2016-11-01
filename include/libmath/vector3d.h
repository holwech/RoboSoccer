// #include "vector3d.h"
// Created on: 26.03.2009
// Author    : Martin Schaefer (martin.schaefer_at_tum.de)
/* Description:
 */

#ifndef VECTOR3D_H_
#define VECTOR3D_H_

#include <cmath>     //math functions
#include <sstream>
#include <stdexcept>

#include "settings.h"
#include "symrad.h"
#include "matrix.h"

//#include <algorithm> //standard stl algos e.g. swap
//#include <stdexcept> //exception handling
//#include <cstring>   //for memcpy
//#include <iostream>  //for printing
//
//
//
//
//
//
//#include "rmath.h"
//#include "symrad.h"

template<typename T> class Vector3d;

typedef Vector3d<fType> Vector3df;
typedef Vector3d<SymRad> Vector3dRad;

template<typename T>
class Vector3d {
  protected:
    T mX;
    T mY;
    T mZ;

  public:
    /**
     *  @brief Default constructor, components can be specified.
     *  @param x,y,z Values for each vector component.
     *
     *  If no parameters are passed to this constructor, a zero vector
     *  is constructed. Otherwise each component can be specified for
     *  construction.
     */
    explicit Vector3d(const T& val = 0.0) :
      mX(val), mY(val), mZ(val) {
    }

    explicit Vector3d(const T& x, const T& y, const T& z = 0.0) :
      mX(x), mY(y), mZ(z) {
    }

    template<typename T2>
    explicit Vector3d(const Vector3d<T2>& other);

    /**
     *  @brief Read vector components from a @c std::istream.
     */
    std::istream& scan(std::istream& in) {
      in >> mX >> mY >> mZ;
      return in;
    }

    /**
     *  @brief print vector components on a @c std::ostream.
     *  @param out @c std::ostream where the information is printed to.
     *  @param indent Number of indentations before output.
     */
    template<typename U>
    U& print(U& out) const {
      out << (isZero(mX) ? T(0) : mX) << "\t" << (isZero(mY) ? T(0) : mY) << "\t"
          << (isZero(mZ) ? T(0) : mZ);
      return out;
    }

    // INLINE METHODS ----------------------------------------------------------

    /**
     *  @brief get the number of vector elements.
     */
    int size() const {
      return 3;
    }

    /**
     *  @brief Returns a const ref of the vector, helpful for objects that
     *  inherit from vector
     */
    const Vector3d<T>& getVect() const {
      return *this;
    }

    //    template<typename T2>
    //    Vector3d<T2> getVect() const {
    //      Vector3d<T2>(mX,mY,mZ);
    //    }


    /**
     *  @brief Returns a ref of the vector, helpful for objects that
     *  inherit from vector
     */
    Vector3d<T>& vect() {
      return *this;
    }

    /**
     *  @brief Returns a read reference to the x component.
     */
    const T& getX() const {
      return mX;
    }

    /**
     *  @brief Returns a read reference to the y component.
     */
    const T& getY() const {
      return mY;
    }

    /**
     *  @brief Returns a read reference to the z component.
     */
    const T& getZ() const {
      return mZ;
    }
    /**
     *  @brief Returns a read/write ref of the x component.
     */

    T& x() {
      return mX;
    }

    /**
     *  @brief Returns a read/write ref of the y component.
     */
    T& y() {
      return mY;
    }

    /**
     *  @brief Returns a read/write ref of the z component.
     */
    T& z() {
      return mZ;
    }

    /**
     *  @brief Returns a copy of the x component.
     */
    const T getPhi() const {
      return toSymRad(mX);
    }

    /**
     *  @brief Returns a read/write reference to the x component.
     */
    T& phi() {
      return mX;
    }

    /**
     *  @brief Returns a copy of the y component.
     */
    const T getTheta() const {
      return toSymRad(mY);
    }

    /**
     *  @brief Returns a read/write reference to the y component.
     */
    T& theta() {
       return mY;
    }

    /**
     *  @brief Returns a copy of the z component.
     */
    const T getPsi() const {
      return toSymRad(mZ);
    }

    /**
     *  @brief Returns a read/write reference to the z component.
     */
    T& psi() {
      return mZ;
    }

    //    /**
    //     *  @brief Returns a copy of the x component.
    //     */
    //    T x() const {
    //      return getX();
    //    }
    //
    //    /**
    //     *  @brief Returns a ref of the y component.
    //     */
    //    T y() const {
    //      return getY();
    //    }
    //
    //    /**
    //     *  @brief Returns a ref of the z component.
    //     */
    //    T z() const {
    //      return getZ();
    //    }


    template<typename T2>
    Vector3d<T>& max(const Vector3d<T2>& right) {
      mX = std::max(mX, right.mX);
      mY = std::max(mY, right.mY);
      mZ = std::max(mX, right.mZ);
      return *this;
    }

    template<typename T2>
    Vector3d<T>& min(const Vector3d<T2>& right) {
      mX = std::min(mX, right.mX);
      mY = std::min(mY, right.mY);
      mZ = std::min(mX, right.mZ);
      return *this;
    }

    /**
     *  @brief Returns the maximum of the absolute element values
     *  good for sensor limit checking when 3d sensors are used
     */
    const T getMaxElAbs() {
      return max3d(fabs(mX), fabs(mY), fabs(mZ));
    }

    const T& getMinElAbs() {
      return min3d(fabs(mX), fabs(mY), fabs(mZ));
    }

    Vector3d<int> getSign() const {
      return Vector3d<int> (sign(mX), sign(mY), sign(mZ));
    }

    Vector3df getDeg() const;

    /**
     *  @brief Calculates and returns the length of the vector.
     */
    T getNorm() const {
      return sqrt(mX * mX + mY * mY + mZ * mZ);
    }

    T getNormSquare() const {
      return (mX * mX + mY * mY + mZ * mZ);
    }

    /**
     *  @brief Calculates and returns the length of the vector.
     */
    T getSizeEq() const {
      return getNormSquare();
    }

    /**
     *  @brief normalize this vector.
     *
     *  This operation only succeeds if the length
     *  of the vector is not zero, otherwise an assertion is raised.
     */
    Vector3d<T>& normalize() {
      double n = getNorm();
      assert(n> 0.0);
      *this *= 1.0 / n;
      return *this;
    }

    /**
     *  @brief Calculate the cross product.
     *
     *  Calculates and returns (as a temporary Vector3D object) the
     *  cross product with the provided vector.
     */
    template<typename T2>
    const Vector3d<T> cross(const Vector3d<T2>& right) {
      return Vector3d(this->mY * right.getZ() - this->mZ * right.getY(), this->mZ
          * right.getX() - this->mX * right.getZ(), this->mX * right.getY() - this->mY
          * right.getX());
    }

    /**
     *  @brief Calculate the scalar product.
     *
     *  Calculates and returns the scalar product with the provided vector
     **/
    template<typename T2>
    T scalar(const Vector3d<T2>& right) {
      return ((this->mX * right.getX()) + (this->mY * right.getY()) + (this->mZ
          * right.getZ()));
    }

    // INLINE OPERATORS---------------------------------------------------------

    /**
     *  @brief Assign from anything that supports subscripting.
     *  Assign to a %Vector3D from any c-style array.
     *  The array data type must be assignable
     *  to @c T and the array must have at least three elements.
     */
    template<typename T2>
    Vector3d<T>& operator=(const T2* values) {
      mX = values[0];
      mY = values[1];
      mZ = values[2];
      return *this;
    }

    /**
     *  @brief Add another vector to this vector.
     */
    template<typename T2>
    Vector3d<T>& operator+=(const Vector3d<T2>& v) {
      mX += v.getX();
      mY += v.getY();
      mZ += v.getZ();
      return *this;
    }

    /**
     *  @brief Substract another vector from this vector.
     */
    template<typename T2>
    Vector3d<T>& operator-=(const Vector3d<T2>& v) {
      mX -= v.mX;
      mY -= v.mY;
      mZ -= v.mZ;
      return *this;
    }

    /**
     *  @brief Scale this vector by a scalar.
     */
    template<typename T2>
    Vector3d<T>& operator*=(T2 scalar) {
      mX *= scalar;
      mY *= scalar;
      mZ *= scalar;
      return *this;
    }

    /**
     *  @brief Calculate the product by element
     *
     *  Calculates the product of this vector and the provided
     *  vector by element (as in Matlab). The result is saved in this vector.
     */
    template<typename T2>
    Vector3d<T>& operator*=(const Vector3d<T2>& v) {
      mX *= v.mX;
      mY *= v.mY;
      mZ *= v.mZ;
      return *this;
    }

    /** @brief Calculate the division by element
     *
     *  Divides this vector with the provided vector by element
     *  (as in Matlab). The result is saved in this vector.
     *  an assert is raised if one component of the divisor is zero
     */
    template<typename T2>
    Vector3d<T>& operator/=(const Vector3d<T2>& v) {
      assert(v.mX> 0.0);
      assert(v.mY> 0.0);
      assert(v.mZ> 0.0);
      mX /= v.mX;
      mY /= v.mY;
      mZ /= v.mZ;
      return *this;
    }

    /** @brief Calculate the division by a scalar (T)
     *  Divides this vector with the provided scalar.
     *  The result is saved in this vector.
     *  an assert is raised if the divisor is zero
     */
    template<typename T2>
    Vector3d<T>& operator/=(const T2& div) {
      assert(div> 0.0);
      mX /= div;
      mY /= div;
      mZ /= div;
      return *this;
    }

    /**@brief Subscript access to the data contained in the %Vector3D.
     *  @param index The index of the requested element.
     *  @return Read/write reference to data.
     *
     *  This operator allows for easy data access. Note that data access
     *  to elements beyond index 2 will cause a std::range_error exception.
     */
    T& operator[](const unsigned int index) throw (std::range_error) {
      switch (index) {
        case 0:
          return mX;
          break;
        case 1:
          return mY;
          break;
        case 2:
          return mZ;
          break;
        default: {
          std::ostringstream error;
          error << "Vector3D:unallowed data access at index " << index;
          throw std::range_error(error.str());
        }
      }
      return mX; // we never reach this point
    }

    /**
     *  @brief Subscript access to the data contained in the %Vector3D.
     *  @param index The index of the requested element.
     *  @return Read-only reference to data.
     *
     *  This operator allows for easy data access. Note that data access
     *  to elements beyond index 2 will cause a std::range_error exception.
     */
    const T& operator[](const unsigned int index) const throw (std::range_error) {
      switch (index) {
        case 0:
          return mX;
          break;
        case 1:
          return mY;
          break;
        case 2:
          return mZ;
          break;
        default: {
          std::ostringstream error;
          error << "Vector3D: unallowed data access at index " << index;
          throw std::range_error(error.str());
        }
      }
      return mX; // we never reach this point
    }
};


const Vector3df ZERO_VEC(0.0);

//MEMBER FUNCTIONS ----------------------------------------------------------

//Specialisation in cpp
template<typename T>
template<typename T2>
Vector3d<T>::Vector3d(const Vector3d<T2>& other) :
  mX(other.getX()), mY(other.getY()), mZ(other.getZ()) {
}

template<>
template<>
Vector3d<fType>::Vector3d(const Vector3d<SymRad>& other);

//Specialisation in cpp
template<typename T>
Vector3df Vector3d<T>::getDeg() const {
  return Vector3df(toPosDeg(mX), toPosDeg(mY), toPosDeg(mZ));
}

template<>
Vector3df Vector3d<SymRad>::getDeg() const;


// INLINE NON MEMBER ----------------------------------------------------------

/**

 *  @brief Compare two Vector3D for equalness.
 *
 *  Two Vector3D are considered equal if all components
 *  are equal. Component equality is checked with isZero.
 */
template<typename T>
inline bool operator==(const Vector3d<T>& left, const Vector3d<T>& right) {
  return (isZero(left.getX() - right.getX()) && isZero(left.getY() - right.getY())
      && isZero(left.getZ() - right.getZ()));
}

/**

 *  @brief Compare two Vector3D for differences.
 *
 *  Two Vector3D are different if one component is different.
 *  Component difference is checked with isZero.
 */
template<typename T>
inline bool operator!=(const Vector3d<T>& left, const Vector3d<T>& right) {
  return !(left == right);
}

/**

 *  @brief Compare a Vector3D and a @c T value for equality.
 *
 *  A Vector3D is considered equal to a @c T value if
 *  all components are equal this value. Each component is
 *  checked by isZero. Useful to test against zero.
 */
template<typename T>
inline bool operator==(const Vector3d<T>& left, T right) {
  return (isZero(left.getX() - right) && isZero(left.getY() - right)
      && isZero(left.getZ() - right));
}

/**

 *  @brief Compare a @c T value and a Vector3D for equality.
 *
 *  A Vector3D is considered equal to a @c T value if
 *  all components are equal this value. Each component is
 *  checked by isZero. Useful to test against zero.
 */
template<typename T>
inline bool operator==(T left, const Vector3d<T>& right) {
  return right == left;
}

/**

 *  @brief Compare a Vector3D and a @c T value for difference.
 *
 *  A Vector3D is considered different from a @c T value if
 *  one component is different from this value. Each component
 *  is checked by isZero.
 */
template<typename T>
inline bool operator!=(const Vector3d<T>& left, T right) {
  return !(left == right);
}

/**

 *  @brief Compare a @c T value and a Vector3D for difference.
 *
 *  A Vector3D is considered different from a @c T value if
 *  one component is different from this value. Each component
 *  is checked by isZero.
 */
template<typename T>
inline bool operator!=(T left, const Vector3d<T>& right) {
  return !(right == left);
}

/**

 *  @brief Add two Vector3D.
 *
 *  Every component of each vector is added and the result
 *  is returned in a temporary Vector3D object.
 */
template<typename T>
inline const Vector3d<T> operator+(const Vector3d<T>& left, const Vector3d<T>& right) {
  return Vector3d<T> (left.getX() + right.getX(), left.getY() + right.getY(),
                      left.getZ() + right.getZ());
}

/**

 *  @brief Substract two Vector3D.
 *
 *  For every component the difference is calculated and returned
 *  in a temporary Vector3D object.
 */
template<typename T>
inline const Vector3d<T> operator-(const Vector3d<T>& left, const Vector3d<T>& right) {
  return Vector3d<T> (left.getX() - right.getX(), left.getY() - right.getY(),
                      left.getZ() - right.getZ());
}

/**
 *  @brief Scale a Vector3D.
 *
 *  Every component of the Vector3D is scaled and the result is
 *  returned in a temporary Vector3D object.
 */
template<typename T, typename U>
inline const Vector3d<T> operator*(const Vector3d<T>& left, U right) {
  return Vector3d<T> (left.getX() * right, left.getY() * right, left.getZ() * right);
}

template<typename T> //can't use another type here since conflicts with matrix
inline const Vector3d<T> operator*(T left, const Vector3d<T>& right) {
  return right * left;
}

/* @brief Multiply one vector by another elementwise
 */
template<typename T>
inline const Vector3d<T> operator*(const Vector3d<T>& left, const Vector3d<T>& right) {
  return Vector3d<T> (left.getX() * right.getX(), left.getY() * right.getY(),
                      left.getZ() * right.getZ());
}

/**
 *! Multiplikation einer Matrix3d mit einem Vector3D. Ergebnis ist wieder ein
 * Vector3D. Wenn die Matrix3d nicht 3 Zeilen und Spalten hat,
 * geht die Operation schief.
 * Es kann keine Referenz zurückgegeben werden,
 * da diese statisch angelegt werden müsste
 * global, damit jeder vom Operator profitieren kann, auch abgeleitete Klassen.
 */
template<typename T>
inline const Vector3d<T> operator*(const Matrix3d& matrix, const Vector3d<T>& vec) {
  //  assert(rows == 3);
  //  assert(cols == 3);
  fType x1 = matrix[0] * vec.getX() + matrix[3] * vec.getY() + matrix[6] * vec.getZ();
  fType x2 = matrix[1] * vec.getX() + matrix[4] * vec.getY() + matrix[7] * vec.getZ();
  fType x3 = matrix[2] * vec.getX() + matrix[5] * vec.getY() + matrix[8] * vec.getZ();
  return Vector3d<T> (x1, x2, x3);
}

/* @brief Divide one vector by a scalar
 * if the divisor equals zero an assertion is raised
 */
template<typename T, typename U>
inline const Vector3d<T> operator/(const Vector3d<T>& left, const U right) {
  assert(!isZero(right));
  return Vector3d<T> (left.getX() / right, left.getY() / right, left.getZ() / right);
}

/* @brief Divide one vector by another elementwise
 * if an element of the divisor equals zero an assertion is raised
 */
template<typename T>
inline const Vector3d<T> operator/(const Vector3d<T>& left, const Vector3d<T>& right) {
  assert(!isZero(right.getX()));
  assert(!isZero(right.getY()));
  assert(!isZero(right.getZ()));
  return Vector3d<T> (left.getX() / right.getX(), left.getY() / right.getY(),
                      left.getZ() / right.getZ());
}

/* @brief Compare two Vectors with each other using their lengths
 */
template<typename T>
inline bool operator <(const Vector3d<T>& left, const Vector3d<T>& right) {
  return left.getSizeEq() < right.getSizeEq();
}

/* @brief Compare two Vectors with each other using their lengths
 */
template<typename T>
inline bool operator <=(const Vector3d<T>& left, const Vector3d<T>& right) {
  return left.getSizeEq() <= right.getSizeEq();
}

/* @brief Compare two Vectors with each other using their lengths
 */
template<typename T>
inline bool operator >(const Vector3d<T>& left, const Vector3d<T>& right) {
  return left.getSizeEq() > right.getSizeEq();
}

/* @brief Compare two Vectors with each other using their lengths
 */
template<typename T>
inline bool operator >=(const Vector3d<T>& left, const Vector3d<T>& right) {
  return left.getSizeEq() >= right.getSizeEq();
}

/* @brief Sort two Vectors in an ascending order comparing their
 *  lenghts (e.g if a is longer than b they are switched)
 */
template<typename T>
inline void sortAsc(Vector3d<T>& left, Vector3d<T>& right) {
  if (left.getNormSquare() > right.getNormSquare()) {
    const Vector3d<T> temp(left);
    left = right;
    right = temp;
  }
}

template<typename T>
//template<typename T2>
Vector3d<T> min(const Vector3d<T>& left, const Vector3d<T>& right) {
  return (Vector3d<T> (min(left.getX(), right.getX()), min(left.getY(), right.getY()),
                       min(left.getZ(), right.getZ())));
}

template<typename T>
//template<typename T2>
Vector3d<T> max(const Vector3d<T>& left, const Vector3d<T>& right) {
  return (Vector3d<T> (max(left.getX(), right.getX()), max(left.getY(), right.getY()),
                       max(left.getZ(), right.getZ())));
}

/* @brief Sort two Pointers to Vectors in an ascending order comparing their
 *  lenghts (e.g if a is longer than b they are switched)
 */
template<typename T>
inline T* sortPtrsAsc(const Vector3d<T>* & pLeft, const Vector3d<T>* & pRight) {
  if (pLeft->getNormSquare() > pRight->getNormSquare()) {
    const T* pTemp(pLeft);
    pLeft = pRight;
    pRight = pTemp;
  }
}

//NON_MEMBER FUNCTIONS----------------------------------------------------------
template<typename T, typename U>
U& operator<<(U& out, const Vector3d<T>& vec) {
  return vec.print(out);
}

template<typename T>
std::istream& operator>>(std::istream& in, Vector3d<T>& vec) {
  return vec.scan(in);
}

#endif /* VECTOR3D_H_ */
