#include "Colour.h"

Colour::Colour()
   : r(0)
   , g(0)
   , b(0)
{ }

Colour::Colour(float c)
   : r(c)
   , g(c)
   , b(c)
{ }

Colour::Colour(float r, float g, float b)
   : r(r)
   , g(g)
   , b(b)
{ }

Colour::~Colour()
{ }

void Colour::set(float r, float g, float b)
{
   this->r = r;
   this->g = g;
   this->b = b;
}

const Colour Colour::operator+(const Colour& rhsCol) const
{
   return Colour(r + rhsCol.r, g + rhsCol.g, b + rhsCol.b);
}

const Colour Colour::operator-(const Colour& rhsCol) const
{
   return Colour(r - rhsCol.r, g - rhsCol.g, b - rhsCol.b);
}

// Component-wise multiplication:
// (r1, g1, b1) * (r2, g2, b2) = (r1*r2, g1*g2, b1*b2)
const Colour Colour::operator*(const Colour& rhsCol) const
{
   return Colour(r * rhsCol.r, g * rhsCol.g, b * rhsCol.b);
}

const Colour Colour::operator*(const float scalar) const
{
   return Colour(r * scalar, g * scalar, b * scalar);
}

const Colour Colour::operator/(const float scalar) const
{
   return Colour(r / scalar, g / scalar, b / scalar);
}

const Colour operator*(float scalar, const Colour& rhsCol)
{
   return Colour(scalar * rhsCol.r, scalar * rhsCol.g, scalar * rhsCol.b);
}

