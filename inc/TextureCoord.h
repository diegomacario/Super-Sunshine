#ifndef TEXTURECOORD_H
#define TEXTURECOORD_H

class TextureCoord
{
public:

   TextureCoord();
   TextureCoord(float u, float v);
   ~TextureCoord();

   void set(float u, float v);

   float u, v;
};

#endif

