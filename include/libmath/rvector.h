// -*- C++ -*-
#ifndef INCLUDED_ROVI_RVECTOR_H
#define INCLUDED_ROVI_RVECTOR_H

/**
 *  @file rvector.h
 *
 *  Definition of Vector.
 */

#include "vector3d.h"
#include "settings.h"

/**
 *  @ingroup math
 *  @brief Generic fixed-size vector class.
 *
 *  This class is a generic vector of fixed size for (almost) arbitrary data types.
 *  The only requirement for the data type is the existence (or must be assignable)
 *  from '0'. The dimension of the vector must be known on compile time, i.e. it can
 *  not be changed on runtime.
 *
 *  Some small examples of use:
 *  - Create a four-dimensional vector with element type @c double: @code Vector<4,double> vec1; @endcode
 *  - Create a three-dimensional vector with element type @c int, read vector values from
 *    a C-style array:
 *    @code const int values [] = { 1, 2, 3 };
 Vector<3,int> vec2 (values); @endcode
 *  - Access data of the vector above: @code vec2 [2] = 0; @endcode
 *
 *  @author Jan Leupold
 */
template<unsigned int dim, typename T>
class Vector
{
public:
  typedef T      Type;
  typedef unsigned int size_type;

protected:
  /**
   *  The elements of the vector, stored as an unaligned array.
   */
  Type m_elements [dim];

public:
  /**
   *  @brief Default constructor with optional value assignment.
   *  @param default_value This value is assigned to every element of the vector.
   *
   *  Constructs a vector of fixed-size dimension (template parameter) and assigns
   *  an initial value to every element. Default value for this assignment is '0'
   *  (therefore the data type for every element must be assignable from '0').
   *
   *  @note This constructor is marked 'explicit' to avoid construction from a single
   *        integer value.
   */
  explicit Vector (const Type& default_value = 0)
  {
    clear (default_value);
  }

  /**
   *  @brief Construct from a C-style array of same data type.
   *
   *  The array passed as parameter must have a minimum length of at least
   *  the dimension of the vector. Out-of-bounds memory accesses are not
   *  checked here. This constructor is used when the data type of the array
   *  elements and the data type of the vector elements are equal.
   */
  explicit Vector (const Type* values)
  {
    memcpy (m_elements, values, dim * sizeof (Type));
  }

  /**
   *  @brief Construct from a C-style array of different data type.
   *
   *  The array passed as parameter must have a minimum length of at least
   *  the dimension of the vector. Out-of-bounds memory accesses are not
   *  checked here. This constructor is used when the data type of the array
   *  elements and the data type of the vector elements are not equal.
   */
  template<typename value2_t>
  explicit Vector (const value2_t* values)
  {
    for (unsigned int elem = 0; elem < dim; ++elem)
      m_elements [elem] = static_cast<Type> (values [elem]);
  }

  /**
   *  @brief Construct from a Vector3D.
   *
   *  The elements of the passed Vector3D are copied to the first three
   *  elements of the new constructed vector. This constructor 'assert's that
   *  its dimension is at least 3. All remaining vector elements are set to '0'.
   */
  explicit Vector (const Vector3df& vec)
  {
    assert (dim > 2);
    clear ();						    // first set all elements to '0'
    m_elements [0] = static_cast<Type> (vec.getX ());    // ... then copy the first three elements
    m_elements [1] = static_cast<Type> (vec.getY ());
    m_elements [2] = static_cast<Type> (vec.getZ ());
  }

  /**
   *  @brief Copy on init constructor in the case of equal data types.
   *
   *  This constructor is used if a %Vector of the same data type is copied
   *  to the new %Vector. The dimensions of the two vectors may differ, the
   *  maximum amount of data is copied (starting from the first element in each
   *  vector). Uncopied elements are set to '0'.
   */
  template<unsigned int dim2>
  explicit Vector (const Vector<dim2,Type>& other)
  {
    if (dim2 < dim)
      {
	clear (dim2, dim - 1);
	memcpy (m_elements, other.m_elements, dim2 * sizeof (Type));
      }
    else
      memcpy (m_elements, other.m_elements, dim * sizeof (Type));
  }

