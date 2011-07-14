#pragma once

#include "../globals.h"

class VBO
{
public:
	VBO();
	~VBO();

	void render();
	void create(class Mesh *mesh);
	void free();

private:
	bool m_hasVertices;
	bool m_hasTexture;
	bool m_hasNormals;

	int m_drawCount;

	GLuint m_vertexBufferID;
	GLuint m_texCoordBufferID;
	GLuint m_normalBufferID;

};


