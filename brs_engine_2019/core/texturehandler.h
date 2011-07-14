#pragma once

#include "texture.h"
#include "image.h"

extern class TextureHandler g_textures;
class TextureParameters;
class Texture;

class TextureHandler
{
public:
	TextureHandler();
	~TextureHandler();

	void init();
	void clear();

	//textures
	void addTextureParameters(string name, TextureParameters* params);
	void loadImages();
	void uploadImages();
	void bindTexture(string name, int texunit = GL_TEXTURE0_ARB);
	void clearTextureUnits(); //for lazy progammers

	//images
	void addImage(string name, Image *image);
	Image& image(string name);

	//framebuffer objects
    void bindDepthFBO();
	void bindTextureFBO(string name);
    void unbindFBO();

	//debug
	void dumpUnusedImages();


private:

	static const int MAX_TEXTURES = 8;
	map<string, Image*> m_images;
	map<string, Texture*> m_textures;
	map<string, TextureParameters*> m_textureParameters;
	string m_lastBoundTexture[8];

	//rendertargets
	class FBOManager *m_FBO;

};