#ifndef POINT_H
#define POINT_H

/*
Description:

   The Point class is used to represent points in 3 dimensions.
   It provides useful functions to perform operations with points.

Possible improvements:

   - There are 4 versions of the dot() function. Two are provided here, and two inside Vector.h.
     A template could be created to unify them, reducing code duplication.
*/

class Vector;
class Normal;

class Point
{
public:

   Point();
   Point(float x, float y, float z);
   ~Point();

   void set(float x, float y, float z);

   // Point - Point = Vector
   const Vector operator-(const Point& rhsPoint) const;

   // Point +- Vector = Point
   const Point operator+(const Vector& rhsVec) const;
   const Point operator-(const Vector& rhsVec) const;

   float x, y, z;
};

float dot(const Point& lhsPoint, const Normal& rhsNormal);
float dot(const Point& lhsPoint, const Point& rhsPoint);

#endif

