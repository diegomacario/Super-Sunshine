#ifndef TEXTURECOORDS_H
#define TEXTURECOORDS_H

struct TextureCoords
{
	TextureCoords()
		: u()
		, v()
	{ }

	TextureCoords(float u, float v)
		: u(u)
		, v(v)
	{ }

	float u;
	float v;
};

#endif
