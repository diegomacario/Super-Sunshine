#include "SceneDescription.h"

SceneDescription::SceneDescription(int width, int height, const std::string& outputFilename, const Point& eye, const Point& center, const Vector& up, float fovy)
   : width(width)
   , height(height)
   , outputFilename(outputFilename)
   , eye(eye)
   , center(center)
   , up(up)
   , fovy(fovy)
{ }

SceneDescription::~SceneDescription()
{ }

int SceneDescription::getWidth()
{
   return width;
}

int SceneDescription::getHeight()
{
   return height;
}

std::string SceneDescription::getOutputFilename()
{
   return outputFilename;
}

Point SceneDescription::getEye()
{
   return eye;
}

Point SceneDescription::getCenter()
{
   return center;
}

Vector SceneDescription::getUpVec()
{
   return up;
}

float SceneDescription::getFovy()
{
   return fovy;
}

