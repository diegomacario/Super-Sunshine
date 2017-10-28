#ifndef TEXTURE_DESCRIPTION_H
#define TEXTURE_DESCRIPTION_H

#include "TextureCoord.h"
#include "Texture.h"

struct TextureDescription
{
   TextureDescription(Texture& texture, TextureCoord textureCoordA, TextureCoord textureCoordB, TextureCoord textureCoordC)
      : texture(texture)
      , textureCoordA(textureCoordA)
      , textureCoordB(textureCoordB)
      , textureCoordC(textureCoordC)
   { }

	Texture texture;
	TextureCoord textureCoordA, textureCoordB, textureCoordC;
};

#endif

