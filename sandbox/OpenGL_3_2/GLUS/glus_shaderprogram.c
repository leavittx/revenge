/**
 * GLUS - OpenGL 3 Utilities. Copyright (C) 2010 Norbert Nopper
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GL/glus.h"

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

GLUSboolean GLUSAPIENTRY glusBuildProgram(GLUSshaderprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource)
{
	GLint compiled;
    GLint linked;

	GLint logLength, charsWritten;

	char* log;

	if (!shaderProgram || !vertexSource || !fragmentSource)
	{
		return GLUS_FALSE;
	}

	shaderProgram->program = 0;
	shaderProgram->vertex = 0;
	shaderProgram->geometry = 0;
	shaderProgram->fragment = 0;

	// http://www.opengl.org/sdk/docs/man/xhtml/glCreateShader.xml
	shaderProgram->vertex = glCreateShader(GL_VERTEX_SHADER);

	// http://www.opengl.org/sdk/docs/man/xhtml/glShaderSource.xml
	glShaderSource(shaderProgram->vertex, 1, (const char**)vertexSource, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glCompileShader.xml
	glCompileShader(shaderProgram->vertex);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
    glGetShaderiv(shaderProgram->vertex, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
		glGetShaderiv(shaderProgram->vertex, GL_INFO_LOG_LENGTH, &logLength);

		log = (char*)malloc(logLength);

		// http://www.opengl.org/sdk/docs/man/xhtml/glGetShaderInfoLog.xml
        glGetShaderInfoLog(shaderProgram->vertex, logLength, &charsWritten, log);

		printf("Vertex shader compile error:\n");
		printf("%s\n", log);

		free(log);

		shaderProgram->vertex = 0;

		return GLUS_FALSE;
	}

	if (geometrySource)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glCreateShader.xml
		shaderProgram->geometry = glCreateShader(GL_GEOMETRY_SHADER);

		// http://www.opengl.org/sdk/docs/man/xhtml/glShaderSource.xml
		glShaderSource(shaderProgram->geometry, 1, (const char**)geometrySource, 0);

		// http://www.opengl.org/sdk/docs/man/xhtml/glCompileShader.xml
		glCompileShader(shaderProgram->geometry);

		// http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
		glGetShaderiv(shaderProgram->geometry, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			// http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
			glGetShaderiv(shaderProgram->geometry, GL_INFO_LOG_LENGTH, &logLength);

			log = (char*)malloc(logLength);

			// http://www.opengl.org/sdk/docs/man/xhtml/glGetShaderInfoLog.xml
			glGetShaderInfoLog(shaderProgram->geometry, logLength, &charsWritten, log);

			printf("Geometry shader compile error:\n");
			printf("%s\n", log);

			free(log);

			shaderProgram->geometry = 0;

			return GLUS_FALSE;
		}
	}

	// http://www.opengl.org/sdk/docs/man/xhtml/glCreateShader.xml
	shaderProgram->fragment = glCreateShader(GL_FRAGMENT_SHADER);

	// http://www.opengl.org/sdk/docs/man/xhtml/glShaderSource.xml
	glShaderSource(shaderProgram->fragment, 1, (const char**)fragmentSource, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glCompileShader.xml
	glCompileShader(shaderProgram->fragment);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
    glGetShaderiv(shaderProgram->fragment, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
		glGetShaderiv(shaderProgram->fragment, GL_INFO_LOG_LENGTH, &logLength);

		log = (char*)malloc(logLength);

		// http://www.opengl.org/sdk/docs/man/xhtml/glGetShaderInfoLog.xml
        glGetShaderInfoLog(shaderProgram->fragment, logLength, &charsWritten, log);

		printf("Fragment shader compile error:\n");
		printf("%s\n", log);

		free(log);

		shaderProgram->fragment = 0;

		return GLUS_FALSE;
	}

	// http://www.opengl.org/sdk/docs/man/xhtml/glCreateProgram.xml
	shaderProgram->program = glCreateProgram();

	// http://www.opengl.org/sdk/docs/man/xhtml/glAttachShader.xml
    glAttachShader(shaderProgram->program, shaderProgram->vertex);

	if (shaderProgram->geometry)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glAttachShader.xml
		glAttachShader(shaderProgram->program, shaderProgram->geometry);
	}

	// http://www.opengl.org/sdk/docs/man/xhtml/glAttachShader.xml
    glAttachShader(shaderProgram->program, shaderProgram->fragment);

	// http://www.opengl.org/sdk/docs/man/xhtml/glLinkProgram.xml
    glLinkProgram(shaderProgram->program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetProgram.xml
	glGetProgramiv(shaderProgram->program, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glGetProgram.xml
		glGetProgramiv(shaderProgram->program, GL_INFO_LOG_LENGTH, &logLength);

		log = (char*)malloc(logLength);

		// http://www.opengl.org/sdk/docs/man/xhtml/glGetProgramInfoLog.xml
		glGetProgramInfoLog(shaderProgram->program, logLength, &charsWritten, log);

		printf("Shader program link error:\n");
		printf("%s\n", log);

		free(log);

		shaderProgram->program = 0;

		return GLUS_FALSE;
	}

	return GL_TRUE;
}

GLUSvoid GLUSAPIENTRY glusDestroyProgram(GLUSshaderprogram* shaderprogram)
{
	if (!shaderprogram)
	{
		return;
	}

	if (shaderprogram->program)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteProgram.xml
		glDeleteProgram(shaderprogram->program);

		shaderprogram->program = 0;
	}

	if (shaderprogram->fragment)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteShader.xml
		glDeleteShader(shaderprogram->fragment);

		shaderprogram->fragment = 0;
	}

	if (shaderprogram->geometry)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteShader.xml
		glDeleteShader(shaderprogram->geometry);

		shaderprogram->geometry = 0;
	}

	if (shaderprogram->vertex)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteShader.xml
		glDeleteShader(shaderprogram->vertex);

		shaderprogram->vertex = 0;
	}
}
