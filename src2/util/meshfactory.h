#pragma once

#include "../globals.h"

class MeshFactory
{
public:
	//import OBJ file
	static class Mesh* loadOBJ(string filename);
	//import compiled MSH file
	static class Mesh* loadMSH(string filename);
	
	//create different primitives
	static class Mesh* createSphere(float radius, int xres, int yres);
	static class Mesh* createCube(float size, int resolution);

private:
	static void parseOBJFace(string token, int& vertexIndex, int& textureIndex, int& normalIndex);

};


