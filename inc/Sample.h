#ifndef SAMPLE_H
#define SAMPLE_H

/*
Description:

   The Sample class is used to represent the coordinates of the points on the image plane through which we shoot rays from the camera.

   The center of the upper-left pixel has coordinates (0.5, 0.5).
   The center of the upper-right pixel has coordinates (width - 0.5, 0.5)
   The center of the lower-left pixel has coordinates (0.5, height - 0.5)
   The center of the lower-right pixel has coordinates (width - 0.5, height - 0.5)
*/

struct Sample
{
   Sample()
      : x(0)
      , y(0)
   { }

   Sample(float x, float y)
      : x(x)
      , y(y)
   { }

   float x, y;
};

#endif

