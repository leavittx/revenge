#include "mesh.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//											   Vertex
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vertex::Vertex():position(Vector3(0, 0, 0)),normal(Vector3(0,0,0)),texcoord(TexCoord(0,0))
{
	color = Color3(1,1,1);
}
Vertex::~Vertex()
{
}

void Vertex::setPosition(float x, float y, float z)
{
	position = Vector3(x, y, z);
}
void Vertex::setNormal(float x, float y, float z)
{
	normal = Vector3(x, y, z);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//												Face
////////////////////////////////////////////////////////////////////////////////////////////////////////////

Face::Face():a(0),b(0),c(0),normal(Vector3(0,0,0))
{
}
Face::~Face()
{
}

void Face::setVertices(int v1, int v2, int v3)
{
	a = v1;
	b = v2;
	c = v3;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//												TexCoord
////////////////////////////////////////////////////////////////////////////////////////////////////////////


TexCoord::TexCoord():u(0.0f),v(0.0f)
{
}
TexCoord::TexCoord(float u, float v):u(u),v(v)
{
}

TexCoord::~TexCoord()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//												BuildFace
////////////////////////////////////////////////////////////////////////////////////////////////////////////

BuildFace::BuildFace()
{
	m_vertices.clear();
	m_normals.clear();
	m_texcoords.clear();
}
BuildFace::~BuildFace()
{
}
void BuildFace::addPoint(int vertexIndex, int texcoordIndex, int normalIndex)
{
	m_vertices.push_back(vertexIndex);
	m_texcoords.push_back(texcoordIndex);
	m_normals.push_back(normalIndex);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//												MeshInfo
////////////////////////////////////////////////////////////////////////////////////////////////////////////


MeshInfo::MeshInfo()
{
	setDefaults();
}

MeshInfo::~MeshInfo()
{
}

void MeshInfo::setDefaults()
{
	m_useVBO = true;
	m_autoTexture = false;
	m_texture = "";
	m_useFaceNormals = false;
	m_retain = false;
	m_calculateNormals = false;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//												  Mesh
////////////////////////////////////////////////////////////////////////////////////////////////////////////


Mesh::Mesh()
{

	reset();
}

Mesh::~Mesh()
{
}

void Mesh::draw(DWORD flags)
{
	if (m_vbo)
	{
		m_vbo->render();
	}
	else
	{
		int faceCount = m_faces.size();
		bool texture = (flags & MESH_DRAW_TEXTURE) > 0;
		bool wireframe = (flags & MESH_DRAW_WIREFRAME) > 0;

		glEnable(GL_DEPTH_TEST);
		if (texture)
		{
			glEnable(GL_TEXTURE_2D);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}

		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < faceCount; i++)
		{
			Face &f = m_faces[i];
			Vertex& v1 = m_vertices[f.a];
			Vertex& v2 = m_vertices[f.b];
			Vertex& v3 = m_vertices[f.c];

			if (texture) 
				glTexCoord2fv((float *)&v1.texcoord);
			glNormal3fv((float *)&v1.normal);
			glVertex3fv((float *)&v1.position);

			if (texture) 
				glTexCoord2fv((float *)&v2.texcoord);
			glNormal3fv((float *)&v2.normal);
			glVertex3fv((float *)&v2.position);

			if (texture) 
				glTexCoord2fv((float *)&v3.texcoord);
			glNormal3fv((float *)&v3.normal);
			glVertex3fv((float *)&v3.position);
		}
		glEnd();
		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}

//////////////////////////// construction //////////////////////////////

void Mesh::reset()
{
	m_buildVertices.clear();
	m_buildFaces.clear();
	m_buildTexCoords.clear();
	m_buildNormals.clear();

	m_vbo = 0;
	m_name = "unnamed mesh";
	m_hasTexture = false;

}
void Mesh::begin()
{
	reset();
}

void Mesh::end()
{
	//loop through the faces

	m_vertices.clear();
	m_faces.clear();

	vector<BuildFace>::iterator it;
	int vertexOffset = 0;

	for (it = m_buildFaces.begin(); it != m_buildFaces.end(); it++)
	{
		BuildFace &f = *it; 
		int vertexCount = f.m_vertices.size(); //how many vertices there are in the face

		//TODO: n > 3
		for (int i = 0; i < 3; i++)
		{
			int vertexIndex = f.m_vertices[i];
			int normalIndex = f.m_normals[i];
			int textureIndex = f.m_texcoords[i];

			//we always have a vertex
			Vector3 position = m_buildVertices[vertexIndex - 1];

			//we might have a normal
			Vector3 normal  = Vector3(0, 0, 0);
			if (normalIndex > 0)
			{
				normal = m_buildNormals[normalIndex - 1];
			}

			//we might have a texture coordinate
			TexCoord texcoord = TexCoord(0, 0);
			if (textureIndex > 0)
			{
				texcoord = m_buildTexCoords[textureIndex - 1];
				m_hasTexture = true;
			}

			//construct a vertex and add it to the vertices vector
			Vertex vertex;
			vertex.position = position;
			vertex.normal = normal;
			vertex.texcoord = texcoord;
			m_vertices.push_back(vertex);
		}
		Face face;
		//set indices to the just created vertices
		face.a = vertexOffset;
		face.b = vertexOffset + 1;
		face.c = vertexOffset + 2;
		m_faces.push_back(face);

		vertexOffset += 3;
	}

	g_debug << "  mesh contains " << m_faces.size() << " triangles" << endl;

	scaleToSize(1.0f);
	center();
}

void Mesh::createVBO()
{
	m_vbo = new VBO();
	m_vbo->create(this);
}

void Mesh::addVertex(Vector3 &v)
{
	m_buildVertices.push_back(v);
}

void Mesh::addFace(BuildFace &f)
{
	m_buildFaces.push_back(f);
}

void Mesh::addTexCoord(TexCoord &t)
{
	m_buildTexCoords.push_back(t);
}
void Mesh::addNormal(Vector3 &n)
{
	m_buildNormals.push_back(n);
}

void Mesh::autoTexture(float scale)
{
}

void Mesh::calculateNormals()
{
}

void Mesh::transform(Matrix &transformation)
{

	vector<Vertex>::iterator it;
	for (it = m_vertices.begin(); it < m_vertices.end(); it++)
	{
		Vertex &v = *it;
		v.position *= transformation;
	}
}

void Mesh::scaleToSize(float maxSize)
{
	float maxDist = 0.0f;
	vector<Vertex>::iterator it;
	for (it = m_vertices.begin(); it < m_vertices.end(); it++)
	{
		Vertex &v = *it;
		float dist2 = v.position.squaredLength();
		if (dist2 > maxDist)
		{
			maxDist = dist2;
		}
	}
	float factor = sqrt(maxDist);
	transform(Matrix::scale(1/factor, 1/factor, 1/factor));
}

void Mesh::center()
{
	int vertexCount = m_vertices.size();
	if (vertexCount <= 0)
		return;

        Vector3 center = Vector3(0, 0, 0);

#ifdef _MSC_VER
        for each (Vertex v in m_vertices) {
#else
//#if defined(__GXX_EXPERIMENTAL_CXX0X__)
//        for (Vertex& v: m_vertices) {
//#else
        for (int i = 0; i < vertexCount; i++) {
            Vertex& v = m_vertices[i];
//#error
#endif
		center += v.position;
	}

	center *= 1.0f / vertexCount;
	for (int i = 0; i < vertexCount; i++)
	{
		m_vertices[i].position -= center;
	}
}
