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

//	static float oldwave = 0;

//	float fft[512];
//	g_system.getWaveData(fft, 512);
//	float wave = 0;
//	for (int i = 0; i < 512; i++)
//	{
//		wave += fft[i];
//	}
	float wave = g_system.getSpectrumSum();

//	if (fabs(oldwave - wave) > 0.1)
//	{
//		g_debug << "wave = " << wave << endl;
//	}
//	oldwave = wave;

	if (!g_pitch.matchHiBassBeat())
	{
		wave = PIf / 2;
	}

	Shader &heart = g_shaders.getShader("metablob");
	heart.bind();
	heart.setUniform1f("time", g_system.getTime());
	heart.setUniform2fv("resolution", 1, res);
	heart.setUniform1f("wave", wave);
	glUtil::fullscreenQuad(col);
}


