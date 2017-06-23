#include "Constants.h"
#include "PointLight.h"

PointLight::PointLight(const Point& lightPosition, const Attenuation& attenuation, const Colour& colour)
   : lightPosition(lightPosition)
   , attenuation(attenuation)
   , Light(colour)
{ }

PointLight::~PointLight()
{ }

Colour PointLight::calculateContributionOfLight(const Point& eye, const Intersection* const intersection) const
{
   Vector lightVec = lightPosition - intersection->hitPoint;
   float distBetweenLightAndIntersection = lightVec.length();
   lightVec = lightVec.normalize();

   Vector eyeVec = (eye - intersection->hitPoint).normalize();
   Vector halfVec = (lightVec + eyeVec).normalize();

   // For a point light, lighting is given by:
   // (Intensity / Attenuation) * (Diffuse Contribution + Specular Contribution)

   Colour attenuatedColour = (getColour() / (attenuation.constant +
                                            (attenuation.linear * distBetweenLightAndIntersection) +
                                            (attenuation.quadratic * distBetweenLightAndIntersection * distBetweenLightAndIntersection)));

   return (attenuatedColour * intersection->materialAtHit->calculateDiffuseAndSpecularLighting(intersection->normalAtHit, lightVec, halfVec));
}

bool PointLight::checkForObstacleBetweenPointAndLight(const Point& point,
                                                      std::vector<GeometricShape*>::iterator objectIteratorBegin,
                                                      const std::vector<GeometricShape*>::iterator objectIteratorEnd) const
{
   Vector shadowRayDir = (lightPosition - point).normalize();

   // Move the origin of the shadow ray in the direction of the ray to avoid self-shadowing
   Ray shadowRay(point + (Constants::shadowBias * shadowRayDir), shadowRayDir);

   float distanceBetweenPointAndLight = (lightPosition - point).length();
   float distanceAlongRayToObstacle;

   // Loop over all the objects checking if the shadow ray intersects any of them
   // If an intersection is found, and the distance from the ray's origin to it is smaller 
   // than the distance from the ray's origin to the light, we return true because the light is blocked
   while (objectIteratorBegin != objectIteratorEnd)
   {
      if ((*objectIteratorBegin)->isIntersected(shadowRay, distanceAlongRayToObstacle))
      {
         if (distanceAlongRayToObstacle < distanceBetweenPointAndLight)
         {
            return true;
         }
      }
      ++objectIteratorBegin;
   }

   return false;
}

