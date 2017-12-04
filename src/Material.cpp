#include <algorithm>
#include <math.h>

#include "Vector.h"
#include "Material.h"

Material::Material(const Colour& diffuse, const Colour& specular, const Colour& emission, float shininess)
   : diffuse(diffuse)
   , specular(specular)
   , emission(emission)
   , shininess(shininess)
{ }

Material::~Material()
{ }

Colour Material::getEmission()
{
   return emission;
}

Colour Material::getSpecular()
{
   return specular;
}

Colour Material::calculateDiffuseAndSpecularLighting(const Normal& normalAtHit, const Vector& lightVec, const Vector& halfVec)
{
   // The first term below corresponds to the diffuse contribution, while the second one corresponds to the specular contribution
   return ((diffuse * std::max(dot(lightVec, normalAtHit), 0.0f)) + (specular * std::pow(std::max(dot(halfVec, normalAtHit), 0.0f), shininess)));
}

