#pragma once

#include "../globals.h"


class Shader
{
public:
	Shader();
	~Shader();

	//setup
	void init(string name, const char *vertexSource, const char *fragmentSource);
	void release();
	void bind();

	//parameter passing
	bool setUniform1f(char* name, GLfloat v0);  
	bool setUniform2f(char* name, GLfloat v0, GLfloat v1); 
	bool setUniform3f(char* name, GLfloat v0, GLfloat v1, GLfloat v2); 
	bool setUniform4f(char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3); 
	
	bool setUniform1i(string name, GLint v0);
	bool setUniform2i(char* name, GLint v0, GLint v1);
	bool setUniform3i(char* name, GLint v0, GLint v1, GLint v2);
	bool setUniform4i(char* name, GLint v0, GLint v1, GLint v2, GLint v3);
	
	bool setUniform1fv(char* name, GLsizei count, GLfloat *value);
	bool setUniform2fv(char* name, GLsizei count, GLfloat *value);
	bool setUniform3fv(char* name, GLsizei count, GLfloat *value);
	bool setUniform4fv(char* name, GLsizei count, GLfloat *value);
	bool setUniform1iv(char* name, GLsizei count, GLint *value);
	bool setUniform2iv(char* name, GLsizei count, GLint *value);
	bool setUniform3iv(char* name, GLsizei count, GLint *value);
	bool setUniform4iv(char* name, GLsizei count, GLint *value);
	   
	bool setUniformMatrix2fv(char* name, GLsizei count, GLboolean transpose, GLfloat *value);
	bool setUniformMatrix3fv(char* name, GLsizei count, GLboolean transpose, GLfloat *value);
	bool setUniformMatrix4fv(char* name, GLsizei count, GLboolean transpose, GLfloat *value);

	//vertex attributes
	bool setVertexAttrib1f(char* name, GLfloat v0);
	bool setVertexAttrib2f(char* name, GLfloat v0, GLfloat v1);
	bool setVertexAttrib3f(char* name, GLfloat v0, GLfloat v1, GLfloat v2);
	bool setVertexAttrib4f(char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);	

	const GLhandleARB getVS() const { return m_vertex; };
	const GLhandleARB getFS() const { return m_fragment; };
	const GLhandleARB getProgram() const { return m_program; };

private:
	GLint getAttribLocation(const GLcharARB *name);
	GLint getUniformLocation(const GLcharARB *name);
	void debug(GLuint object, string type);

	string m_name;
	GLhandleARB m_vertex;
	GLhandleARB m_fragment;
	GLhandleARB m_program;


};

