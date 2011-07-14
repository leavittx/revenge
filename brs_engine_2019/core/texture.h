#pragma once

#include "../globals.h"

class Image;

class TextureParameters
{
public:
	TextureParameters();
	~TextureParameters();
	void init(string name);

	bool m_upload;
	bool m_retain;
	bool m_linear;
	bool m_repeat;
	bool m_mipmap;
	void setDefaults();

private:
};


class Texture
{
public:
	Texture();
	~Texture();

	GLuint getID() { return m_ID; };
	void upload(Image &sourceimage, TextureParameters &params);
	void bind(int texunit = 0);
	bool hasBeenUsed();
private:

	TextureParameters m_params;
	GLuint m_ID;
	bool m_hasBeenBound;

};