  /**
   *  @brief Copy on init constructor in the case of different data types.
   *
   *  This constructor is used if a %Vector of a different data type is copied
   *  to the new %Vector. The dimensions of the two vectors may differ, the
   *  maximum amount of data is copied (starting from the first element in each
   *  vector). Uncopied elements are set to '0'.
   */
  template<unsigned int dim2, typename value2_t>
  explicit Vector (const Vector<dim2,value2_t>& other)
  {
    if (dim2 < dim)
      {
	clear (dim2, dim - 1);
	for (unsigned int elem = 0; elem < dim2; ++elem)
	  m_elements [elem] = static_cast<Type> (other [elem]);
      }
    else
      for (unsigned int elem = 0; elem < dim; ++elem)
	m_elements [elem] = static_cast<Type> (other [elem]);
  }

  /**
   *  @brief Copy on assignment in case of equal data type.
   *
   *  This assignment operator is used if data is copied from a %Vector with
   *  the same data type as this vector. The dimensions of the two vectors may differ,
   *  the maximum amount of data is copied (starting from the first element in each
   *  vector). Uncopied elements are not modified.
   */
  template<unsigned int dim2>
  Vector&
  operator= (const Vector<dim2,Type>& other)
  {
    if (this != &other)
      {
	if (dim2 < dim)
	  memcpy (m_elements, other.m_elements, dim2 * sizeof (Type));
	else
	  memcpy (m_elements, other.m_elements, dim * sizeof (Type));
      }
    return *this;
  }

  /**
   *  @brief Copy on assignment in case of different data types.
   *
   *  This assignment operator is used if data is copied from a %Vector with
   *  different data type as this vector. The dimensionis of the two vectors may differ,
   *  the maximum amount of data is copied (starting from the first element in each
   *  vector). Uncopied elements are not modified.
   */
  template<unsigned int dim2,typename value2_t>
  Vector&
  operator= (const Vector<dim2,value2_t>& other)
  {
    // no need to check for equality ... the other vector has a different data type
    if (dim2 < dim)
      for (unsigned int elem = 0; elem < dim2; ++elem)
	m_elements [elem] = static_cast<Type> (other [elem]);
    else
      for (unsigned int elem = 0; elem < dim; ++elem)
	m_elements [elem] = static_cast<Type> (other [elem]);
    return *this;
  }

  /**
   *  @brief Copy on assignment from a C-style array of same data type.
   *
   *  The array from which values are copied must have a minimum length of
   *  at least the dimension of the vector. Out-of-bounds memory accesses are not
   *  checked here.
   */
  Vector&
  operator= (const Type* values)
  {
    memcpy (m_elements, values, dim * sizeof (Type));
    return *this;
  }

  /**
   *  @brief Copy on assignment from any C-style array.
   *
   *  The array from which values are copied must have a minimum length of
   *  at least the dimension of the vector. Out-of-bounds memory accesses are not
   *  checked here.
   */
  template<typename value2_t>
  Vector&
  operator= (const value2_t* values)
  {
    for (unsigned int elem = 0; elem < dim; ++elem)
      m_elements [elem] = static_cast<Type> (values [elem]);
    return *this;
  }

  /**
   *  @brief Subscript access to the data contained in the %Vector.
   *  @param index The index of the requested element.
   *  @return Read/write reference to data.
   *
   *  This operator allows for easy data access. Note that data access with this
   *  operator is range-checked with 'assert' only.
   */
  Type&
  operator[] (unsigned int index)
  {
    assert (index < dim);
    return m_elements [index];
  }

  /**
   *  @brief Subscript access to the data contained in the %Vector.
   *  @param index The index of the requested element.
   *  @return Real-only reference to data.
   *
   *  This operator allows for easy data access. Note that data access with this
   *  operator is range-checked by 'assert' only.
   */
  const Type&
  operator[] (unsigned int index) const
  {
    assert (index < dim);
    return m_elements [index];
  }

  /**
   *  @brief Calculate the cross product (only if dimension == 3).
   *
   *  Calculates the cross product of this vector and the provided
   *  vector. The result is saved in this vector. This method checks
   *  with 'assert' that the dimension of both vectors is three.
   */
  template<class value2_t>
  Vector&
  operator*= (const Vector<dim,value2_t>& other)
  {
    assert (dim == 3);
    Vector<dim,Type> temp (*this);
    m_elements [0] = temp.m_elements [1] * other [2] - temp.m_elements [2] * other [1];
    m_elements [1] = temp.m_elements [2] * other [0] - temp.m_elements [0] * other [2];
    m_elements [2] = temp.m_elements [0] * other [1] - temp.m_elements [1] * other [0];
    return *this;
  }

