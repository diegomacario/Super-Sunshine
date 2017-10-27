#include <cmath>

#include "Point.h"
#include "Vector.h"
#include "Normal.h"

Vector::Vector()
   : x(0)
   , y(0)
   , z(0)
{ }

Vector::Vector(float x, float y, float z)
   : x(x)
   , y(y)
   , z(z)
{ }

Vector::~Vector()
{ }

void Vector::set(float x, float y, float z)
{
   this->x = x;
   this->y = y;
   this->z = z;
}

const Vector Vector::operator*(const float scalar) const
{
   return Vector(x * scalar, y * scalar, z * scalar);
}

const Vector Vector::operator/(const float scalar) const
{
   return Vector(x / scalar, y / scalar, z / scalar);
}

// Vector +- Vector = Vector
const Vector Vector::operator+(const Vector& rhsVec) const
{
   return Vector(x + rhsVec.x, y + rhsVec.y, z + rhsVec.z);
}

const Vector Vector::operator-(const Vector& rhsVec) const
{
   return Vector(x - rhsVec.x, y - rhsVec.y, z - rhsVec.z);
}

// Vector +- Point = Point
const Point Vector::operator+(const Point& rhsPoint) const
{
   return Point(x + rhsPoint.x, y + rhsPoint.y, z + rhsPoint.z);
}

const Point Vector::operator-(const Point& rhsPoint) const
{
   return Point(x - rhsPoint.x, y - rhsPoint.y, z - rhsPoint.z);
}

const Vector Vector::normalize() const
{
   if ((x == 0) && (y == 0) && (z == 0))
   {
      return Vector();
   }
   else
   {
      return (Vector(x, y, z) / static_cast<float>(sqrt((x * x) + (y * y) + (z * z))));
   }
}

float Vector::length() const
{
   return static_cast<float>(sqrt((x * x) + (y * y) + (z * z)));
}

const Vector operator*(float scalar, const Vector& rhsVec)
{
   return Vector(scalar * rhsVec.x, scalar * rhsVec.y, scalar * rhsVec.z);
}

const Vector cross(const Vector& lhsVec, const Vector& rhsVec)
{
   float i, j, k;

   i = (lhsVec.y * rhsVec.z) - (lhsVec.z * rhsVec.y);
   j = (lhsVec.z * rhsVec.x) - (lhsVec.x * rhsVec.z);
   k = (lhsVec.x * rhsVec.y) - (lhsVec.y * rhsVec.x);

   return Vector(i, j, k);
}

float dot(const Vector& lhsVec, const Vector& rhsVec)
{
   return ((lhsVec.x * rhsVec.x) + (lhsVec.y * rhsVec.y) + (lhsVec.z * rhsVec.z));
}

float dot(const Vector& lhsVec, const Normal& rhsNormal)
{
   return ((lhsVec.x * rhsNormal.x) + (lhsVec.y * rhsNormal.y) + (lhsVec.z * rhsNormal.z));
}

