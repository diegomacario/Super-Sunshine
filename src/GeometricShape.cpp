#include "GeometricShape.h"
#include <iostream>

GeometricShape::GeometricShape(const Colour& ambient, Material* material)
   : ambient(ambient)
   , material(material)
	, textured(false)
{ }

GeometricShape::GeometricShape(TextureDescription* textureDescription, Material* material)
   : textureDescription(textureDescription)
   , material(material)
	, textured(true)
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

Colour GeometricShape::getAmbient(Point barycentricCoord) const
{
	Point uValues = Point(textureDescription->textureCoordA.u, textureDescription->textureCoordB.u, textureDescription->textureCoordC.u);
	Point vValues = Point(textureDescription->textureCoordA.v, textureDescription->textureCoordB.v, textureDescription->textureCoordC.v);

   float interpolatedU = dot(uValues, barycentricCoord);
   float interpolatedV = dot(vValues, barycentricCoord);

	TextureCoord interpolatedCoord(interpolatedU, interpolatedV);

   Colour colour = textureDescription->texture.sampleColour(interpolatedCoord);
   return colour;
}

Material * GeometricShape::getMaterial() const
{
   return material;
}

bool GeometricShape::isTextured() const
{
	return this->textured;
}
