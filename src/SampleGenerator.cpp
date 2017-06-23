#include <iostream>

#include "SampleGenerator.h"

SampleGenerator::SampleGenerator(const int width, const int height)
   : width(width)
   , height(height)
   , x(0.5)
   , y(0.5)
{ }

SampleGenerator::~SampleGenerator()
{ }

// Samples are generated line by line from left to right, starting at the upper-left corner of the image plane
void SampleGenerator::generateSample(Sample& sample)
{
   // Update sample
   sample.x = x;
   sample.y = y;

   // Move towards the right
   x++;

   // When a horizontal line is completed, jump to the beginning of the next line
   if (x > width)
   {
      x = 0.5;
      y++;

      displayProgress(y);
   }
}

bool SampleGenerator::sampleIsAvailable()
{
   if (y < height)
   {
      return true;
   }
   return false;
}

void SampleGenerator::displayProgress(const float y)
{
   static bool beginning = true;

   // The initial goal is 10% of the image
   static float goalInPercent = static_cast<float>(0.1);
   static float goalInLines = height * goalInPercent;

   if (beginning)
   {
      std::cout << '\n' << " Your image is being generated." << '\n';
      beginning = false;
   }

   if (y >= goalInLines)
   {
      std::cout << '\n' << " " << goalInPercent * 100 << "% completed.";

      if (goalInPercent >= 1)
      {
         std::cout << '\n' << '\n' << " Your image is ready!" << '\n';
      }

      // Increase the goal by 10%
      goalInPercent += static_cast<float>(0.1);
      goalInLines = height * goalInPercent;
   }
}

