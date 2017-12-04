#include "Texture.h"
#include <iostream>

Texture::Texture()
{ }

Texture::~Texture()
{ }

void Texture::set(const char * textureFile)
{
   this->textureFile = textureFile;
   this->imageLoaded = loadImage();
}

Colour Texture::sampleColour(const TextureCoord& textureCoord) const
{
   if(!isImageLoaded())
   {
      std::cout << "\n Texture::sampleColour: Could not get the texture colour at the specified texture coordinates because no texture file has been loaded.\n";
      return Colour(0, 0, 0);
   }

   if(!validateCoords(textureCoord))
   {
      std::printf("\n Texture::sampleColour: The texture coordinate (%f, %f) is invalid (the texture file has dimensions (%u, %u)). \n", textureCoord.u * width, textureCoord.v * height, width, height);
      return Colour(0, 0, 0);
   }

   RGBQUAD sampleColour;
   FreeImage_GetPixelColor(image, static_cast<unsigned int>(textureCoord.u * width), static_cast<unsigned int>(textureCoord.v * height), &sampleColour);

   return Colour(sampleColour.rgbRed / 255.0f, sampleColour.rgbGreen / 255.0f, sampleColour.rgbBlue / 255.0f);
}

bool Texture::validateCoords(const TextureCoord& textureCoord) const
{
   return ((textureCoord.u >= 0) && (textureCoord.u <= 1) && (textureCoord.v >= 0) && (textureCoord.v <= 1));
}

bool Texture::isImageLoaded() const
{
   return this->imageLoaded;
}

bool Texture::loadImage()
{
   FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(textureFile);
   image = FreeImage_Load(format, textureFile);

   if(!image)
   {
      std::cerr << "\n Texture::loadImage: Error loading the following texture file: " << textureFile << "\n";
      return false;
   }

   imageLoaded = true;
   width = FreeImage_GetWidth(image);
   height = FreeImage_GetHeight(image);
   return true;
}

void Texture::unloadImage()
{
   if (isImageLoaded())
   {
      FreeImage_Unload(image);
      this->imageLoaded = false;
   }
}

