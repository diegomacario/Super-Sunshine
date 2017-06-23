#ifndef RAY_H
#define RAY_H

#include "Point.h"
#include "Vector.h"

/*
Description:

   The Ray class is used to represent rays in 3 dimensions.

   The equation of a ray is:
   Ray = origin + (direction * t)

   Where origin is a point and direction is a vector. The parameter t is used to measure distances along the ray.
*/

class Ray
{
public:

   Ray();
   Ray(const Point& origin, const Vector& direction);
   ~Ray();

   Point origin;
   Vector direction;
};

#endif

