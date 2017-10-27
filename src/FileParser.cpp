#include <iostream>
#include <sstream>
#include <memory>

#include "Triangle.h"
#include "Sphere.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "FileParser.h"
#include "SceneDescription.h"

FileParser::FileParser(const char* filename)
{
   lineStream.open(filename);
}

FileParser::~FileParser()
{ }

void FileParser::readFile(std::unique_ptr<SceneDescription>& sceneDesc, std::unique_ptr<Scene>& scene)
{
   if (lineStream.is_open())
   {
      std::unique_ptr<ValidationFlags> validationFlags(new ValidationFlags());
      std::unique_ptr<FileParserState> state(new FileParserState());

      std::string line, cmd;
      bool cmdParsed = false;

      getline(lineStream, line);
      while (lineStream)
      {
         // Ignore blank/commented lines
         if ((line.find_first_not_of(" \t\r\n") != std::string::npos) && (line[0] != '#'))
         {
            std::stringstream wordStream(line);
            wordStream >> cmd;

            try
            {
               // Pre-validation
               parameterPreValidation(cmd, validationFlags, state);

               // The parseGeometryCommands() function is the only one out of the 5 parsing functions that can throw
               cmdParsed = parseGeometryCommands(cmd, wordStream, validationFlags, state);
            }
            catch(const char* errorMsg)
            {
               cleanExceptionExit(state);
               throw errorMsg;
            }

            if (!cmdParsed)
            {
               if (parseSetupCommands(cmd, wordStream, validationFlags, state))
               {
                  // Setup
               }
               else if (parseTransformationCommands(cmd, wordStream, state))
               {
                  // Transformation
               }
               else if (parseLightCommands(cmd, wordStream, state))
               {
                  // Light
               }
			   else if (parseTextureCommands(cmd, wordStream, validationFlags, state))
			   {
				  // Textures
			   }
               else if (parseMaterialCommands(cmd, wordStream, state))
               {
                  // Material
               }
               else
               {
                  // Unknown
                  std::cout << "\n Command \"" << cmd << "\" is unknown. It will be ignored.\n";
               }
            }
            else
            {
               cmdParsed = false;
            }

         }
         getline(lineStream, line);
      }

      try
      {
         // Post-validation
         parameterPostValidation(validationFlags);
      }
      catch(const char* errorMsg)
      {
         cleanExceptionExit(state);
         throw errorMsg;
      }

      // Create the SceneDescription and Scene objects
      sceneDesc.reset(new SceneDescription(state->width,
                                           state->height,
                                           state->outputFilename,
                                           state->eye,
                                           state->center,
                                           state->up,
                                           state->fovy));

      scene.reset(new Scene(state->objects,
                            state->lights,
                            state->maxDepth));
   }
   else
   {
      // No need to call cleanExceptionExit() since no objects have been dynamically allocated with raw pointers
      throw "\n Unable to open the scene description.\n";
   }
}

bool FileParser::parseSetupCommands(const std::string& cmd, std::stringstream& wordStream, std::unique_ptr<ValidationFlags>& validationFlags, std::unique_ptr<FileParserState>& state)
{
   // The values array is given a size of 10 because that is the maximum number
   // of parameters a setup command can have (in the case of the camera command)
   float values[10];
   bool validInput = false;

   if (cmd == "size")
   {
      validInput = readValues(cmd, wordStream, 2, values);
      if (validInput)
      {
         state->width = static_cast<int>(values[0]);
         state->height = static_cast<int>(values[1]);
         validationFlags->sizeIsSpecified = true;
      }
      return true;
   }
   else if (cmd == "maxdepth")
   {
      validInput = readValues(cmd, wordStream, 1, values);
      if (validInput)
      {
         state->maxDepth = static_cast<int>(values[0]);
         validationFlags->maxDepthIsSpecified = true;
      }
      return true;
   }
   else if (cmd == "output")
   {
      wordStream >> state->outputFilename;
      if (wordStream.fail())
      {
         std::cout << "\n Could not read the output filename.\n";
      }
      else
      {
         validationFlags->outputIsSpecified = true;
      }
      return true;
   }
   else if (cmd == "camera")
   {
      validInput = readValues(cmd, wordStream, 10, values);
      if (validInput)
      {
         state->eye.set(values[0], values[1], values[2]);
         state->center.set(values[3], values[4], values[5]);
         state->up.set(values[6], values[7], values[8]);
         state->fovy = values[9];
         validationFlags->cameraIsSpecified = true;
      }
      return true;
   }

   return false;
}

