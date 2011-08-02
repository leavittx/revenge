#include "cubemap.h"
/*
CubeMap::CubeMap()
{
}
CubeMap::~CubeMap()
{
}
void CubeMap::upload(Image &sourceimageTop,
					 Image &sourceimageBottom,
					 Image &sourceimageLeft,
					 Image &sourceimageRight,
					 Image &sourceimageFront,
					 Image &sourceimageBack,
					 TextureParameters &params)
{
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, m_ID);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, sourceimageRight.getWidth(), sourceimageRight.getHeight(), 0,
				 GL_RGB, GL_UNSIGNED_BYTE, sourceimageRight.getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, sourceimageLeft.getWidth(), sourceimageLeft.getHeight(), 0,
				 GL_RGB, GL_UNSIGNED_BYTE, sourceimageLeft.getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, sourceimageTop.getWidth(), sourceimageTop.getHeight(), 0,
				 GL_RGB, GL_UNSIGNED_BYTE, sourceimageTop.getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, sourceimageBottom.getWidth(), sourceimageBottom.getHeight(), 0,
				 GL_RGB, GL_UNSIGNED_BYTE, sourceimageBottom.getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, sourceimageBack.getWidth(), sourceimageBack.getHeight(), 0,
				 GL_RGB, GL_UNSIGNED_BYTE, sourceimageBack.getData());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, sourceimageFront.getWidth(), sourceimageFront.getHeight(), 0,
				 GL_RGB, GL_UNSIGNED_BYTE, sourceimageFront.getData());

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, params.m_linear ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, params.m_linear ? GL_LINEAR : GL_NEAREST);

	m_params = params;
}
void CubeMap::bind(int texunit)
{
	glActiveTexture(texunit);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, m_ID);
	m_hasBeenBound = true;
}

bool CubeMap::hasBeenUsed()
{
	return m_hasBeenBound;
}

*/
