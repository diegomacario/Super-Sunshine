#ifndef COLOUR_H
#define COLOUR_H

/*
Description:

   The Colour class is used to represent RGB colours.

   The RGB values can range from 0 to 1.

   After the RGB values of a pixel are determined, they are multiplied by 255 and clamped if necessary by the Film class.
   This is done because the stb_image_write library requires colours to range from 0 to 255 in order to generate a PNG.

Possible improvements:

   - Currently, users can only specify colours using the [0, 1.0] range. It would be great if users could choose between
     that range and the [0, 255] range, since most of the websites that provide colour palettes specify the colours using
     the latter range.
*/

class Colour
{
public:

   Colour();
   Colour(float c);
   Colour(float r, float g, float b);
   ~Colour();

   void set(float r, float g, float b);

   const Colour operator+(const Colour& rhsCol) const;
   const Colour operator-(const Colour& rhsCol) const;
   const Colour operator*(const Colour& rhsCol) const;

   const Colour operator*(const float scalar) const;
   const Colour operator/(const float scalar) const;

   float r, g, b;
};

const Colour operator*(float scalar, const Colour& rhsCol);

#endif

