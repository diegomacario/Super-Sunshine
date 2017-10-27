#include "GeometricShape.h"
#include <iostream>

GeometricShape::GeometricShape(const Colour& ambient, Material* material)
   : ambient(ambient)
   , material(material)
{ }

/*GeometricShape::GeometricShape(const Colour& textureA, const Colour& textureB, const Colour& textureC, Material* material)
   : textureA(textureA)
   , textureB(textureB)
   , textureC(textureC)
   , material(material)
{ }*/

GeometricShape::~GeometricShape()
{
   if (material != nullptr)
   {
      delete material;
   }
}

Colour GeometricShape::getAmbient() const
{
	//std::printf("(%f, %f, %f)\n", ambient.r, ambient.g, ambient.b);
   return ambient;
}

Material * GeometricShape::getMaterial() const
{
   return material;
}