bool FileParser::parseGeometryCommands(const std::string& cmd, std::stringstream& wordStream, std::unique_ptr<ValidationFlags>& validationFlags, std::unique_ptr<FileParserState>& state)
{
   // The values array is given a size of 6 because that is the maximum number
   // of parameters a geometry command can have (in the case of the textureTri command)
   float values[6];
   bool validInput = false;

   if (cmd == "sphere")
   {
      validInput = readValues(cmd, wordStream, 4, values);
      if (validInput)
      {
         state->objects.push_back(new Sphere(Point(values[0], values[1], values[2]), 
                                                   values[3],
                                                   state->objToWorldTransfStack.top(),
                                                   state->worldToObjTransfStack.top(),
                                                   state->ambient,
                                                   new Material(state->diffuse, state->specular, state->emission, state->shininess)));
      }
      return true;
   }
   else if (cmd == "maxverts")
   {
      validInput = readValues(cmd, wordStream, 1, values);
      if (validInput)
      {
         state->maxVerts = static_cast<int>(values[0]);
         validationFlags->maxVertsIsSpecified = true;
      }
      return true;
   }
   else if ((cmd == "vertex") && (state->vertexCounter < state->maxVerts))
   {
      validInput = readValues(cmd, wordStream, 3, values);
      if (validInput)
      {
         state->vertices.push_back(Point(values[0], values[1], values[2]));
         state->vertexCounter++;
      }
      return true;
   }
   else if (cmd == "tri")
   {
      validInput = readValues(cmd, wordStream, 3, values);
      if (validInput)
      {
         int vertA = static_cast<int>(values[0]);
         int vertB = static_cast<int>(values[1]);
         int vertC = static_cast<int>(values[2]);

         if (((vertA >= state->maxVerts) || (vertA < 0)) || ((vertB >= state->maxVerts) || (vertB < 0)) || ((vertC >= state->maxVerts) || (vertC < 0)))
         {
            throw "\n A triangle was specified with a nonexistent vertex.\n";
         }

         // The vertices of a triangle are transformed by M before being stored
         state->objects.push_back(new Triangle(state->objToWorldTransfStack.top() * state->vertices[vertA],
                                               state->objToWorldTransfStack.top() * state->vertices[vertB],
                                               state->objToWorldTransfStack.top() * state->vertices[vertC],
                                               state->ambient,
                                               new Material(state->diffuse, state->specular, state->emission, state->shininess)));
      }
      return true;
   }
   else if (cmd == "texturedTri")
   {
		validInput = readValues(cmd, wordStream, 6, values);
		if (validInput)
		{
         int vertA = static_cast<int>(values[0]);
         int vertB = static_cast<int>(values[2]);
         int vertC = static_cast<int>(values[4]);

         if (((vertA >= state->maxVerts) || (vertA < 0)) || ((vertB >= state->maxVerts) || (vertB < 0)) || ((vertC >= state->maxVerts) || (vertC < 0)))
         {
            throw "\n A triangle was specified with a nonexistent vertex.\n";
         }

			int textureA = static_cast<int>(values[1]);
			int textureB = static_cast<int>(values[3]);
			int textureC = static_cast<int>(values[5]);

		   state->objects.push_back(new Triangle(state->objToWorldTransfStack.top() * state->vertices[vertA],
                                             state->objToWorldTransfStack.top() * state->vertices[vertB],
                                             state->objToWorldTransfStack.top() * state->vertices[vertC],
															state->textureCoords[textureA],
															state->textureCoords[textureB],
															state->textureCoords[textureC],
															&state->texture,
															new Material(state->diffuse, state->specular, state->emission, state->shininess)));

		}
		return true;
   }

   return false;
}

bool FileParser::parseTransformationCommands(const std::string& cmd, std::stringstream& wordStream, std::unique_ptr<FileParserState>& state)
{
   // The values array is given a size of 4 because that is the maximum number
   // of parameters a transformation command can have (in the case of the rotate command)
   float values[4];
   bool validInput = false;

   if (cmd == "translate")
   {
      validInput = readValues(cmd, wordStream, 3, values);
      if (validInput)
      {
         Affine translationMatrix(values[0], values[1], values[2], TRANSLATION);
         state->objToWorldTransfStack.top() = state->objToWorldTransfStack.top() * translationMatrix;
         state->worldToObjTransfStack.top() = translationMatrix.invert() * state->worldToObjTransfStack.top();
      }
      return true;
   }
   else if (cmd == "scale")
   {
      validInput = readValues(cmd, wordStream, 3, values);
      if (validInput)
      {
         Affine scaleMatrix(values[0], values[1], values[2], SCALE);
         state->objToWorldTransfStack.top() = state->objToWorldTransfStack.top() * scaleMatrix;
         state->worldToObjTransfStack.top() = scaleMatrix.invert() * state->worldToObjTransfStack.top();
      }
      return true;
   }
   else if (cmd == "rotate")
   {
      validInput = readValues(cmd, wordStream, 4, values);
      if (validInput)
      {
         Affine rotationMatrix(values[0], values[1], values[2], values[3]);
         state->objToWorldTransfStack.top() = state->objToWorldTransfStack.top() * rotationMatrix;
         state->worldToObjTransfStack.top() = rotationMatrix.invert() * state->worldToObjTransfStack.top();
      }
      return true;
   }
   else if (cmd == "pushTransform")
   {
      state->objToWorldTransfStack.push(state->objToWorldTransfStack.top());
      state->worldToObjTransfStack.push(state->worldToObjTransfStack.top());
      return true;
   }
   else if (cmd == "popTransform")
   {
      state->objToWorldTransfStack.pop();
      state->worldToObjTransfStack.pop();
      return true;
   }

   return false;
}

