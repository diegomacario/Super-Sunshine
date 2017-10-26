#ifndef TEXTURECOORD_H
#define TEXTURECOORD_H

struct TextureCoord
{
	TextureCoord()
		: u()
		, v()
	{ }

	TextureCoord(float u, float v)
		: u(u)
		, v(v)
	{ }


	void set(float u, float v)
	{
		TextureCoord(u, v);
	}

	float u;
	float v;
};

#endif
