#ifndef COLOUR_H
#define COLOUR_H

/*
Description:

   The Colour class is used to represent RGB colours.

   The RGB values can range from 0 to 1.

   After the RGB values of a pixel are determined, they are multiplied by 255 and clamped if necessary by the Film class.
   This is done because the FreeImage library requires colours to range from 0 to 255 in order to generate a PNG.
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