  /**
   *  @brief Scale every element of this vector by a scalar.
   */
  template<class value2_t>
  Vector& operator*= (const value2_t& mult)
  {
    for (unsigned int elem = 0; elem < dim; ++elem)
      m_elements [elem] *= mult;
    return *this;
  }

  /**
   *  @brief Add another vector (of same dimension) to this vector.
   */
  template<class value2_t>
  Vector&
  operator+= (const Vector<dim,value2_t>& other)
  {
    for (unsigned int elem = 0; elem < dim; ++elem)
      m_elements [elem] += other [elem];
    return *this;
  }

  /**
   *  @brief Substract another vector (of same dimension) from this vector.
   */
  template<class value2_t>
  Vector&
  operator-= (const Vector<dim,value2_t>& other)
  {
    for (unsigned int elem = 0; elem < dim; ++elem)
      m_elements [elem] -= other [elem];
    return *this;
  }

  /**
   *  @brief Add a value to every element in the vector.
   */
  template<class value2_t>
  Vector&
  operator+= (const value2_t& add)
  {
    for (unsigned int elem = 0; elem < dim; ++elem)
      m_elements [elem] += add;
    return *this;
  }

  /**
   *  @brief Substract a value from every element in the vector.
   */
  template<class value2_t>
  Vector&
  operator-= (const value2_t& sub)
  {
    for (unsigned int elem = 0; elem < dim; ++elem)
      m_elements [elem] -= sub;
    return *this;
  }

  /**
   *  @brief Calls @c operator++ for every element in the vector (pre-increment).
   */
  Vector&
  operator++ ()
  {
    for (unsigned int elem = 0; elem < dim; ++elem)
      ++m_elements [elem];
    return *this;
  }

  /**
   *  @brief Calls @c operator++(int) for every element in the vector (post-increment).
   */
  Vector
  operator++ (int)
  {
    Vector<dim,Type> temp (*this);
    for (unsigned int elem = 0; elem < dim; ++elem)
      m_elements [elem]++;
    return temp;
  }

  /**
   *  @brief Calls @c operator-- for every element in the vector (pre-decrement).
   */
  Vector& operator-- ()
  {
    for (unsigned int elem = 0; elem < dim; ++elem)
      --m_elements [elem];
    return *this;
  }

  /**
   *  @brief Calls @c operator--(int) for every element in the vector (post-decrement).
   */
  Vector
  operator-- (int)
  {
    Vector<dim,Type> temp (*this);
    for (unsigned int elem = 0; elem < dim; ++elem)
      m_elements [elem]--;
    return temp;
  }

  /**
   *  @brief Returns the dimension of this vector.
   */
  size_type size () const
  {
    return dim;
  }

  /**
   *  @brief See size method.
   *
   *  This method is only an alias for the size method.
   */
  size_type getLen () const
  {
    return size ();
  }

  /**
   *  @brief Returns a read-only pointer to the element array.
   */
  const Type*
  getValues () const
  {
    return m_elements;
  }

  /**
   *  @brief Returns a read-write pointer to the element array.
   */
  Type*
  getValues ()
  {
    return m_elements;
  }

  /**
   *  @brief clear (set) all elements of the vector to a certain value.
   *
   *  If no parameter is supplied, all elements are set to '0'.
   */
  void
  clear (const Type& value = 0)
  {
    for (unsigned int elem = 0; elem < dim; ++elem)
      m_elements [elem] = value;
  }

  /**
   *  @brief clear (set) part of the vector to a certain value.
   *  @param begin Index of the first element (starting from 0).
   *  @param end   Index of the last element (maximum is: dimension - 1).
   *  @param value %Vector elements are set to this value.
   *
   *  If no 'value' parameter is supplied, vector elements are set to '0'.
   *  Note that 'begin' must be smaller or equal to 'end' and that 'begin' and 'end'
   *  must be smaller than the dimension of the vector. These conditions
   *  are checked with 'assert'.
   */
  void
  clear (unsigned int begin, unsigned int end, const Type& value = 0)
  {
    assert (begin <= end);
    assert (begin < dim);
    assert (end   < dim);
    for (unsigned int elem = begin; elem <= end; ++elem)
      m_elements [elem] = value;
  }

  /**
   *  @brief Calculates the length of the vector.
   *
   *  The geometric length of the vector is the square root of the sum
   *  of all element squares. Note that the sum of all element squares must
   *  be acceptable for @c sqrt().
   */
  Type
  getNorm () const
  {
    Type sum = 0;
    for (unsigned int elem = 0; elem < dim; ++elem)
      sum += m_elements [elem] * m_elements [elem];
    return sqrt (sum);
  }

