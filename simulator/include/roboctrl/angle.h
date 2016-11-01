#ifndef ANGLE_H_
#define ANGLE_H_

/** @file angle.h
 * @brief Defines angle class and operators.
 *
 *
 * @author Original: TBD, Revised: M. Anselm 05/2012
 */

#include "share.h"
#include "global.h"

/** @brief Class for handling angles.
 *
 * The Angle Classes main job is to make sure that the angles are in the range of -M_PI to M_PI.
 * This is the convention which is used in this API. Furthermore the class allows the
 * comparisons of two angles.
 * You can also do arithmetic operations with the angle objects.
 * And finally the class makes it easy to convert between the radian and the degree
 * representation of an angle and allows to use the stream output <<
 * with angles (they are output as "DEG°").
 */
class Angle
{
private:
	double mAngle;
public:
        /** Constructor for Angle class.
	 *  @param[in] degree Initial angle value in degree.
	 */
        Angle(const int32_t degree) : mAngle(toSymRad(degree)) {}

        /** Constructor for Angle class.
	 *  @param[in] angle Initial angle value in radian.
	 */
        Angle(const double angle = 0.0) : mAngle(toSymRad(angle)) {}

        /** Constructor for Angle class
	 *  @param[in] angle Initial angle value from an other Angle object.
	 */
        Angle(const Angle &angle) : mAngle(angle.toDouble()) {}

        /** Set the angle.
         *  @param[in] angle New angle value in radian.
         */
        void Set(double angle) { mAngle = toSymRad(angle); }

        /** Set angle form a floating point value.
          * @param[in] val Floating point value in degree.
          */
        void setFloatDeg(double val) { mAngle = toSymRad(val * DEG_TO_RAD); }

        /** Get the angle in radian.
	 *  @return angle in radian.
	 *  @see Rad(void) const
	 */
        double Get(void) const { return mAngle; }

        /** Get the angle in radian.
         *  @return angle in radian.
	 *  @see Get(void) const
	 */
        double Rad(void) const { return mAngle; }

        /** Get the angle in radian.
         *  @return angle in radian.
         *  @see Get(void) const
         */
        double toDouble(void) const { return mAngle; }

        /** Get the angle in degree.
         *  @return angle in degree.
         */
        int32_t Deg(void) const { return (mAngle * RAD_TO_DEG); }

        /** Get angle in degree as a floating point value.
          */
        double getFloatDeg(void) const { return double(mAngle * RAD_TO_DEG); }

        /** Rotates all angles in order to make them all positive.
         *  Used to enable comparisons between angles.
         */
        double GetPositive(void) const { if (mAngle < 0.0) return mAngle + 2.0 * D_PI; else return mAngle; }

        /** Get the absolut angle in radian.
	 *  @return Absolut angle in radian.
	 */
        double Abs(void) const { return fabs(mAngle); }

        Angle& operator=(const double op2) {
            mAngle = toSymRad(op2);
            return *this;
        }

        Angle& operator=(const Angle op2) {
            mAngle = op2.toDouble();
            return *this;
        }

        Angle& operator+=(const Angle op2) {
            mAngle = toSymRad(mAngle + op2.toDouble());
            return *this;
        }

        Angle& operator-=(const Angle op2) {
            mAngle = toSymRad(mAngle - op2.toDouble());
            return *this;
        }

        Angle& operator+=(const double op2) {
            mAngle = toSymRad(mAngle + op2);
            return *this;
        }

        Angle& operator-=(const double op2) {
            mAngle = toSymRad(mAngle - op2);
            return *this;
        }

        Angle operator-(void) const {
            return Angle(-mAngle);
        }
};

/** @brief Add two Angles and return the result as a temporary Angle.
*/
inline Angle operator+(const Angle op1, const Angle op2) {
    return Angle(op1.toDouble() + op2.toDouble());
}

/** @brief Add a double to an Angle and return the result
*   as a temporary Angle (commutative).
*/
inline Angle operator+(const Angle op1, const double op2) {
    return Angle(op1.toDouble() + op2);
}
inline Angle operator+(const double op1, const Angle op2) {
    return (op2 + op1);
}

/** @brief Multiply an Angle with a double and return the result
*   as a temporary Angle (commutative).
*/
inline Angle operator*(const Angle op1, const double op2) {
    return Angle(toSymRad(op1.toDouble() * op2));
}
inline Angle operator*(const double op1, const Angle op2) {
    return (op2 * op1);
}

/** @brief Divide an Angle by a double and return the result as a temporary Angle.
*/
inline Angle operator/(const Angle op1, const double op2) {
    return Angle(toSymRad(op1.toDouble() / op2));
}

/** @brief Subtract two Angles and return the result as a temporary Angle.
*/
inline Angle operator-(const Angle op1, const Angle op2) {
    return Angle(op1.toDouble() - op2.toDouble());
}

/** @brief Subtract a double from an Angle and return the result as a temporary Angle
*/
inline Angle operator-(const Angle op1, const double op2) {
    return Angle(op1.toDouble() - op2);
}

/** @brief Compare two Angles or an Angle and a double.
*   Equality is checked with isZero.
*/
inline bool operator==(const Angle op1, const Angle op2) {
    return (isZero(op1.toDouble() - op2.toDouble()));
}
inline bool operator==(const Angle op1, const double op2) {
    return (op1 == Angle(op2));
}
inline bool operator==(const double op1, const Angle op2) {
    return (Angle(op2) == op1);
}

inline bool operator!=(const Angle op1, const Angle op2) {
    return !(op1 == op2);
}
inline bool operator!=(const Angle op1, const double op2) {
    return !(op1 == Angle(op2));
}
inline bool operator!=(const double op1, const Angle op2) {
    return !(Angle(op2) == op1);
}

inline bool operator>(const Angle op1, const Angle op2) {
    return (op1.toDouble() > op2.toDouble());
}
inline bool operator>(const Angle op1, const double op2) {
    return (op1 > Angle(op2));
}
inline bool operator>(const double op1, const Angle op2) {
    return (Angle(op1) > op2);
}

inline bool operator>=(const Angle op1, const Angle op2) {
    return (op1.toDouble() >= op2.toDouble());
}
inline bool operator>=(const Angle op1, const double op2) {
    return (op1 >= Angle(op2));
}
inline bool operator>=(const double op1, const Angle op2) {
    return (Angle(op1) >= op2);
}

inline bool operator<(const Angle op1, const Angle op2) {
    return (op1.toDouble() < op2.toDouble());
}
inline bool operator<(const Angle op1, const double op2) {
    return (op1 < Angle(op2));
}
inline bool operator<(const double op1, const Angle op2) {
    return (Angle(op1) < op2);
}

inline bool operator<=(const Angle op1, const Angle op2) {
    return (op1.toDouble() <= op2.toDouble());
}
inline bool operator<=(const Angle op1, const double op2) {
    return (op1 <= Angle(op2));
}
inline bool operator<=(const double op1, const Angle op2) {
    return (Angle(op1) <= op2);
}

inline double cos(Angle val){
    return cos(val.toDouble());
}

inline double sin(Angle val){
    return sin(val.toDouble());
}

inline double tan(Angle val){
    return tan(val.toDouble());
}

std::ostream& operator<<(std::ostream& ausgabe, const Angle& angle);

#endif /* ANGLE_H_ */
