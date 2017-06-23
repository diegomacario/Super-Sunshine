#include "Point.h"
#include "Vector.h"
#include "Ray.h"

Ray::Ray()
   : origin(0, 0, 0)
   , direction(0, 0, 0)
{ }

Ray::Ray(const Point& origin, const Vector& direction)
   : origin(origin)
   , direction(direction)
{ }

Ray::~Ray()
{ }

