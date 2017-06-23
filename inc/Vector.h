#ifndef VECTOR_H
#define VECTOR_H

/*
Description:

   The Vector class is used to represent vectors in 3 dimensions.
   It provides useful functions to perform operations with vectors.

Possible improvements:

   - There are 3 versions of the dot() function. Two are provided here and one inside Point.h.
     A template could be created to unify them and reduce code duplication.
*/

class Point;
class Normal;

class Vector
{
public:

   Vector();
   Vector(float x, float y, float z);
   ~Vector();

   void set(float x, float y, float z);

   const Vector operator*(const float scalar) const;
   const Vector operator/(const float scalar) const;

   // Vector +- Vector = Vector
   const Vector operator+(const Vector& rhsVec) const;
   const Vector operator-(const Vector& rhsVec) const;

   // Vector +- Point = Point
   const Point operator+(const Point& rhsPoint) const;
   const Point operator-(const Point& rhsPoint) const;

   const Vector normalize() const;

   float length() const;

   float x, y, z;
};

const Vector operator*(float scalar, const Vector& rhsVec);

const Vector cross(const Vector& lhsVec, const Vector& rhsVec);

float dot(const Vector& lhsVec, const Vector& rhsVec);
float dot(const Vector& lhsVec, const Normal& rhsNormal);

#endif

