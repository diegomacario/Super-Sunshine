#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Point.h"
#include "Light.h"

/*
Description:

   The PointLight class inherits from the Light class and is used to represent point lights.

   This type of light is placed at a specific location and can be affected by constant, linear and quadratic attenuation.
*/

struct Attenuation
{
   Attenuation()
      : constant(1)
      , linear(0)
      , quadratic(0)
   { }

   Attenuation(float constant, float linear, float quadratic)
      : constant(constant)
      , linear(linear)
      , quadratic(quadratic)
   { }

   void set(float constant, float linear, float quadratic)
   {
      this->constant = constant;
      this->linear = linear;
      this->quadratic = quadratic;
   }

   float constant, linear, quadratic;
};

class PointLight : public Light
{
public:

   PointLight(const Point& lightPosition, const Attenuation& attenuation, const Colour& colour);
   ~PointLight();

   Colour calculateContributionOfLight(const Point& eye, const Intersection* const intersection) const;

   bool checkForObstacleBetweenPointAndLight(const Point& point,
                                             std::vector<GeometricShape*>::iterator objectIteratorBegin,
                                             const std::vector<GeometricShape*>::iterator objectIteratorEnd) const;

private:

   Point lightPosition;
   Attenuation attenuation;
};

#endif

