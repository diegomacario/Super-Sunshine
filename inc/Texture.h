#ifndef TEXTURE_H
#define TEXTURE_H

#include "TextureCoord.h"
#include "Colour.h"
#include "FreeImage.h"


class Texture 
{
public:

	Texture();
	void set(const char * textureFile);

	Colour sampleColour(TextureCoord textureCoord);

	void unloadImage();
	bool isImageLoaded();

private:

	bool imageLoaded = false;
	bool loadImage();

	FIBITMAP * image;
	unsigned height, width;

	bool validateCoords(TextureCoord textureCoord);

	const char * textureFile;

};

#endif
