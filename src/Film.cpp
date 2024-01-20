#include <algorithm>
#include <string.h>

#include "Colour.h"
#include "Film.h"

Film::Film(const int width, const int height)
   : pixels(new unsigned char[width * height * 3])
   , pixelCounter(0)
{
   memset(pixels, 0, width * height * sizeof(pixels[0]));
}

Film::~Film()
{
   delete[] pixels;
}

void Film::exposePixel(Colour pixelColour)
{
   pixels[pixelCounter] = static_cast<unsigned int>(std::min(255 * pixelColour.r, 255.0f));
   pixels[pixelCounter + 1] = static_cast<unsigned int>(std::min(255 * pixelColour.g, 255.0f));
   pixels[pixelCounter + 2] = static_cast<unsigned int>(std::min(255 * pixelColour.b, 255.0f));
}

void Film::prepareNextPixel()
{
   pixelCounter += 3;
}

unsigned char * const Film::getExposedFilm() const
{
   return pixels;
}

