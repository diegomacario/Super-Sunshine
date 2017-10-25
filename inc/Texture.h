#ifndef TEXTURE_H
#define TEXTURE_H

#include "TextureCoords.h"
#include "Colour.h"
#include "FreeImage.h"


class Texture 
{
public:

	Texture(char * textureFile, TextureCoords * textureCoords);

	Colour getColour(float u, float v);

	char * textureFile;
	TextureCoords * coords;

private:
	// image
	FIBITMAP * image;

	// Read image file
	bool loadImage();
	
};

#endif