  /**
   *  @brief See getNorm method.
   *
   *  This method is only an alias for getNorm.
   */
  Type
  getLength () const
  {
    return getNorm ();
  }

  /**
   *  @brief normalize this vector to a length of '1'.
   *
   *  Note that the original length of the vector is checked
   *  with 'assert' if it is greater than '0'.
   */
  void
  normalize ()
  {
    Type length = getNorm ();
    assert (length > 0);
    for (unsigned int elem = 0; elem < dim; ++elem)
      m_elements [elem] /= length;
  }

  /**
   *  @brief Convert to a Vector3D.
   *
   *  Constructs a Vector3D from the first three values of
   *  this vector.
   */
  const Vector3df threeD () const
  {
    assert (dim > 2);
    return Vector3df(m_elements [0], m_elements [1], m_elements [2]);
  }
};

/**

 *
 *  Output operator for Vector on a @c std::ostream.
 */
template<unsigned int dim, typename T>
std::ostream&
operator<< (std::ostream& out, const Vector<dim,T>& vec)
{
  if (!vec.getLen ()) return out;

  for (unsigned int elem = 0; elem < dim - 1; ++elem)
    out << vec [elem] << ' ';
  out << vec [vec.getLen () - 1];

  return out;
}

/**

 *  @brief Compare two Vector for equality.
 *
 *  Equality is checked element for element.
 */
template<unsigned int dim, typename T, typename value2_t>
bool
operator== (const Vector<dim,T>& left, const Vector<dim,value2_t>& right)
{
  for (unsigned int elem = 0; elem < dim; ++elem)
    if (left [elem] != right [elem]) return false;
  return true;
}

/**

 *  @brief Compare two Vector for differences.
 *
 *  Differences are checked element for element.
 */
template<unsigned int dim, typename T, typename value2_t>
bool
operator!= (const Vector<dim,T>& left, const Vector<dim,value2_t>& right)
{
  return !operator== (left, right);
}


/**

 *  @brief Compare every element of a Vector for equality to a value.
 *  @return 'true' if all elements are equal to the value.
 */
template<unsigned int dim, typename T, typename value2_t>
bool
operator== (const Vector<dim,T>& left, const value2_t& right)
{
  for (unsigned int elem = 0; elem < dim; ++elem)
    if (left [elem] != right) return false;
  return true;
}

/**

 *  @brief Compare every element of a Vector for differences to a value.
 */
template<unsigned int dim, class T>
bool
operator!= (const Vector<dim,T>& left, const T& right)
{
  return !(left == right);
}

/**

 *  @brief Calculate the dot product of two Vector.
 *
 *  Both vectors must have the same dimension. The return value
 *  has the same data type as the left hand side vector of the
 *  multiplication.
 */
template<unsigned int dim, typename T, typename value2_t>
const T
operator* (const Vector<dim,T>& left, const Vector<dim,value2_t>& right)
{
  T temp = 0;
  for (unsigned int elem = 0; elem < dim; ++elem)
    temp += left [elem] * right [elem];
  return temp;
}

/**

 *  @brief Scale a Vector by a factor, element for element.
 *  @return A temporary, scaled Vector.
 */
template<unsigned int dim, typename T, typename value2_t>
const Vector<dim,T>
operator* (const Vector<dim,T>& left, const value2_t& right)
{
  Vector<dim,T> temp (left);
  temp *= right;
  return temp;
}

/**

 *  @brief Add two Vector.
 *  @return The result of the addition as temporary copy.
 *
 *  The element data type of the result is the same as the left
 *  hand side vector of the addition.
 */
template<unsigned int dim, typename T, typename value2_t>
const Vector<dim,T>
operator+ (const Vector<dim,T>& left, const Vector<dim,value2_t>& right)
{
  Vector<dim,T> temp (left);
  temp += right;
  return temp;
}

/**

 *  @brief Substract two Vector.
 *  @return The result of the substraction as temporary copy.
 *
 *  The element data type of the result is the same as the left
 *  hand side vector of the substraction.
 */
template<unsigned int dim, typename T, typename value2_t>
const Vector<dim,T>
operator- (const Vector<dim,T>& left, const Vector<dim,value2_t>& right)
{
  Vector<dim,T> temp (left);
  temp -= right;
  return temp;
}

#endif /* not INCLUDED_ROVI_RVECTOR_H */
