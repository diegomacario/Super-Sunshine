#include <math.h>

#include "Ray.h"
#include "Sphere.h"

Sphere::Sphere(const Point& center, const float radius, const Affine& objToWorldTransf, const Affine& worldToObjTransf, const Colour& ambient, Material* material)
   : center(center)
   , radius(radius)
   , objToWorldTransf(objToWorldTransf)
   , worldToObjTransf(worldToObjTransf)
   , GeometricShape(ambient, material)
{ }

Sphere::~Sphere()
{ }

bool Sphere::isIntersected(const Ray& ray, Intersection* intersection) const
{
   Point transformedHitPoint;
   Normal transformedNormalAtHitPoint;

   if (sphereIsIntersected(ray, transformedHitPoint, transformedNormalAtHitPoint))
   {
      // We apply M to the intersection point to bring it back into the appropriate coordinate system
      intersection->hitPoint = objToWorldTransf * transformedHitPoint;

      // We apply (M^-1)^T to the normal at the intersection point (this is the standard way of transforming normals)
      intersection->normalAtHit = worldToObjTransf * transformedNormalAtHitPoint;

      intersection->materialAtHit = getMaterial();
      intersection->ambient = getAmbient();
      intersection->distAlongRayToHit = (intersection->hitPoint - ray.origin).length();

      return true;
   }

   return false;
}

bool Sphere::isIntersected(const Ray& ray, float& distAlongRayToIntersection) const
{
   Point transformedHitPoint;
   Normal temporaryNormal;

   if (sphereIsIntersected(ray, transformedHitPoint, temporaryNormal))
   {
      // We apply M to the intersection point to bring it back into the appropriate coordinate system
      distAlongRayToIntersection = ((objToWorldTransf * transformedHitPoint) - ray.origin).length();

      return true;
   }

   return false;
}

bool Sphere::isIntersected(const Ray& ray) const
{
   if (sphereIsIntersected(ray))
   {
      return true;
   }

   return false;
}

// This function implements the sphere intersection algorithm
bool Sphere::sphereIsIntersected(const Ray& ray, Point& transformedHitPoint, Normal& transformedNormalAtHitPoint) const
{
   // The sphere intersection algorithm is based on the quadratic equation given by:
   // dot((pointOnCircumference - center), (pointOnCircumference - center)) = radius^2

   // Replacing pointOnCircumference by the equation of a ray gives the complete expression,
   // in which the unknown is the distance along the ray to intersection point

   // A short review of some of the necessary knowledge on quadratic equations is presented below:
   // a*t^2 + b*t + c = 0
   // discriminant = b^2 - 4*a*c

   // We are interested in using the sphere intersection algorithm with both spheres and transformed spheres, such as ellipsoids
   // To achieve this, instead of applying a transformation M to a sphere, we apply M^-1 to the ray we are testing for intersections
   // We then check if the transformed ray intersects the sphere
   // If we find an intersection, we apply M to it in order to bring it back to the appropriate coordinate system

   Ray transformedRay(worldToObjTransf * ray.origin, (worldToObjTransf * ray.direction).normalize());

   Vector fromCenterToEye = transformedRay.origin - center;

   float a = dot(transformedRay.direction, transformedRay.direction);
   float b = 2 * dot(transformedRay.direction, fromCenterToEye);
   float c = dot(fromCenterToEye, fromCenterToEye) - (radius * radius);

   float discriminant = (b * b) - (4 * a * c);

   // If the discriminant is smaller than zero, the roots of the quadratic equation are complex
   // Consequently, there is no intersection
   if (discriminant < 0)
   {
      return false;
   }

   float rootA = (-b + static_cast<float>(sqrt(discriminant))) / (2 * a);
   float rootB = (-b - static_cast<float>(sqrt(discriminant))) / (2 * a);

   // If both roots are negative, the ray intersects a sphere that is behind the camera
   // This is not considered a valid intersection because we are only interested in the spheres that are in front of the camera
   if ((rootA < 0) && (rootB < 0))
   {
      return false;
   }

   float distAlongRayToHit;

   if ((rootA >= 0) && (rootB >= 0))
   {
      // If the two roots are real, non-negative and not equal:
      // We pick the smallest one, since it corresponds to the closest intersection
      if (rootA < rootB)
      {
         distAlongRayToHit = rootA;
      }
      else if (rootB < rootA)
      {
         distAlongRayToHit = rootB;
      }
      // If the two roots are real, non-negative and equal:
      // We pick any of the two roots, since the ray is tangent to the sphere
      else
      {
         distAlongRayToHit = rootA;
      }
   }
   else if (((rootA >= 0) && (rootB < 0)) || ((rootA < 0) && (rootB >= 0)))
   {
      // If one root is non-negative and the other is negative, the origin of the ray is inside the sphere
      // For this reason we pick the non-negative one
      if (rootA > rootB)
      {
         distAlongRayToHit = rootA;
      }
      else
      {
         distAlongRayToHit = rootB;
      }
   }

   transformedHitPoint = ((transformedRay.direction * distAlongRayToHit) + transformedRay.origin);
   transformedNormalAtHitPoint = Normal(transformedHitPoint - center);

   return true;
}

// This function implements a true/false version of the sphere intersection algorithm
bool Sphere::sphereIsIntersected(const Ray& ray) const
{
   // The sphere intersection algorithm is based on the quadratic equation given by:
   // dot((pointOnCircumference - center), (pointOnCircumference - center)) = radius^2

   // Replacing pointOnCircumference by the equation of a ray gives the complete expression,
   // in which the unknown is the distance along the ray to intersection point

   // A short review of some of the necessary knowledge on quadratic equations is presented below:
   // a*t^2 + b*t + c = 0
   // discriminant = b^2 - 4*a*c

   // We are interested in using the sphere intersection algorithm with both spheres and transformed spheres, such as ellipsoids
   // To achieve this, instead of applying a transformation M to a sphere, we apply M^-1 to the ray we are testing for intersections
   // We then check if the transformed ray intersects the sphere
   // If we find an intersection, we apply M to it in order to bring it back to the appropriate coordinate system

   Ray transformedRay(worldToObjTransf * ray.origin, (worldToObjTransf * ray.direction).normalize());

   Vector fromCenterToEye = transformedRay.origin - center;

   float a = dot(transformedRay.direction, transformedRay.direction);
   float b = 2 * dot(transformedRay.direction, fromCenterToEye);
   float c = dot(fromCenterToEye, fromCenterToEye) - (radius * radius);

   float discriminant = (b * b) - (4 * a * c);

   // If the discriminant is smaller than zero, the roots of the quadratic equation are complex
   // Consequently, there is no intersection
   if (discriminant < 0)
   {
      return false;
   }

   float rootA = (-b + static_cast<float>(sqrt(discriminant))) / (2 * a);
   float rootB = (-b - static_cast<float>(sqrt(discriminant))) / (2 * a);

   // If both roots are negative, the ray intersects a sphere that is behind the camera
   // This is not considered a valid intersection because we are only interested in the spheres that are in front of the camera
   if ((rootA < 0) && (rootB < 0))
   {
      return false;
   }

   return true;
}

