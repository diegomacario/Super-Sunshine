#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "GeometricShape.h"

/*
Description:

   The Triangle class inherits from the GeometricShape class and is used to represent triangles in 3 dimensions.

Possible improvements:

   - The current implementation of the triangle intersection algorithm could be optimized
     with the advice provided in the book "Real-Time Rendering" by Hanes, Hoffman and Moller.

   - The 3 versions of the isIntersected() function call the triangleIsIntersected() function,
     even though the second version does not need the hit point and the third one does not need the
     hit point or the distance along the ray to it. This reduces code duplication, but it could be optimized.
*/

class Triangle : public GeometricShape
{
public:

   Triangle(const Point& vertexA, const Point& vertexB, const Point& vertexC, const Colour& ambient, Material* material);

   Triangle(const Point& vertexA, const Point& vertexB, const Point& vertexC, TextureDescription* textureDescription, Material* material);
   ~Triangle();

   bool isIntersected(const Ray& ray, Intersection* intersection) const;
   bool isIntersected(const Ray& ray, float& distAlongRayToIntersection) const;
   bool isIntersected(const Ray& ray) const;

private:

   bool triangleIsIntersected(const Ray& ray, float& distAlongRayToHit, Point& hitPoint) const;
   Point calcBarycentricCoord(Point& hitPoint) const;

   // Vertices are specified CCWISE
   Point vertexA, vertexB, vertexC;
   Normal faceNormal;
};

#endif

