#include "Vector.h"
#include "Ray.h"
#include "Triangle.h"

Triangle::Triangle(const Point& vertexA, const Point& vertexB, const Point& vertexC, const Colour& ambient, Material* material)
   : vertexA(vertexA)
   , vertexB(vertexB)
   , vertexC(vertexC)
   , GeometricShape(ambient, material)
{
   // Compute the normal
   Vector vec = cross((vertexB - vertexA), (vertexC - vertexA));
   faceNormal = Normal(vec.x, vec.y, vec.z);
}

Triangle::~Triangle()
{ }

bool Triangle::isIntersected(const Ray& ray, Intersection* intersection) const
{
   if (triangleIsIntersected(ray, intersection->distAlongRayToHit, intersection->hitPoint))
   {
      intersection->normalAtHit = faceNormal;
      intersection->materialAtHit = getMaterial();
      intersection->ambient = getAmbient();

      return true;
   }

   return false;
}

bool Triangle::isIntersected(const Ray& ray, float& distAlongRayToIntersection) const
{
   if (triangleIsIntersected(ray, distAlongRayToIntersection, Point()))
   {
      return true;
   }

   return false;
}

bool Triangle::isIntersected(const Ray& ray) const
{
   float distAlongRayToHit;

   if (triangleIsIntersected(ray, distAlongRayToHit, Point()))
   {
      return true;
   }

   return false;
}

// This function implements the triangle intersection algorithm
bool Triangle::triangleIsIntersected(const Ray& ray, float& distAlongRayToHit, Point& hitPoint) const
{
   float rayDirDotNormal = dot(ray.direction, faceNormal);

   // If the ray is perpendicular to the face normal of the triangle, the ray and the plane in which the triangle lies are parallel
   // For this reason, the ray cannot intersect the triangle
   if (rayDirDotNormal == 0)
   {
      return false;
   }

   distAlongRayToHit = (dot(vertexA, faceNormal) - dot(ray.origin, faceNormal)) / rayDirDotNormal;

   // If the distance along to the ray to the hit point is negative, the plane in which the triangle lies is behind the camera
   // Even if the ray does intersect the triangle, this is not considered a valid intersection because we are only interested in the triangles that are in front of the camera
   if (distAlongRayToHit < 0)
   {
      return false;
   }

   // We use barycentric coordinates to determine if the hit point lies inside the triangle
   hitPoint = (ray.direction * distAlongRayToHit) + ray.origin;

   /*
                C
               / \
              /   \
             /     \
            /   P   \
           /         \
          A-----------B
   */

   // Triangle ABC
   Vector normalOfTriABC = cross((vertexB - vertexA), (vertexC - vertexA));
   float areaOfTriABC = normalOfTriABC.length() / 2;

   // Sub-triangle BCP (P corresponds to the hit point)
   Vector normalOfSubTri = cross((vertexC - vertexB), (hitPoint - vertexB));
   float areaOfTriBCP = normalOfSubTri.length() / 2;

   if (dot(normalOfSubTri, normalOfTriABC) < 0)
   {
      return false;
   }

   // Sub-triangle ABP (P corresponds to the hit point)
   normalOfSubTri = cross((vertexB - vertexA), (hitPoint - vertexA));
   float areaOfTriABP = normalOfSubTri.length() / 2;

   if (dot(normalOfSubTri, normalOfTriABC) < 0)
   {
      return false;
   }

   // Sub-triangle CAP (P corresponds to the hit point)
   normalOfSubTri = cross((vertexA - vertexC), (hitPoint - vertexC));
   float areaOfTriCAP = normalOfSubTri.length() / 2;

   if (dot(normalOfSubTri, normalOfTriABC) < 0)
   {
      return false;
   }

   return true;
}