bool FileParser::parseLightCommands(const std::string& cmd, std::stringstream& wordStream, std::unique_ptr<FileParserState>& state)
{
   // The values array is given a size of 6 because that is the maximum number
   // of parameters a light command can have (in the case of the directional and point commands)
   float values[6];
   bool validInput = false;

   if (cmd == "directional")
   {
      validInput = readValues(cmd, wordStream, 6, values);
      if (validInput)
      {
         state->lights.push_back(new DirectionalLight(Vector(values[0], values[1], values[2]), Colour(values[3], values[4], values[5])));
      }
      return true;
   }
   else if (cmd == "point")
   {
      validInput = readValues(cmd, wordStream, 6, values);
      if (validInput)
      {
         state->lights.push_back(new PointLight(Point(values[0], values[1], values[2]), state->attenuation, Colour(values[3], values[4], values[5])));
      }
      return true;
   }
   else if (cmd == "attenuation")
   {
      validInput = readValues(cmd, wordStream, 3, values);
      if (validInput)
      {
         state->attenuation.set(values[0], values[1], values[2]);
      }
      return true;
   }
   else if (cmd == "ambient")
   {
      validInput = readValues(cmd, wordStream, 3, values);
      if (validInput)
      {
         state->ambient.set(values[0], values[1], values[2]);
      }
      return true;
   }

   return false;
}

bool FileParser::parseTextureCommands(const std::string& cmd, std::stringstream& wordStream, std::unique_ptr<ValidationFlags>& validationFlags, std::unique_ptr<FileParserState>& state)
{
	float values[2];
	bool validInput = false;


	if (cmd == "texture")
	{
		state->texture.unloadImage();
		std::string file;
		wordStream >> file;
		state->texture.set(file.c_str());

		if (!state->texture.isImageLoaded())
		{
			std::cout << "\n Could not read the texture file.\n";
			validationFlags->textureIsSpecified = false;
			return false;
		}
        validationFlags->textureIsSpecified = true;
		return true;
	}
	else if (cmd == "textureCoord")
	{
		validInput = readValues(cmd, wordStream, 2, values);
		if (validInput)
		{
			state->textureCoords.push_back(TextureCoord(values[0], values[1]));
			Colour colour = state->texture.sampleColour(TextureCoord(values[0], values[1]));
			state->ambient.set(colour.r, colour.g, colour.b);
		}
		return true;
	}
	return false;
}

bool FileParser::parseMaterialCommands(const std::string& cmd, std::stringstream& wordStream, std::unique_ptr<FileParserState>& state)
{
   // The values array is given a size of 3 because that is the maximum number
   // of parameters a material command can have (in the case of the diffuse, specular and emission commands)
   float values[6];
   bool validInput = false;

   if (cmd == "diffuse")
   {
      validInput = readValues(cmd, wordStream, 3, values);
      if (validInput)
      {
         state->diffuse.set(values[0], values[1], values[2]);
      }
      return true;
   }
   else if (cmd == "specular")
   {
      validInput = readValues(cmd, wordStream, 3, values);
      if (validInput)
      {
         state->specular.set(values[0], values[1], values[2]);
      }
      return true;
   }
   else if (cmd == "shininess")
   {
      validInput = readValues(cmd, wordStream, 1, values);
      if (validInput)
      {
         state->shininess = values[0];
      }
      return true;
   }
   else if (cmd == "emission")
   {
      validInput = readValues(cmd, wordStream, 3, values);
      if (validInput)
      {
         state->emission.set(values[0], values[1], values[2]);
      }
      return true;
   }

   return false;
}

bool FileParser::readValues(const std::string& cmd, std::stringstream& wordStream, const int numValues, float* values)
{
   for (int i = 0; i < numValues; i++)
   {
      wordStream >> values[i];
      if (wordStream.fail())
      {
         std::cout << "\n Failed reading value " << i << " of a \"" << cmd << "\" command. It will be skipped.\n";
         return false;
      }
   }
   return true;
}

