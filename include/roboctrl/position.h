#ifndef POSITION_H_
#define POSITION_H_

#include <iostream>

#include "angle.h"
#include "share.h"

/** @brief Class for handling positions.
 *  Position class which can hold the position of a robot or any other position.
 *  The class also handles additional common computations with positions like
 *  computing the euclidian distance between two positions. The class has a
 *  internal resolution of 1 mm.
 */
class Position {
private:
    static const double RESOLUTION = 1000.0; // 1000.0 means we have a resolution of mm

    double mX; // Stores X value in m
    double mY; // Stores Y value in m

    // rounds all position information to a resolution of mm
    double RoundToResolution(const double op) const {
        const double res = floor(fabs(op) * RESOLUTION + 0.5) / RESOLUTION;
        if(op < 0) return -res;
        else return res;
    }

public:
    /** Initializes the Position to (0.0/0.0).
     *  @param[in] x The X value in m.
     *  @param[in] y The Y value in m.
     */
    Position(double x = 0.0, double y = 0.0) : mX(RoundToResolution(x)), mY(RoundToResolution(y)) { }

    /** Get X value.
     *  @return The X value.
     */
    double GetX(void) const { return mX; }
    /** Get Y value.
     *  @return The Y value.
     */
    double GetY(void) const { return mY; }
    /** Get a copy of the object.
     *  @return The new Position object.
     */
    Position GetPos(void) const { return Position(mX, mY); }

    /** Set X value.
     *  @param[in] x The X value in m.
     */
    void SetX(double x) { mX = RoundToResolution(x); }
    /** Set Y value.
     *  @param[in] y The Y value in m.
     */
    void SetY(double y) { mY = RoundToResolution(y); }

    /** Computes the distance between the objects position and the one given by the argument.
     *  @param[in] dest The distance to this Position is calculated.
     *  @return The computed distance in m.
     */
    double DistanceTo(const Position& dest) const { return sqrt(pow(dest.GetX() - mX, 2) + pow(dest.GetY() - mY, 2)); }

    /** Computes the angle of a line between the objects position and the destination position.
     *  @param[in] dest Destination Position for the calcualation
     *  @return Angle to this position. If dest == own Position the methode returns a zero Angle.
     */
    Angle AngleOfLineToPos(const Position& dest) {
        //TODO MS replace with atan2
        if(this->GetPos() != dest) {
            // necessary, because if both positions are equal atan returns crap
            double psi = -atan((dest.GetX() - mX) / (dest.GetY() - mY));
            if((dest.GetY() - mY) < 0) psi -= (M_PI / 2.0);
            else psi += (M_PI / 2.0);
            return Angle(psi);
        }
        else return Angle(0.0);
    }

    bool operator==(const Position op2) {
        return ((mX == op2.GetX()) && (mY == op2.GetY()));
    }

    bool operator!=(const Position op2) {
        return ((mX != op2.GetX()) || (mY != op2.GetY()));
    }

    const Position& operator+=(const Position& op2) {
        mX += op2.GetX();
        mY += op2.GetY();
        return *this;
    }

    const Position& operator-=(const Position& op2) {
        mX -= op2.GetX();
        mY -= op2.GetY();
        return *this;
    }
};

std::ostream& operator<<(std::ostream& ausgabe, const Position& pos);

#endif /* POSITION_H_ */
