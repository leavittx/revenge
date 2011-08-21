#include "heart.h"

void Heart::init()
{
	g_params.useNamespace("heart");
	m_debug = g_params.getBool("debug");

	GLWindow& glWindow = g_system.getGLWindow();
	res[0] = float(glWindow.getWidth());
	res[1] = float(glWindow.getHeight());
}

void Heart::release()
{
}

void Heart::update()
{
	g_params.useNamespace("heart");
}

void Heart::draw()
{
	g_params.useNamespace("heart");
	Color4 col(1, 1, 1, 1);

	Shader &heart = g_shaders.getShader("heart");
	heart.bind();
	heart.setUniform1f("time", float(g_system.getTime()));
	heart.setUniform2fv("resolution", 1, res);
	glUtil::fullscreenQuad(col);
	//uniform float time;
	//uniform vec2 resolution;
}

