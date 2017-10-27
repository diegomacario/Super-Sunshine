#include "GeometricShape.h"
#include <iostream>

GeometricShape::GeometricShape(const Colour& ambient, Material* material)
   : ambient(ambient)
   , material(material)
{ }

GeometricShape::GeometricShape(const TextureCoord& textureA, const TextureCoord& textureB, const TextureCoord& textureC, Texture* texture, Material* material)
   : textureA(textureA)
   , textureB(textureB)
   , textureC(textureC)
	, texture(texture)
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

Colour GeometricShape::getAmbient(Point hitPoint) const
{
	/*if(hitPoint.x < 0 || hitPoint.y < 0 || hitPoint.z < 0 || hitPoint.x > 1 || hitPoint.y > 1 || hitPoint.z > 1)
	{
		return Colour(0);
	}*/
	Point uValues = Point(textureA.u, textureB.u, textureC.u);
	Point vValues = Point(textureA.v, textureB.v, textureC.v);

	//std::printf("u: (%f, %f, %f) v: (%f, %f, %f)\n",textureA.u, textureB.u, textureC.u, textureA.v, textureB.v, textureC.v);
	//std::printf("hitpoint (%f, %f, %f)\n", hitPoint.x, hitPoint.y, hitPoint.z);

   float interpolatedU = dot(uValues, hitPoint);
   float interpolatedV = dot(vValues, hitPoint);

	TextureCoord interpolatedCoord(interpolatedU, interpolatedV);

   Colour colour = texture->sampleColour(interpolatedCoord);
   return colour;
}

Material * GeometricShape::getMaterial() const
{
   return material;
}

