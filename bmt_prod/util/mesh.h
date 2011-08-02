#pragma once

#include "../globals.h"
#include "vbo.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Basics
///////////////////////////////////////////////////////////////////////////////////////////////////////

class VBO;

class TexCoord
{
public:
	TexCoord(float u, float v);
	TexCoord();
	~TexCoord();

	float u, v;
};

class Vertex
{
public:
	Vertex();
	~Vertex();

	void setColor(float r, float g, float b);
	void setPosition(float x, float y, float z);
	void setNormal(float x, float y, float z);

	Color3 color;
	Vector3 position;
	Vector3 normal;
	TexCoord texcoord;
};

class Face
{
public:
	Face();
	~Face();

	void setVertices(int v1, int v2, int v3);
	int a, b, c;
	Vector3 normal;
};

class BuildFace
{
	friend class Mesh;
public:
	BuildFace();
	~BuildFace();

	void addPoint(int vertexIndex, int texcoordIndex, int normalIndex);

private:
	vector<int> m_vertices;
	vector<int> m_texcoords;
	vector<int> m_normals;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
// MeshInfo
///////////////////////////////////////////////////////////////////////////////////////////////////////

class MeshInfo
{
public:
	MeshInfo();
	~MeshInfo();
	
	void setDefaults();

	bool m_useVBO;
	bool m_autoTexture;
	bool m_useFaceNormals;
	bool m_calculateNormals;
	bool m_retain;
	string m_texture;

};

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mesh
///////////////////////////////////////////////////////////////////////////////////////////////////////

enum MESH_DRAW_FLAGS
{
	MESH_DRAW_FLAT = 0,
	MESH_DRAW_TEXTURE = 1,
	MESH_DRAW_WIREFRAME = 2,
};

class Mesh
{
	friend class MeshFactory;
	friend class VBO;
public:
	Mesh();
	~Mesh();

	//draw
	void draw(DWORD flags);

	//mesh construction
	void begin();
	void end();
	void createVBO();
	void addVertex(Vector3 &v);
	void addFace(BuildFace &f);
	void addTexCoord(TexCoord &t);
	void addNormal(Vector3 &n);
	void transform(Matrix &transformation);
	void center();
	void scaleToSize(float maxSize = 1.0f);

private:
	string m_name;
	VBO *m_vbo;
	//construction
	void reset(); //clear out everything
	void calculateNormals();
	void autoTexture(float scale = 1.0f);  //TODO: make it not suck ;)

	//these are used during the construction of the mesh by MeshFactory
	vector<Vector3> m_buildVertices;
	vector<BuildFace> m_buildFaces;
	vector<TexCoord> m_buildTexCoords;
	vector<Vector3> m_buildNormals;

	//actual data
	vector<Vertex> m_vertices;
	vector<Face> m_faces;

	bool m_hasTexture;

};

