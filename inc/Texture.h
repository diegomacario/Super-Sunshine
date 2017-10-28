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

	Colour sampleColour(TextureCoord textureCoord) const;

	void unloadImage();
	bool isImageLoaded() const;

private:

	bool imageLoaded = false;
	bool loadImage();

	FIBITMAP * image;
	unsigned height, width;

	bool validateCoords(TextureCoord textureCoord) const;

	const char * textureFile;

};

#endif
