#include <cmath>

#include "Vector.h"
#include "Normal.h"

Normal::Normal()
   : x(0)
   , y(0)
   , z(0)
{ }

Normal::Normal(float x, float y, float z)
{
   if ((x == 0) && (y == 0) && (z == 0))
   {
      this->x = x;
      this->y = y;
      this->z = z;
   }
   else
   {
      float length = static_cast<float>(sqrt((x * x) + (y * y) + (z * z)));

      this->x = x / length;
      this->y = y / length;
      this->z = z / length;
   }
}

Normal::Normal(const Vector& vec)
{
   float length = vec.length();

   if (length == 0)
   {
      this->x = vec.x;
      this->y = vec.y;
      this->z = vec.z;
   }
   else
   {
      this->x = vec.x / length;
      this->y = vec.y / length;
      this->z = vec.z / length;
   }
}

Normal::~Normal()
{ }

const Normal Normal::normalize() const
{
   if ((x == 0) && (y == 0) && (z == 0))
   {
      return Normal();
   }
   else
   {
      float length = static_cast<float>(sqrt((x * x) + (y * y) + (z * z)));
      return (Normal(x / length, y / length, z / length));
   }
}

const Normal Normal::operator+(const Normal& rhsNormal) const
{
   return Normal(x + rhsNormal.x, y + rhsNormal.y, z + rhsNormal.z).normalize();
}

const Normal Normal::operator-(const Normal& rhsNormal) const
{
   return Normal(x - rhsNormal.x, y - rhsNormal.y, z - rhsNormal.z).normalize();
}

const Vector operator*(float scalar, const Normal& rhsNormal)
{
   return Vector(scalar * rhsNormal.x, scalar * rhsNormal.y, scalar * rhsNormal.z);
}

