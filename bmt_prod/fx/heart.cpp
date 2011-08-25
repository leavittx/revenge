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

//	Shader &heart = g_shaders.getShader("heart");
//	heart.bind();
//	heart.setUniform1f("time", float(g_system.getTime()));
//	heart.setUniform2fv("resolution", 1, res);

	float wave[3];
	wave[0] = g_system.getSpectrumSum();
	wave[1] = g_system.getSpectrumSum();
	wave[2] = g_system.getSpectrumSum();

//	if (fabs(oldwave - wave) > 0.1)
//	{
//		g_debug << "wave = " << wave << endl;
//	}
//	oldwave = wave;

	if (!g_pitch.matchLowBassBeat())
	{
		wave[0] = PIf / 2;
	}
	if (!g_pitch.matchBassBeat())
	{
		wave[1] = PIf / 2;
	}
	if (!g_pitch.matchHiBassBeat())
	{
		wave[2] = PIf / 2;
	}


	Shader &fx_monjori = g_shaders.getShader("fx_monjori");
	fx_monjori.bind();
	fx_monjori.setUniform1f("time", float(g_system.getTime()));
	fx_monjori.setUniform2fv("resolution", 1, res);
	fx_monjori.setUniform3fv("wave", 1, wave);

	glUtil::fullscreenQuad(col);
	//uniform float time;
	//uniform vec2 resolution;
}

