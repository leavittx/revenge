#include "texture.h"


TextureParameters::TextureParameters()
{
	setDefaults();
}
TextureParameters::~TextureParameters()
{
}


void TextureParameters::setDefaults()
{
	m_upload = true;
	m_retain = false;
	m_linear = true;
	m_repeat = true;
	m_mipmap = false;

}

Texture::Texture()
{
	m_ID = -1;
	m_hasBeenBound = false;
}

Texture::~Texture()
{
}

bool Texture::hasBeenUsed()
{
	return m_hasBeenBound && m_params.m_upload;
}

void Texture::bind(int texunit)
{
	glActiveTexture(texunit);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	m_hasBeenBound = true;
}
void Texture::upload(Image &sourceImage, TextureParameters &params)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	int width = sourceImage.getWidth();
	int height = sourceImage.getHeight();

	if(((width == 1024 && height == 1024) ||
	   (width == 512 && height == 512) || 
	   (width == 256 && height == 256) || 
	   (width == 128 && height == 128) || 
	   (width == 64  && height == 64) ||
	   (width == 32  && height == 32)) && !params.m_mipmap)
	{
		int filter = params.m_linear ? GL_LINEAR : GL_NEAREST;
		int wrap = GL_REPEAT;//params.m_repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, wrap);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sourceImage.getData());
	}
	else
	{
		int wrap = GL_REPEAT;//params.m_repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, wrap);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, GL_UNSIGNED_BYTE, sourceImage.getData()); 
	}
	m_params = params;
}
