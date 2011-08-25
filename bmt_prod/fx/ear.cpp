#include "ear.h"

void EarScene::init()
{
	g_params.useNamespace("ear");
//	m_debug = g_params.getBool("debug");

	GLWindow& glWindow = g_system.getGLWindow();
	res[0] = float(glWindow.getWidth());
	res[1] = float(glWindow.getHeight());
}

void EarScene::release()
{
}

void EarScene::update()
{
	g_params.useNamespace("ear");
}

void EarScene::draw()
{
	g_params.useNamespace("ear");
	Color4 col(1, 1, 1, 1);

//	static float oldwave = 0;

//	float fft[512];
//	g_system.getWaveData(fft, 512);
//	float wave = 0;
//	for (int i = 0; i < 512; i++)
//	{
//		wave += fft[i];
//	}
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

	Shader &fx_radial_blur = g_shaders.getShader("fx_radial_blur");
	fx_radial_blur.bind();
	fx_radial_blur.setUniform1f("time", float(g_system.getTime()));
	fx_radial_blur.setUniform2fv("resolution", 1, res);
	g_textures.bindTexture("rblur000.png", GL_TEXTURE0_ARB);
	fx_radial_blur.setUniform1i("tex0", 0);


//	Shader &fx_monjori = g_shaders.getShader("fx_monjori");
//	fx_monjori.bind();
//	fx_monjori.setUniform1f("time", float(g_system.getTime()));
//	fx_monjori.setUniform2fv("resolution", 1, res);
//	fx_monjori.setUniform3fv("wave", 1, wave);

//	Shader &tunnel = g_shaders.getShader("fx_tunnel");
//	tunnel.bind();
//	tunnel.setUniform1f("time", float(g_system.getTime()));
//	tunnel.setUniform2fv("resolution", 1, res);
//	g_textures.bindTexture("bmtlogo.png", GL_TEXTURE0_ARB);
//	tunnel.setUniform1i("tex0", 0);

	glUtil::fullscreenQuad(col);
}
