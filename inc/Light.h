#ifndef LIGHT_H
#define LIGHT_H

#include <vector>

#include "Ray.h"
#include "GeometricShape.h"

/*
Description:

   The Light class is the base from which the PointLight and DirectionalLight classes inherit.

   It specifies two pure virtual functions which must be implemented by the classes that inherit from it.
   These functions are common to all lights, but implemented differently for each type of light.

   It also provides the common member variable that defines the colour of a light source.
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

class Light
{
public:

   Light(const Colour& colour);
   virtual ~Light();

   virtual Colour calculateContributionOfLight(const Point& eye, const Intersection* const intersection) const = 0;

   virtual bool checkForObstacleBetweenPointAndLight(const Point& point,
                                                     std::vector<GeometricShape*>::iterator objectIteratorBegin,
                                                     const std::vector<GeometricShape*>::iterator objectIteratorEnd) const = 0;

   Colour getColour() const;

private:

   Colour colour;
};

#endif

