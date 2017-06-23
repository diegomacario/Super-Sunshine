#include "GeometricShape.h"

GeometricShape::GeometricShape(const Colour& ambient, Material* material)
   : ambient(ambient)
   , material(material)
{ }

GeometricShape::~GeometricShape()
{
   if (material != nullptr)
   {
      delete material;
   }
}

Colour GeometricShape::getAmbient() const
{
   return ambient;
}

Material * GeometricShape::getMaterial() const
{
   return material;
}

