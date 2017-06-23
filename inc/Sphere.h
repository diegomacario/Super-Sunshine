#ifndef SPHERE_H
#define SPHERE_H

#include "Affine.h"
#include "GeometricShape.h"

/*
Description:

   The Sphere class inherits from the GeometricShape class and is used to represent spheres.

Possible improvements:

   - The current implementation of the sphere intersection algorithm could be optimized
     with the advice provided in the book "Real-Time Rendering" by Hanes, Hoffman and Moller.

   - The second version of the isIntersected() function calls the first version of the sphereIsIntersected() function,
     even though it does not need the transformed normal at the hit point. This reduces code duplication, but it could be optimized.

   - There is some code duplication between the two versions of the sphereIsIntersected() function, but providing the second version
     helps avoid some unnecessary calculations in the third version of the isIntersected() function.
*/

class Sphere : public GeometricShape
{
public:

   Sphere(const Point& center, const float radius, const Affine& objToWorldTransf, const Affine& worldToObjTransf, const Colour& ambient, Material* material);
   ~Sphere();

   bool isIntersected(const Ray& ray, Intersection* intersection) const;
   bool isIntersected(const Ray& ray, float& distAlongRayToIntersection) const;
   bool isIntersected(const Ray& ray) const;

private:

   // This function implements the sphere intersection algorithm
   bool sphereIsIntersected(const Ray& ray, Point& transformedHitPoint, Normal& transformedNormalAtHitPoint) const;
   // This function implements a true/false version of the sphere intersection algorithm
   bool sphereIsIntersected(const Ray& ray) const;

   Point center;
   float radius;

   Affine objToWorldTransf, worldToObjTransf;
};

#endif

