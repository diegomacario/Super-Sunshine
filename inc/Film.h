#ifndef FILM_H
#define FILM_H

#include "Colour.h"

/*
Description:

   The film class is used to store the RGB values of each pixel on the screen.

   It receives the final colour of each pixel and converts it to the 0 to 255 range, which is required by the stb_image_write library.

   Colours are stored sequentially in the array that will be used to generate the final image. Every three values correspond to one pixel.
*/

class Film
{
public:

   Film(const int width, const int height);
   ~Film();

   void exposePixel(Colour pixelColour);
   void prepareNextPixel();
   unsigned char * const getExposedFilm() const;

private:

   unsigned char * pixels;
   unsigned int pixelCounter;
};

#endif

