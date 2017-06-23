#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Vector.h"
#include "Light.h"

/*
Description:

   The DirectionalLight class inherits from the Light class and is used to represent directional lights.

   This type of light is given a specific direction and is not affected by attenuation.
*/

class DirectionalLight : public Light
{
public:

   DirectionalLight(const Vector& lightDirection, const Colour& colour);
   ~DirectionalLight();

   Colour calculateContributionOfLight(const Point& eye, const Intersection* const intersection) const;

   bool checkForObstacleBetweenPointAndLight(const Point& point,
                                             std::vector<GeometricShape*>::iterator objectIteratorBegin,
                                             const std::vector<GeometricShape*>::iterator objectIteratorEnd) const;

private:

   Vector lightDirection;
};

#endif

