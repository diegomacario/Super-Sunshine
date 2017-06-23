#include "Light.h"

Light::Light(const Colour& colour)
   : colour(colour)
{ }

Light::~Light()
{ }

Colour Light::getColour() const
{
   return colour;
}

