#include <memory>
#include <iostream>

#include "FileParser.h"
#include "SampleGenerator.h"
#include "RayGenerator.h"
#include "Film.h"
#include "SceneDescription.h"
//#include "FreeImage.h"

bool validateCmdLineArguments(const int numOfArgs);

int main(int argc, char* argv[])
{
   if (!validateCmdLineArguments(argc))
   {
      return 1;
   }

   // Initialize the file parser with the name of the scene description file
   FileParser fileParser(argv[1]);

   std::unique_ptr<SceneDescription> sceneDesc = nullptr;
   std::unique_ptr<Scene> scene = nullptr;

   // Parse the scene description file
   // If the parsing succeeds, the sceneDesc and scene pointers are assigned dynamically allocated objects
   // that contain all the parameters specified by the user
   try
   {
      fileParser.readFile(sceneDesc, scene);
   }
   catch(const char* errorMsg)
   {
      std::cerr << errorMsg;
      return 1;
   }

   // Initialize the FreeImage library
   //FreeImage_Initialise();

   // The SampleGenerator updates a Sample object to specify the coordinates of the center of each pixel on the image plane
   SampleGenerator sampleGenerator(sceneDesc->getWidth(), sceneDesc->getHeight());

   // The RayGenerator updates a Ray object so that it passes through the center of each pixel on the image plane
   RayGenerator rayGenerator(sceneDesc->getWidth(),
                             sceneDesc->getHeight(),
                             sceneDesc->getEye(),
                             sceneDesc->getCenter(),
                             sceneDesc->getUpVec(),
                             sceneDesc->getFovy());

   // The Film object stores the final colour of each pixel and coverts it from the [0, 1.0] range to the [0, 255] range,
   // so that it can be used to generate an image with the FreeImage library
   Film film(sceneDesc->getWidth(), sceneDesc->getHeight());

   Sample sample;
   Ray ray(sceneDesc->getEye(), Vector(0, 0, 0));
   Intersection intersection;

   // Loop over all the pixels in the image plane
   while(sampleGenerator.sampleIsAvailable())
   {
      sampleGenerator.generateSample(sample);
      rayGenerator.generateRay(sample, ray);

      // If the current ray intersects an object, we calculate the lighting at the intersection point and store the colour
      // If not, the current pixel remains black
      if (scene->findNearestIntersection(ray, &intersection))
      {
         film.exposePixel(scene->calculateLightingAtIntersection(sceneDesc->getEye(), &intersection));
      }

      film.prepareNextPixel();
   }

   // Generate image
   //FIBITMAP * image = FreeImage_ConvertFromRawBits(film.getExposedFilm(),
   //                                                sceneDesc->getWidth(),
   //                                                sceneDesc->getHeight(),
   //                                                sceneDesc->getWidth() * 3,
   //                                                24,
   //                                                0xFF0000,
   //                                                0x00FF00,
   //                                                0x0000FF,
   //                                                true);

   // Save image
   //FreeImage_Save(FIF_PNG, image, sceneDesc->getOutputFilename().c_str(), 0);

   // Uninitialize the FreeImage library
   //FreeImage_DeInitialise();

   return 0;
}

bool validateCmdLineArguments(const int numOfArgs)
{
   if (numOfArgs < 2)
   {
      std::cout << " \nYou did not specify a scene description.\n";
      return false;
   }
   else if (numOfArgs > 2)
   {
      std::cout << " \nYou can only specify one scene description.\n";
      return false;
   }
   return true;
}