void FileParser::parameterPreValidation(std::string cmd, std::unique_ptr<ValidationFlags>& validationFlags, const std::unique_ptr<FileParserState>& state)
{
   // The "size" command must be the first command specified in the scene description
   if (validationFlags->firstLine)
   {
      if (!(cmd == "size"))
      {
         throw "\n The size must be specified before any other parameters.\n";
      }
      validationFlags->firstLine = false;
   }

   // "size" can only be specified once
   else if (validationFlags->sizeIsSpecified && (cmd == "size"))
   {
      throw "\n The size can only be specified once.\n";
   }

   // "maxdepth" can only be specified once
   else if (validationFlags->maxDepthIsSpecified && (cmd == "maxdepth"))
   {
      throw "\n The maximum depth can only be specified once.\n";
   }

   // "output" can only be specified once
   else if (validationFlags->outputIsSpecified && (cmd == "output"))
   {
      throw "\n The output filename can only be specified once.\n";
   }

   // "camera" can only be specified once
   else if (validationFlags->cameraIsSpecified && (cmd == "camera"))
   {
      throw "\n The camera description can only be specified once.\n";
   }

   // "maxverts" can only be specified once
   else if (validationFlags->maxVertsIsSpecified && (cmd == "maxverts"))
   {
      throw "\n The maximum number of vertices can only be specified once.\n";
   }

   // "maxverts" must be specified before any vertices are defined
   else if (!validationFlags->maxVertsIsSpecified && (cmd == "vertex"))
   {
      throw "\n The maximum number of vertices must be specified before any vertices are defined.\n";
   }

   else if (validationFlags->maxVertsIsSpecified && (cmd == "vertex") && (state->vertexCounter >= state->maxVerts))
   {
      throw "\n The maximum number of vertices has been exceeded.\n";
   }

   else if (cmd == "popTransform" && (state->objToWorldTransfStack.size() <= 1))
   {
      throw "\n An attempt to pop a transformation was made while the transformation stack was empty.\n";
   }
   // "texture" must be specified before any textureCoords are defined
   else if (!validationFlags->textureIsSpecified && (cmd == "textureCoord"))
   {
	  throw "\n A texture file must be specified before any texture coordinates are defined.\n";
   }

}

void FileParser::parameterPostValidation(const std::unique_ptr<ValidationFlags>& validationFlags)
{
   // "output" must be specified
   if (!validationFlags->outputIsSpecified)
   {
      throw "\n The output filename must be specified.\n";
   }

   // "camera" must be specified
   else if (!validationFlags->cameraIsSpecified)
   {
      throw "\n The camera description must be specified.\n";
   }
}

void FileParser::cleanExceptionExit(std::unique_ptr<FileParserState>& state)
{
   // If an exception gets thrown, the pointers to the shapes and the lights that were created dynamically will not be copied into a Scene object
   // In this situation, the destructor of the Scene object will not get the chance to delete those shapes/lights once it is done using them
   // For this reason, we delete them by calling this function before rethrowing the exception

   for (std::vector<GeometricShape*>::iterator it = state->objects.begin(); it != state->objects.end(); ++it)
   {
      delete *it;
   }

   state->objects.clear();

   for (std::vector<Light*>::iterator it = state->lights.begin(); it != state->lights.end(); ++it)
   {
      delete *it;
   }

   state->lights.clear();
}

FileParser::ValidationFlags::ValidationFlags()
   : firstLine(true)
   , sizeIsSpecified(false)
   , maxDepthIsSpecified(false)
   , outputIsSpecified(false)
   , cameraIsSpecified(false)
   , maxVertsIsSpecified(false)
   , textureIsSpecified(false)
{ }

FileParser::FileParserState::FileParserState()
   : width(0)
   , height(0)
   , maxDepth(5) // The maximum depth is equal to 5 by default
   , outputFilename()
   , eye()
   , center()
   , up()
   , fovy(0)
   , maxVerts(0)
   , vertexCounter(0)
   , vertices()
   , objects()
   , objToWorldTransfStack()
   , worldToObjTransfStack()
   , attenuation()
   , ambient(static_cast<float>(0.2)) // The ambient illumination is equal to (0.2, 0.2, 0.2) by default
   , lights()
   , texture()
   , textureCoords()
   , diffuse()
   , specular()
   , emission()
   , shininess(0)
{
   // Initialize both stacks with unit matrices
   objToWorldTransfStack.push(Affine(1, 1, 1, SCALE));
   worldToObjTransfStack.push(Affine(1, 1, 1, SCALE));
}

