#include "TextureCoord.h"

TextureCoord::TextureCoord()
   : u(0)
   , v(0)
{ }

TextureCoord::TextureCoord(float u, float v)
   : u(u)
   , v(v)
{ }

TextureCoord::~TextureCoord()
{ }

void TextureCoord::set(float u, float v)
{
   this->u = u;
   this->v = v;
}

