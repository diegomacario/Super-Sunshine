#include "Point.h"
#include "Vector.h"
#include "Normal.h"

Point::Point()
   : x(0)
   , y(0)
   , z(0)
{ }

Point::Point(float x, float y, float z)
   : x(x)
   , y(y)
   , z(z)
{ }

Point::~Point()
{ }

void Point::set(float x, float y, float z)
{
   this->x = x;
   this->y = y;
   this->z = z;
}

// Point - Point = Vector
const Vector Point::operator-(const Point& rhsPoint) const
{
   return Vector(x - rhsPoint.x, y - rhsPoint.y, z - rhsPoint.z);
}

// Point + Vector = Point
const Point Point::operator+(const Vector& rhsVec) const
{
   return Point(x + rhsVec.x, y + rhsVec.y, z + rhsVec.z);
}

// Point - Vector = Point
const Point Point::operator-(const Vector& rhsVec) const
{
   return Point(x - rhsVec.x, y - rhsVec.y, z - rhsVec.z);
}

float dot(const Point& lhsPoint, const Normal& rhsNormal)
{
   return ((lhsPoint.x * rhsNormal.x) + (lhsPoint.y * rhsNormal.y) + (lhsPoint.z * rhsNormal.z));
}

float dot(const Point& lhsPoint, const Point& rhsPoint)
{
   return ((lhsPoint.x * rhsPoint.x) + (lhsPoint.y * rhsPoint.y) + (lhsPoint.z * rhsPoint.z));
}

