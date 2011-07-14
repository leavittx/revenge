#include "meshfactory.h"
#include "../core/parser.h"

void MeshFactory::parseOBJFace(string token, int& vertexIndex, int& textureIndex, int& normalIndex)
{
	vertexIndex = 0;
	textureIndex = 0;
	normalIndex = 0;

	int first_slash_index = token.find_first_of("/");
	if (first_slash_index == -1)
	{
		//there's no slashes, only pure vertex index
		//what to do here? Assume the others are zero? Is this even legal? Though teddy.obj is built this way..
		StringUtils::convertStringToNumber<int>(vertexIndex, token, std::dec);
	}
	else
	{
		//there are slashes, so we're guaranteed to have two or three components
		//first parse the vertex number
		string vertexnumber = token.substr(0, first_slash_index); //separate the vertex number from the token
		token = token.substr(first_slash_index+1, token.length()); //strip the vertex number from the string

		//find where the second slash is
		int second_slash_index = token.find_first_of("/");
		string texturenumber = token.substr(0, second_slash_index);
		token = token.substr(second_slash_index+1, token.length());

		string normalnumber = token; //only the normal index is left

		//convert the strings to integers
		StringUtils::convertStringToNumber<int>(vertexIndex, vertexnumber, std::dec);
		StringUtils::convertStringToNumber<int>(textureIndex, texturenumber, std::dec);
		StringUtils::convertStringToNumber<int>(normalIndex, normalnumber, std::dec);
	}
}



Mesh* MeshFactory::loadOBJ(string filename)
{
	Mesh *mesh = 0;
	vector<string> file;
	vector<string>::iterator it;
	vector<string> tokens;

	//read file and parse it
	if (StringUtils::readFile(filename, file, false))
	{
		mesh = new Mesh();
		mesh->begin();
		mesh->m_name = filename;
		g_debug << "Parsing mesh " << filename << endl;
		for (it = file.begin(); it < file.end(); it++)
		{
			string &s = *it;
			tokens.clear();
			StringUtils::tokenize(s, tokens);
			if (tokens.size() > 0)
			{
				//check out command
				if (tokens[0] == "#")
				{
					//this is a comment, so do nothing
				}
				if (tokens[0] == "v")
				{
					//parse vertex position
					float x, y, z, w = 1.0f;
					StringUtils::convertStringToNumber<float>(x, tokens[1], std::dec);
					StringUtils::convertStringToNumber<float>(y, tokens[2], std::dec);
					StringUtils::convertStringToNumber<float>(z, tokens[3], std::dec);

					//it might have a fourth parameter

					int paramcount = tokens.size() - 1;
					if (paramcount == 4)
					{
						StringUtils::convertStringToNumber<float>(w, tokens[4], std::dec);
					}
					//convert the homogenous coordinate to a regular 3d vector. Let's hope nothing breaks
					Vector3 v = Vector3(x/w, y/w, z/w);
					mesh->addVertex(v);
				}
				if (tokens[0] == "vn")
				{
					//parse vertex normal
					float x, y, z, w=1.0f;
					StringUtils::convertStringToNumber<float>(x, tokens[1], std::dec);
					StringUtils::convertStringToNumber<float>(y, tokens[2], std::dec);
					StringUtils::convertStringToNumber<float>(z, tokens[3], std::dec);

					//it might have a fourth parameter
					int paramcount = tokens.size() - 1;
					if (paramcount == 4)
					{
						StringUtils::convertStringToNumber<float>(w, tokens[4], std::dec);
					}

					Vector3 v = Vector3(x/w, y/w, z/w);
					mesh->addNormal(v);
				}
				if (tokens[0] == "vt")
				{
					float u = 0;
					float v = 0;
					StringUtils::convertStringToNumber<float>(u, tokens[1], std::dec);
					StringUtils::convertStringToNumber<float>(v, tokens[2], std::dec);

					TexCoord texcoord = TexCoord(u, v);

					mesh->addTexCoord(texcoord);
					//parse texture coordinate
				}
				if (tokens[0] == "f")
				{
					//parse face
					//
					int faceCount = tokens.size() - 1; //the f command is not included in this count
					BuildFace face;

					//parse each parameter
					for (int i = 0; i < faceCount; i++)
					{
						int vertexIndex = 0;
						int textureIndex = 0;
						int normalIndex = 0;
						parseOBJFace(tokens[1 + i], vertexIndex, textureIndex, normalIndex);
						face.addPoint(vertexIndex, textureIndex, normalIndex);
					}
					mesh->addFace(face);
				}

				if (tokens[0] == "g")
				{
					//parse group information?
				}
				if (tokens[0] == "mtllib")
				{
					//matlib? 
				}
				if (tokens[0] == "usemtl")
				{
					//use materials? 
				}
			}
		}
		mesh->end();
//		mesh->center();
		mesh->createVBO();
		return mesh;
	}
	else
	{
		return 0;
	}
	return 0;
}
Mesh* MeshFactory::loadMSH(string filename)
{
	return 0;
}
Mesh* MeshFactory::createSphere(float radius, int xres, int yres)
{
	return 0;
}
