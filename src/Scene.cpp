#include "Constants.h"
#include "Scene.h"

Scene::Scene(const std::vector<GeometricShape*>& objects, const std::vector<Light*>& lights, int maxDepth)
   : objects(objects)
   , lights(lights)
   , maxDepth(maxDepth)
{ }

Scene::~Scene()
{
   for (std::vector<GeometricShape*>::iterator it = objects.begin(); it != objects.end(); ++it)
   {
      delete *it;
   }

   objects.clear();

   for (std::vector<Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
   {
      delete *it;
   }

   lights.clear();
}

bool Scene::findNearestIntersection(const Ray& ray, Intersection* nearestIntersection)
{
   std::vector<GeometricShape*>::iterator it = objects.begin();

   // Loop over all the objects until we find the first intersection
   while (it != objects.end())
   {
      if ((*it)->isIntersected(ray, nearestIntersection))
      {
         break;
      }
      ++it;
   }

   // Return false if the ray does not intersect any objects
   if (it == objects.end())
   {
      return false;
   }

   // Increase the interator so that we don't calculate the first intersection again
   ++it;

   Intersection currentIntersection;

   // Check if there are any intersections that are closer
   // If there are, we update the description of the intersection
   while (it != objects.end())
   {
      if ((*it)->isIntersected(ray, &currentIntersection))
      {
         if (currentIntersection.distAlongRayToHit < nearestIntersection->distAlongRayToHit)
         {
            nearestIntersection->hitPoint = currentIntersection.hitPoint;
            nearestIntersection->normalAtHit = currentIntersection.normalAtHit;
            nearestIntersection->materialAtHit = currentIntersection.materialAtHit;
            nearestIntersection->ambient = currentIntersection.ambient;
            nearestIntersection->distAlongRayToHit = currentIntersection.distAlongRayToHit;
         }
      }
      ++it;
   }

   return true;
}

Colour Scene::calculateLightingAtIntersection(const Point& eye, Intersection* intersection)
{
   static int depth = 0;

   Colour pixelColour = intersection->ambient + intersection->materialAtHit->getEmission();

   std::vector<GeometricShape*>::iterator objectIteratorBegin = objects.begin();
   std::vector<GeometricShape*>::iterator objectIteratorEnd = objects.end();
   std::vector<Light*>::iterator lightIterator = lights.begin();

   // Cast shadow rays to all the lights in the scene
   while (lightIterator != lights.end())
   {
      // If there are no obstacles between the intersection and a light, we calculate that light's contribution and take it into account
      if (!((*lightIterator)->checkForObstacleBetweenPointAndLight(intersection->hitPoint, objectIteratorBegin, objectIteratorEnd)))
      {
         pixelColour = pixelColour + (*lightIterator)->calculateContributionOfLight(eye, intersection);
      }
      ++lightIterator;
   }

   // Cast the reflected ray
   if (depth < maxDepth)
   {
      // Calculate the reflected ray
      Vector eyeVector = (eye - intersection->hitPoint);
      Vector reflectionDirection = (2 * (dot(eyeVector, intersection->normalAtHit) * intersection->normalAtHit) - eyeVector).normalize();
      Ray reflection(intersection->hitPoint + (Constants::shadowBias * reflectionDirection), reflectionDirection);

      // Store the specularity and the hit point, since we will lose them because we reuse the Intersection object
      Colour oldSpecular = intersection->materialAtHit->getSpecular();
      Point newEye = intersection->hitPoint;

      // If the reflected ray intersects an object, take into account its contribution
      if (findNearestIntersection(reflection, intersection))
      {
         ++depth;
         // Make the recursive call to calculate the lighting at the intersection point of the reflected ray
         pixelColour = pixelColour + (oldSpecular * calculateLightingAtIntersection(newEye, intersection));
      }
      else
      {
         depth = 0;
      }
   }
   else
   {
      depth = 0;
   }

   return pixelColour;
}

