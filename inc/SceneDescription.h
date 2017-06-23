#ifndef SCENE_DESCRIPTION_H
#define SCENE_DESCRIPTION_H

#include <string>

#include "Point.h"
#include "Vector.h"

/*
Description:

   The SceneDescription class is used to store the dimensions of the scene and the camera parameters.
   It also stores the name of the image that will be generated when the program finishes executing.

   Just like the Scene class, it is created by the FileParser using the parameters specified in the scene description file. It is intended to be read-only.
*/

class SceneDescription
{
public:

   SceneDescription(int width, int height, const std::string& outputFilename, const Point& eye, const Point& center, const Vector& up, float fovy);
   ~SceneDescription();

   int getWidth();
   int getHeight();
   std::string getOutputFilename();
   Point getEye();
   Point getCenter();
   Vector getUpVec();
   float getFovy();

private:

   int width, height;
   std::string outputFilename;
   Point eye, center;
   Vector up;
   float fovy;
};

#endif
