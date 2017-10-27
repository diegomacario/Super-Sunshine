#include <cmath>

#include "Constants.h"
#include "RayGenerator.h"

RayGenerator::RayGenerator(const int width, const int height, const Point& eye, const Point& center, const Vector& up, float fovy)
   : width(width)
   , height(height)
   , eye(eye)
{
   float fovyInRadians = (fovy * Constants::pi) / 180;

   // Set beta equal to tan(fovy / 2) and alpha equal to tan(fovx / 2)
   // Those values are the constants in their complete expressions
   beta = static_cast<float>(tan(fovyInRadians / 2));
   alpha = beta * (static_cast<float>(width) / static_cast<float>(height));

   // These vectors form the basis of the camera's coordinate frame.
   w = (eye - center).normalize();
   u = (cross(up, w)).normalize();
   v = cross(w, u);
}

RayGenerator::~RayGenerator()
{ }

void RayGenerator::generateRay(const Sample& sample, Ray& ray)
{
   // The origin of the ray has already been set as the eye
   // The direction of the ray is calculated with the expression below
   ray.direction = ((alpha * (((2 * sample.x) / width) - 1) * u) + (beta * (1 - ((2 * sample.y) / height)) * v) - w).normalize();
}

