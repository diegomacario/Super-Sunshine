#ifndef MATERIAL_H
#define MATERIAL_H

#include "Colour.h"
#include "Vector.h"

/*
Description:

   The Material class is used to store the parameters that define the response of a particular shape to light sources.

   The diffuse contribution of a light source is given by:
   diffuse * max(dot(lightVec, normalAtHit), 0)

   The specular contribution of a light source is given by:
   specular * (max(dot(halfVec, normalAtHit), 0))^shininess

   The Material class provides a function for computing and returning these two terms for a specific light source.
*/

class Material
{
public:

   Material(const Colour& diffuse, const Colour& specular, const Colour& emission, float shininess);
   ~Material();

   Colour getEmission();
   Colour getSpecular();
   Colour calculateDiffuseAndSpecularLighting(const Normal& normalAtHit, const Vector& lightVec, const Vector& halfVec);

private:

   Colour diffuse, specular, emission;
   float shininess;

};

#endif

