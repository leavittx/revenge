#include "shader.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Init and bind
//////////////////////////////////////////////////////////////////////////////////////////////////

void Shader::init(string name, const char *vertexSource, const char *fragmentSource)
{
	m_name = name;

	g_debug << "compiling shader " << m_name << endl;

	bool useDebug = true;

	if (useDebug) g_system.glDebug("vertex1");
	m_vertex = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	if (useDebug) g_system.glDebug("vertex2");
	glShaderSourceARB(m_vertex, 1, &vertexSource, NULL);
	if (useDebug) g_system.glDebug("vertex3");
	glCompileShaderARB(m_vertex);
	if (useDebug) g_system.glDebug("vertex4");
	debug(m_vertex, "output from vertex shader:");

	if (useDebug) g_system.glDebug("fragment1");
	m_fragment = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	if (useDebug) g_system.glDebug("fragment2");
	glShaderSourceARB(m_fragment, 1, &fragmentSource, NULL);
	if (useDebug) g_system.glDebug("fragment3");
	glCompileShaderARB(m_fragment	);
	if (useDebug) g_system.glDebug("fragment4");
	debug(m_fragment, "output from fragment shader:");

	if (useDebug) g_system.glDebug("program1");
	m_program = glCreateProgramObjectARB();
	if (useDebug) g_system.glDebug("program2");
	glAttachObjectARB(m_program, m_vertex);
	if (useDebug) g_system.glDebug("program3");
	glAttachObjectARB(m_program, m_fragment);
	if (useDebug) g_system.glDebug("program4");
	glLinkProgramARB(m_program);
	if (useDebug) g_system.glDebug("program5");
}

void Shader::release()
{
	glDeleteShader(m_vertex);
	glDeleteShader(m_fragment);
	glDeleteProgram(m_program);
}

void Shader::bind()
{
//	g_system.glDebug("ennen shaderin bindausta");
	glUseProgramObjectARB(m_program);
//	g_system.glDebug("shaderin bindauksen jälkeen");
}

void Shader::debug(GLuint object, string type)
{
	if (glIsShader(object))
	{
		const int MAX_LOG_LENGTH = 4096;
		char log[MAX_LOG_LENGTH];
		int logLength = 0;
		glGetShaderInfoLog(object, MAX_LOG_LENGTH, &logLength, log);
		if (logLength > 0)
		{
			g_debug << type << endl;
			g_debug << log << endl;
		}
	}
	else
	{
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Parameter locations
//////////////////////////////////////////////////////////////////////////////////////////////////

GLint Shader::getUniformLocation(const GLcharARB *name)
{
	GLint location;
	location = glGetUniformLocationARB(m_program, name);
//	g_system.glDebug("uniform location paskan jälkeen");
	if (location == -1)
	{
//		g_debug << "Cannot find uniform variable " << name << " in shader " << m_name << endl;
	} 
	return location;
}

GLint Shader::getAttribLocation(const GLcharARB *name)
{
	GLint location;
	location = glGetAttribLocationARB(m_program, name);
	if (location == -1)
	{
		g_debug << "Cannot find attrib " << name << " in shader " << m_name << endl;
	}
	return location;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Uniform values
//////////////////////////////////////////////////////////////////////////////////////////////////

bool Shader::setUniform1f(char *name, GLfloat v0)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

	glUniform1fARB(location, v0);
	return true;
}

bool Shader::setUniform2f(char* name, GLfloat v0, GLfloat v1) 
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

	glUniform2fARB(location, v0, v1);

	return true;
}
bool Shader::setUniform3f(char* name, GLfloat v0, GLfloat v1, GLfloat v2) 
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

	glUniform3fARB(location, v0, v1, v2);
	return true;
}
bool Shader::setUniform4f(char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

	glUniform4fARB(location, v0, v1, v2, v3);
	return true;
}

bool Shader::setUniform1i(string name, GLint v0)
{
//	name = name.substr(0, name.length()-1);
	int location = getUniformLocation(name.c_str());
	if (location == -1)
		return false;

	glUniform1i(location, v0);

	return true;
}
bool Shader::setUniform2i(char* name, GLint v0, GLint v1)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

	glUniform2i(location, v0, v1);
	return true;
}
bool Shader::setUniform3i(char* name, GLint v0, GLint v1, GLint v2)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

	glUniform3i(location, v0, v1, v2);
	return true;
}
bool Shader::setUniform4i(char* name, GLint v0, GLint v1, GLint v2, GLint v3)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

	glUniform4i(location, v0, v1, v2, v3);
	return true;
}

bool Shader::setUniform1fv(char* name, GLsizei count, GLfloat *value)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

	glUniform1fv(location, count, value);

	return true;
}
bool Shader::setUniform2fv(char* name, GLsizei count, GLfloat *value)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;
	glUniform2fv(location, count, value);

	return true;
}
bool Shader::setUniform3fv(char* name, GLsizei count, GLfloat *value)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;
	glUniform3fv(location, count, value);

	return true;
}
bool Shader::setUniform4fv(char* name, GLsizei count, GLfloat *value)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

	glUniform4fv(location, count, value);
	return true;
}
bool Shader::setUniform1iv(char* name, GLsizei count, GLint *value)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

	glUniform1iv(location, count, value);

	return true;
}
bool Shader::setUniform2iv(char* name, GLsizei count, GLint *value)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

	glUniform2iv(location, count, value);
	return true;
}
bool Shader::setUniform3iv(char* name, GLsizei count, GLint *value)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

	glUniform3iv(location, count, value);
	return true;
}
bool Shader::setUniform4iv(char* name, GLsizei count, GLint *value)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

	glUniform4iv(location, count, value);
	return true;
}
   
bool Shader::setUniformMatrix2fv(char* name, GLsizei count, GLboolean transpose, GLfloat *value)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

    glUniformMatrix2fv(location, count, transpose, value);
	return true;
}
bool Shader::setUniformMatrix3fv(char* name, GLsizei count, GLboolean transpose, GLfloat *value)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

    glUniformMatrix3fv(location, count, transpose, value);
	return true;
}
bool Shader::setUniformMatrix4fv(char* name, GLsizei count, GLboolean transpose, GLfloat *value)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

    glUniformMatrix4fv(location, count, transpose, value);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Vertex attributes
//////////////////////////////////////////////////////////////////////////////////////////////////

//vertex attributes
bool Shader::setVertexAttrib1f(char* name, GLfloat v0)
{
	int location = getAttribLocation(name);
	if (location == -1)
		return false;

	glVertexAttrib1f(location, v0);

	return true;
}
bool Shader::setVertexAttrib2f(char* name, GLfloat v0, GLfloat v1)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;

	glVertexAttrib2f(location, v0, v1);
	return true;
}
bool Shader::setVertexAttrib3f(char* name, GLfloat v0, GLfloat v1, GLfloat v2)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;
	glVertexAttrib3f(location, v0, v1, v2);

	return true;
}
bool Shader::setVertexAttrib4f(char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	int location = getUniformLocation(name);
	if (location == -1)
		return false;
	glVertexAttrib4f(location, v0, v1, v2, v3);

	return true;
}
