#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
Description:

   The Constants namespace contains forward declarations to useful constants.
*/

namespace Constants
{
   extern const float pi;

   // The shadow bias is used to avoid self-shadowing when casting shadow rays
   // It is also isued to avoid self-reflection when casting reflection rays
   extern const float shadowBias;
}

#endif

