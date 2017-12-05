#ifndef TEXTURE_H
#define TEXTURE_H

#include "TextureCoord.h"
#include "Colour.h"
#include "FreeImage.h"

class Texture
{
public:

   Texture();
   ~Texture();

   void set(const char * textureFile);

   bool isImageLoaded() const;
   void unloadImage();

   Colour sampleColour(const TextureCoord& textureCoord) const;

private:

   const char * textureFile;
   FIBITMAP * image;
   unsigned int height, width;
   bool imageLoaded = false;

   bool loadImage();

   bool validateCoords(const TextureCoord& textureCoord) const;
};

#endif

