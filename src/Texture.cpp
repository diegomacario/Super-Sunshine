#include "Texture.h"
#include <iostream>


Texture::Texture()
{ }

void Texture::set(const char * textureFile)
{
	this->textureFile = textureFile;
	this->imageLoaded = loadImage();
}


Colour Texture::sampleColour(TextureCoord textureCoord)
{
	if(!isImageLoaded())
	{
		std::cout << "Texture::sampleColour: no image loaded";
		return Colour(0, 0, 0);
	}

	if(!validateCoords(textureCoord))
	{
		std::printf("Texture::sampleColour: textureCoord (%f, %f) is invalid (image dimension: (%u, %u)). \n", textureCoord.u*width, textureCoord.v*height, width, height);
		return Colour(0, 0, 0);
	}

	RGBQUAD color;
	FreeImage_GetPixelColor(image, textureCoord.u*width, textureCoord.v*height, &color);
	Colour colour = Colour(color.rgbRed/255.0, color.rgbGreen/255.0, color.rgbBlue/255.0);

	return colour;
}

bool Texture::validateCoords(TextureCoord textureCoord){
	return textureCoord.u >= 0 && textureCoord.u <= 1 && textureCoord.v >= 0 && textureCoord.v <= 1;
}

bool Texture::isImageLoaded()
{
	return this -> imageLoaded;
}

bool Texture::loadImage()
{

	FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(textureFile); 
	image = FreeImage_Load(format, textureFile);

	if(!image)
	{
		std::cerr << "Texture::loadImage: error loading image: " << textureFile << "\n";
		return false;
	}
	
	this -> imageLoaded = true;
	width = FreeImage_GetWidth(image); 
	height = FreeImage_GetHeight(image); 
	return true;
}

void Texture::unloadImage()
{
	if(!isImageLoaded()) return;

	FreeImage_Unload(image);
	this -> imageLoaded = false;
}

