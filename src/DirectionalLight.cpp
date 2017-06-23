#include "Constants.h"
#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const Vector& lightDirection, const Colour& colour)
   : lightDirection(lightDirection)
   , Light(colour)
{ }

DirectionalLight::~DirectionalLight()
{ }

Colour DirectionalLight::calculateContributionOfLight(const Point& eye, const Intersection* const intersection) const
{
   Vector lightVec = lightDirection.normalize();
   Vector eyeVec = (eye - intersection->hitPoint).normalize();
   Vector halfVec = (lightVec + eyeVec).normalize();

   return (getColour() * intersection->materialAtHit->calculateDiffuseAndSpecularLighting(intersection->normalAtHit, lightVec, halfVec));
}

bool DirectionalLight::checkForObstacleBetweenPointAndLight(const Point& point,
                                                            std::vector<GeometricShape*>::iterator objectIteratorBegin,
                                                            const std::vector<GeometricShape*>::iterator objectIteratorEnd) const
{
   // Move the origin of the shadow ray in the direction of the ray to avoid self-shadowing
   Ray shadowRay(point + (Constants::shadowBias * lightDirection), lightDirection.normalize());

   // Loop over all the objects checking if the shadow ray intersects any of them
   // If an intersection is found we return true, because the light is blocked
   while (objectIteratorBegin != objectIteratorEnd)
   {
      if ((*objectIteratorBegin)->isIntersected(shadowRay))
      {
         return true;
      }
      ++objectIteratorBegin;
   }

   return false;
}

