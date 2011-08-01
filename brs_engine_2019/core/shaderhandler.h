#pragma once

#include "shader.h"

extern class ShaderHandler g_shaders;
class Shader;

class ShaderHandler
{
public:
	ShaderHandler();
	~ShaderHandler();

	void init();

	void bindShader(string name);
	Shader& getShader(string name); //this should not be done. Figure a way to pass the shader parameters properly
	void unbindShader();
	void loadShaders();
	void freeShaders();

private:


	char *loadShaderSource(string filename);
	map<string, Shader*> m_shaders;


};
