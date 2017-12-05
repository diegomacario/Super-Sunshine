#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <memory>
#include <stack>
#include <fstream>

#include "Affine.h"
#include "TextureCoord.h"
#include "Texture.h"
#include "Scene.h"

/*
Description:

   The FileParser class is used to read the scene description provided by the user.

   Its constructor requires the name of the file that contains the scene description.
   It provides a function to parse said file and dynamically allocate a SceneDescription object and a Scene object.
*/

class SceneDescription;

class FileParser
{
public:

   FileParser(const char* filename);
   ~FileParser();

   void readFile(std::unique_ptr<SceneDescription>& sceneDesc, std::unique_ptr<Scene>& scene);

private:

   struct ValidationFlags
   {
      ValidationFlags();

      bool firstLine;
      bool sizeIsSpecified;
      bool outputIsSpecified;
      bool cameraIsSpecified;
      bool maxVertsIsSpecified;
      bool maxDepthIsSpecified;
      bool textureIsSpecified;
   };

   // The destructor of a FileParserState object does not delete
   // the dynamically allocated shapes/lights it has pointers to because the pointers to those shapes/lights
   // should always be copied to a Scene object, which does delete them in its destructor
   // If an exception is thrown before those pointers are copied, we call cleanExceptionExit() before rethrowing
   struct FileParserState
   {
      FileParserState();

      // Setup
      int width, height, maxDepth;
      std::string outputFilename;
      Point eye, center;
      Vector up;
      float fovy;

      // Geometry
      int maxVerts, vertexCounter;
      std::vector<Point> vertices;
      std::vector<GeometricShape*> objects;

      // Transformations
      std::stack<Affine> objToWorldTransfStack;
      std::stack<Affine> worldToObjTransfStack;

      // Lights
      Attenuation attenuation;
      Colour ambient;
      std::vector<Light*> lights;

      // Materials
      Colour diffuse;
      Colour specular;
      Colour emission;
      float shininess;

      // Textures
      Texture texture;
      std::vector<TextureCoord> textureCoords;
   };

   bool parseSetupCommands(const std::string& cmd, std::stringstream& wordStream, std::unique_ptr<ValidationFlags>& validationFlags, std::unique_ptr<FileParserState>& state);
   bool parseGeometryCommands(const std::string& cmd, std::stringstream& wordStream, std::unique_ptr<ValidationFlags>& validationFlags, std::unique_ptr<FileParserState>& state);
   bool parseTransformationCommands(const std::string& cmd, std::stringstream& wordStream, std::unique_ptr<FileParserState>& state);
   bool parseLightCommands(const std::string& cmd, std::stringstream& wordStream, std::unique_ptr<FileParserState>& state);
   bool parseMaterialCommands(const std::string& cmd, std::stringstream& wordStream, std::unique_ptr<FileParserState>& state);
   bool parseTextureCommands(const std::string& cmd, std::stringstream& wordStream, std::unique_ptr<ValidationFlags>& validationFlags, std::unique_ptr<FileParserState>& state);

   bool readValues(const std::string& cmd, std::stringstream& wordStream, const int numValues, float* values);

   void parameterPreValidation(std::string cmd, std::unique_ptr<ValidationFlags>& validationFlags, const std::unique_ptr<FileParserState>& state);
   void parameterPostValidation(const std::unique_ptr<ValidationFlags>& validationFlags);

   void cleanExceptionExit(std::unique_ptr<FileParserState>& state);

   std::ifstream lineStream;
};

#endif

