#include "vbo.h"

VBO::VBO()
{
	m_vertexBufferID = -1;
	m_texCoordBufferID = -1;
	m_normalBufferID = -1;
	m_drawCount = 0;

	m_hasVertices = false;
	m_hasTexture = false;
	m_hasNormals = false;
}

VBO::~VBO()
{
}

void VBO::free()
{
	glBindBufferARB(GL_ARRAY_BUFFER, 0);
	if (m_vertexBufferID != -1)
		glDeleteBuffersARB(1, &m_vertexBufferID);
	if (m_vertexBufferID != -1)
		glDeleteBuffersARB(1, &m_texCoordBufferID);
	if (m_vertexBufferID != -1)
		glDeleteBuffersARB(1, &m_normalBufferID);

	m_vertexBufferID = -1;
	m_texCoordBufferID = -1;
	m_normalBufferID = -1;
}

void VBO::render()
{
	if (m_hasVertices)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertexBufferID);
		glVertexPointer(3, GL_FLOAT, 0, (char *)0);			//bind vertex buffer
	}
	else
	{
		g_debug << "trying to render a VBO that has no vertices" << endl;
		return;
	}

	m_hasTexture = true;
	if (m_hasTexture)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);		
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_texCoordBufferID);
		glTexCoordPointer(2, GL_FLOAT, 0, (char *)0);		//texture coordinates
	}
	else
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	if (m_hasNormals)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_normalBufferID);
		glNormalPointer(GL_FLOAT, 0, (char *)0);			//bind normal buffer
	}
	else
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}

//	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDrawArrays(GL_TRIANGLES, 0, m_drawCount);

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);				
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);				
}

void VBO::create(Mesh *mesh)
{
	vector<Vertex>& vertices = mesh->m_vertices;
	vector<Face>& faces = mesh->m_faces;

	m_hasTexture = mesh->m_hasTexture;
	int faceCount = faces.size();

	if (faceCount > 0)
	{
		g_debug << "Creating a VBO for mesh " << mesh->m_name << endl;
		g_debug << " -- " << endl;
		g_debug << "    VBO faceCount = " << faceCount << endl;
		Vector3 *vertexBufferData = new Vector3[faceCount * 3];
		Vector3 *normalBufferData = new Vector3[faceCount * 3];
		float *textureBufferData = new float[faceCount * 3 * 2]; //two texcoords per vertex
		//here we assume that the data is valid
		//might not be a good assumption but we do
		m_drawCount = faceCount * 3;

		int vertexOffset = 0;
		int normalOffset = 0;
		int textureOffset = 0;

		for (int i = 0; i < faceCount; i++)
		{
			//for each face, copy the data from the mesh into the bufferes
			Face &f = faces[i];
			Vertex& v1 = vertices[f.a];
			Vertex& v2 = vertices[f.b];
			Vertex& v3 = vertices[f.c];

			vertexBufferData[vertexOffset++] = v1.position; 
			vertexBufferData[vertexOffset++] = v2.position; 
			vertexBufferData[vertexOffset++] = v3.position; 

			normalBufferData[normalOffset++] = v1.normal;
			normalBufferData[normalOffset++] = v2.normal;
			normalBufferData[normalOffset++] = v3.normal;

			if (m_hasTexture)
			{
				TexCoord &t1 = vertices[f.a].texcoord;
				TexCoord &t2 = vertices[f.b].texcoord;
				TexCoord &t3 = vertices[f.c].texcoord;
				textureBufferData[textureOffset++] = t1.u;
				textureBufferData[textureOffset++] = t1.v;
				textureBufferData[textureOffset++] = t2.u;
				textureBufferData[textureOffset++] = t2.v;
				textureBufferData[textureOffset++] = t3.u;
				textureBufferData[textureOffset++] = t3.v;
			}
			else
			{
				textureBufferData[textureOffset++] = v1.texcoord.u;
				textureBufferData[textureOffset++] = v1.texcoord.v;
				textureBufferData[textureOffset++] = v2.texcoord.u;
				textureBufferData[textureOffset++] = v2.texcoord.v;
				textureBufferData[textureOffset++] = v3.texcoord.u;
				textureBufferData[textureOffset++] = v3.texcoord.v;
			}
		}

		g_debug << "    VBO vertexOffset = " << vertexOffset << endl;
		g_debug << "    VBO textureOffset = " << textureOffset << endl;
		g_debug << "    VBO normalOffset = " << normalOffset << endl;

		//create the buffers
		glGenBuffersARB(1, &m_vertexBufferID);
		glGenBuffersARB(1, &m_texCoordBufferID);
		glGenBuffersARB(1, &m_normalBufferID);

		g_debug << "    VBO vertex buffer ID = " << m_vertexBufferID << endl;
		g_debug << "    VBO texture buffer ID = " << m_texCoordBufferID << endl;
		g_debug << "    VBO normal buffer ID = " << m_normalBufferID << endl;

		//upload vertex data
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_vertexBufferID);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, faceCount * 3 * sizeof(Vector3), vertexBufferData, GL_STATIC_DRAW_ARB);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

		//upload texcoord data
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_texCoordBufferID);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, faceCount * 3 * 2 * sizeof(float), textureBufferData, GL_STATIC_DRAW_ARB);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

		//upload normal data
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, m_normalBufferID);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, faceCount * 3 * sizeof(Vector3), normalBufferData, GL_STATIC_DRAW_ARB);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

		m_hasVertices = true;
		m_hasNormals = true;

		delete [] vertexBufferData;
		delete [] normalBufferData;
		delete [] textureBufferData;
		
		g_debug << "" << endl;
	}
	else
	{
		g_debug << "    Trying to create a VBO with no data for mesh " << mesh->m_name << "!" << endl;
	}
}