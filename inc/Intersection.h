#ifndef INTERSECTION_H
#define INTERSECTION_H

/*
Description:

   The Intersection struct contains all the information that needs be stored when a ray intersects a shape. This includes:

   - The point in space at which the ray intersects the shape.
   - The surface normal at the intersection point.
   - The material of the shape that is intersected by the ray.
   - The ambient illumination of the shape that is intersected by the ray.
   - The distance from the origin of the ray to the intersection point.

   The first 4 terms are used to make lighting calculations.
   The last term is used to compare different intersections in order to determine which one is the nearest one.
*/

struct Intersection
{
   Intersection()
      : hitPoint()
      , normalAtHit()
      , materialAtHit(nullptr)
      , ambient()
      , distAlongRayToHit(0)
   { }

   Intersection(const Point& hitPoint, const Normal& normalAtHit, Material * materialAtHit, Colour ambient, float distAlongRayToHit)
      : hitPoint(hitPoint)
      , normalAtHit(normalAtHit)
      , materialAtHit(materialAtHit)
      , ambient(ambient)
      , distAlongRayToHit(distAlongRayToHit)
   { }

   Point hitPoint;
   Normal normalAtHit;
   Material * materialAtHit;
   Colour ambient;
   float distAlongRayToHit;
};

#endif

