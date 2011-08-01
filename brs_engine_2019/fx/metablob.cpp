#include "metablob.h"

void Metablob::init()
{
	g_params.useNamespace("metablob");
	m_debug = g_params.getBool("debug");

	GLWindow& glWindow = g_system.getGLWindow();
	res[0] = glWindow.getWidth();
	res[1] = glWindow.getHeight();
}

void Metablob::release()
{
}

void Metablob::update()
{
	g_params.useNamespace("metablob");
}

void Metablob::draw()
{
	g_params.useNamespace("metablob");
	Color4 col(1, 1, 1, 1);

	Shader &heart = g_shaders.getShader("metablob");
	heart.bind();
	heart.setUniform1f("time", g_system.getTime());
	heart.setUniform2fv("resolution", 1, res);
	glUtil::fullscreenQuad(col);
}


