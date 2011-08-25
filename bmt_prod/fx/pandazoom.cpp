#include "pandazoom.h"

void PandaZoomScene::init()
{
	g_params.useNamespace("pandazoom");

	m_state = 1.0;

	tex.resize(DEPTH);
	mat.resize(DEPTH);

	m_steps.push_back(RStep("000.png", Vector2f(506, 521),  27.7569, -1.570799947));
	m_steps.push_back(RStep("001.png", Vector2f(518, 510),  83.2221, -0.978792012));
	m_steps.push_back(RStep("001.png", Vector2f(518, 513),  80.5489, -0.353789985));
	m_steps.push_back(RStep("001.png", Vector2f(514, 512), 100.4250, -0.862286985));
	m_steps.push_back(RStep("001.png", Vector2f(513, 512),  66.3469, -0.362210006));
	m_steps.push_back(RStep("003.png", Vector2f(492, 473),  97.4080, -0.006706140));
	m_steps.push_back(RStep("003.png", Vector2f(503, 477), 112.6710,  0.010707800));
	m_steps.push_back(RStep("003.png", Vector2f(495, 472), 106.5190, -0.000789034));
	m_steps.push_back(RStep("003.png", Vector2f(500, 468),  70.1304,  0.089209899));
	m_steps.push_back(RStep("004.png", Vector2f(467, 466),  52.5986, -0.802788973));
	m_steps.push_back(RStep("005.png", Vector2f(512, 506),  24.7588,  0.448208988));
	m_steps.push_back(RStep("006.png", Vector2f(495, 595),  62.8535, -1.503780007));
	m_steps.push_back(RStep("007.png", Vector2f(503, 512), 112.4310, -0.010788900));
	m_steps.push_back(RStep("008.png", Vector2f(511, 555),  56.8839, -0.012701700));
	m_steps.push_back(RStep("008.png", Vector2f(510, 555),  67.6461, -0.012701700));
	m_steps.push_back(RStep("008.png", Vector2f(510, 554),  59.6700, -0.012701700));
	m_steps.push_back(RStep("008.png", Vector2f(511, 555),  62.4204, -0.012701700));
	m_steps.push_back(RStep("008.png", Vector2f(512, 538),  29.3221, -0.012701700));
	m_steps.push_back(RStep("009.png", Vector2f(506, 516),  46.1691, -1.570799947));
	m_steps.push_back(RStep("009.png", Vector2f(511, 514),  42.1049, -1.570799947));
	m_steps.push_back(RStep("009.png", Vector2f(507, 509),  50.6653, -1.185709953));
	m_steps.push_back(RStep("010.png", Vector2f(509, 510),  16.5619, -0.767713010));
	m_steps.push_back(RStep("011.png", Vector2f(517, 516),  35.5021, -0.364710003));
	m_steps.push_back(RStep("011.png", Vector2f(517, 517),  32.3327, -1.570799947));
	m_steps.push_back(RStep("011.png", Vector2f(517, 517),  34.3428, -0.869211018));
	m_steps.push_back(RStep("012.png", Vector2f(515, 505),  19.6428,  1.342779994));
	m_steps.push_back(RStep("012.png", Vector2f(515, 505),  19.4127,  1.348780036));
	m_steps.push_back(RStep("012.png", Vector2f(515, 505),  19.6974,  1.324779987));
	m_steps.push_back(RStep("012.png", Vector2f(515, 505),  20.1112,  1.359279990));
	m_steps.push_back(RStep("012.png", Vector2f(512, 505),  21.5696,  1.341779947));
	m_steps.push_back(RStep("013.png", Vector2f(511, 517),  15.4110,  1.536780000));
	m_steps.push_back(RStep("013.png", Vector2f(513, 517),  15.6155,  1.570799947));
	m_steps.push_back(RStep("013.png", Vector2f(513, 515),  11.6957, -0.743215024));
	m_steps.push_back(RStep("014.png", Vector2f(514, 510),  17.2782,  0.073786803));
	m_steps.push_back(RStep("015.png", Vector2f(511, 510),  43.5902, -0.860710979));
	m_steps.push_back(RStep("016.png", Vector2f(510, 512),  25.0532, -1.443210006));
	m_steps.push_back(RStep("015.png", Vector2f(510, 509),  40.5866, -1.502709985));
	m_steps.push_back(RStep("016.png", Vector2f(510, 512),  16.5285, -1.443210006));
	m_steps.push_back(RStep("017.png", Vector2f(522, 520),  26.8496, -1.521299958));
	m_steps.push_back(RStep("017b.png",Vector2f(508, 509),  29.4839, -1.244210005));
	m_steps.push_back(RStep("017c.png",Vector2f(515, 509),   6.6063, -1.570799947));
	m_steps.push_back(RStep("018.png", Vector2f(518, 515),  18.7643, -1.570799947));
	m_steps.push_back(RStep("019.png", Vector2f(510, 509),  27.1699, -1.570799947));
	m_steps.push_back(RStep("020.png", Vector2f(515, 510),  28.8179,  1.065209985));
	m_steps.push_back(RStep("021.png", Vector2f(499, 515),  26.5550,  1.570799947));
	m_steps.push_back(RStep("022.png", Vector2f(507, 504),  84.2685,  0.915784001));
	m_steps.push_back(RStep("023.png", Vector2f(512, 515),  46.3318, -0.002713150));
	m_steps.push_back(RStep("023.png", Vector2f(513, 515),  45.2213, -0.000713205));
	m_steps.push_back(RStep("023.png", Vector2f(513, 516),  42.2222, -0.002213200));
	m_steps.push_back(RStep("023.png", Vector2f(507, 517),  38.8788, -1.570799947));
	m_steps.push_back(RStep("024.png", Vector2f(512, 522),  22.2984, -0.010789800));
	m_steps.push_back(RStep("025.png", Vector2f(516, 525),  16.1324,  1.570799947));
	m_steps.push_back(RStep("026.png", Vector2f(513, 510),  22.8625,  1.570799947));
	m_steps.push_back(RStep("027.png", Vector2f(515, 509),  48.4326,  1.509780049));
	m_steps.push_back(RStep("028.png", Vector2f(510, 508),  34.6283,  1.424780011));
	m_steps.push_back(RStep("028.png", Vector2f(511, 510),  36.6798,  1.174280047));
	m_steps.push_back(RStep("028.png", Vector2f(509, 511),  59.0516,  0.775285006));
	m_steps.push_back(RStep("001.png", Vector2f(511, 509),  25.6503,  1.570799947));
	m_steps.push_back(RStep("001.png", Vector2f(515, 516),  24.4867,  0.073786803));

	reverse(m_steps.begin(), m_steps.end());


	//	for (int i = 0; i < 20000; i++)
	//		m_steps.push_back(RStep("001.png", Vector2f(518, 510), 83.2221, -0.978792));

	//	for (int i = 0; i < 20000; i++)
	//		m_steps.push_back(RStep("zoom001.png", Vector2f(512, 542), 60.5619, -0.667713));


	//	m_steps.push_back(RStep("000.png", Vector2f(506, 521), 27.7569, -1.5708));
	//	m_steps.push_back(RStep("001.png", Vector2f(518, 510), 83.2221, -0.978792));
	//	m_steps.push_back(RStep("001.png", Vector2f(518, 513), 80.5489, -0.35379));
	//	m_steps.push_back(RStep("001.png", Vector2f(514, 512), 100.425, -0.862287));
	//	m_steps.push_back(RStep("001.png", Vector2f(513, 512), 66.3469, -0.36221));
	//	m_steps.push_back(RStep("003.png", Vector2f(492, 473), 97.408,  -0.00670614));
	//	m_steps.push_back(RStep("003.png", Vector2f(503, 477), 112.671,  0.0107078));
	//	m_steps.push_back(RStep("003.png", Vector2f(495, 472), 106.519, -0.000789034));
	//	m_steps.push_back(RStep("003.png", Vector2f(500, 468), 70.1304,  0.0892099));
	//	m_steps.push_back(RStep("004.png", Vector2f(467, 466), 52.5986, -0.802789));
	//	m_steps.push_back(RStep("005.png", Vector2f(512, 506), 24.7588,  0.448209));
	//	m_steps.push_back(RStep("005.png", Vector2f(512, 506), 24.7588,  0.448209));

//	step data/tex/zoom/006.tif 495 595 62.8535 -1.50378
//	step data/tex/zoom/007.tif 503 512 112.431 -0.0107889
//	step data/tex/zoom/008.tif 511 555 56.8839 -0.0127017
//	step data/tex/zoom/008.tif 510 555 67.6461 -0.0127017
//	step data/tex/zoom/008.tif 510 554 59.67 -0.0127017
//	step data/tex/zoom/008.tif 511 555 62.4204 -0.0127017
//	step data/tex/zoom/008.tif 512 538 29.3221 -0.0127017
//	step data/tex/zoom/009.tif 506 516 46.1691 -1.5708
//	step data/tex/zoom/009.tif 511 514 42.1049 -1.5708
//	step data/tex/zoom/009.tif 507 509 50.6653 -1.18571
//	step data/tex/zoom/010.tif 509 510 16.5619 -0.767713
//	step data/tex/zoom/011.tif 517 516 35.5021 -0.36471
//	step data/tex/zoom/011.tif 517 517 32.3327 -1.5708
//	step data/tex/zoom/011.tif 517 517 34.3428 -0.869211
//	step data/tex/zoom/012.tif 515 505 19.6428 1.34278
//	step data/tex/zoom/012.tif 515 505 19.4127 1.34878
//	step data/tex/zoom/012.tif 515 505 19.6974 1.32478
//	step data/tex/zoom/012.tif 515 505 20.1112 1.35928
//	step data/tex/zoom/012.tif 512 505 21.5696 1.34178
//	step data/tex/zoom/013.tif 511 517 15.411 1.53678
//	step data/tex/zoom/013.tif 513 517 15.6155 1.5708
//	step data/tex/zoom/013.tif 513 515 11.6957 -0.743215
//	step data/tex/zoom/014.tif 514 510 17.2782 0.0737868
//	step data/tex/zoom/015.tif 511 510 43.5902 -0.860711
//	step data/tex/zoom/016.tif 510 512 25.0532 -1.44321
//	step data/tex/zoom/015.tif 510 509 40.5866 -1.50271
//	step data/tex/zoom/016.tif 510 512 16.5285 -1.44321
//	step data/tex/zoom/017.tif 522 520 26.8496 -1.5213
//	step data/tex/zoom/017b.tif 508 509 29.4839 -1.24421
//	step data/tex/zoom/017c.tif 515 509 6.60627 -1.5708
//	step data/tex/zoom/018.tif 518 515 18.7643 -1.5708
//	step data/tex/zoom/019.tif 510 509 27.1699 -1.5708
//	step data/tex/zoom/020.tif 515 510 28.8179 1.06521
//	step data/tex/zoom/021.tif 499 515 26.555 1.5708
//	step data/tex/zoom/022.tif 507 504 84.2685 0.915784
//	step data/tex/zoom/023.tif 512 515 46.3318 -0.00271315
//	step data/tex/zoom/023.tif 513 515 45.2213 -0.000713205
//	step data/tex/zoom/023.tif 513 516 42.2222 -0.0022132
//	step data/tex/zoom/023.tif 507 517 38.8788 -1.5708
//	step data/tex/zoom/024.tif 512 522 22.2984 -0.0107898
//	step data/tex/zoom/025.tif 516 525 16.1324 1.5708
//	step data/tex/zoom/026.tif 513 510 22.8625 1.5708
//	step data/tex/zoom/027.tif 515 509 48.4326 1.50978
//	step data/tex/zoom/028.tif 510 508 34.6283 1.42478
//	step data/tex/zoom/028.tif 511 510 36.6798 1.17428
//	step data/tex/zoom/028.tif 509 511 59.0516 0.775285
//	step data/tex/zoom/001.tif 511 509 25.6503 1.5708
//	step data/tex/zoom/001.tif 515 516 24.4867 0.0737868
}

void PandaZoomScene::release()
{
	m_steps.clear();
	tex.clear();
	mat.clear();
}

void PandaZoomScene::update()
{
	g_params.useNamespace("pandazoom");

//	float EVENT1 = minsec(6,38.323);
//	float EVENT3 = minsec(7,35.207);
//	float EVENT4 = minsec(8,3.65); // end

//	if (gt < EVENT3)
//	{
		float prog1 = Math::calcPosFloat(g_system.getTime(), 0, 40000);

		m_state = 0.5 + prog1 * 34.5;
//	}
//	else // if (gt < EVENT4)
//	{
//		double prog = step(EVENT3, EVENT4, gt);
//		m_state = 35 + (m_animation.length - 35) * prog;
//	}

	//assert(m_state >= 0.0);
	//assert(m_state < m_animation.length);

	m_state = m_steps.size() - m_state;

	m_state = min(m_state, m_steps.size() - 1e-2f);
	m_state = max(m_state, 0.0f);
}

void PandaZoomScene::draw()
{
	g_params.useNamespace("pandazoom");

//	TODO
//	mainFBO.use();

	//TODO
	float invRatio = 1.0 / g_system.getAspectRatio();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//TODO
	GLWindow& glWindow = g_system.getGLWindow();

	Recti viewport(Vector2i(0, 0), Vector2i(glWindow.getWidth(), glWindow.getHeight()));
	glUtil::viewport(viewport);

	//TODO
	Matrix4f m(Matrix4f::Scaling(Vector3(invRatio, 1.f, 1.f) * 0.99f));
	glUtil::projectionMatrix(m); // to span all the screen

	float decal = Math::calcPosFloat(g_system.getTime()/* * (g_pitch.matchLowBassBeat()?200:1)*/, 0, 200000) * 0.168;

	glUtil::modelviewMatrix(Matrix4f::Identity() * Matrix4f::Translation(Vector3(decal, 0, 0)));



	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glUtil::setBlend(glUtil::ADD, glUtil::SRC_ALPHA, glUtil::ONE_MINUS_SRC_ALPHA,
					 glUtil::MAX, glUtil::ONE, glUtil::ONE);

	draw(m_state, 0);

//	GLWindow& glWindow = g_system.getGLWindow();
//	float res[2];
//	res[0] = float(glWindow.getWidth());
//	res[1] = float(glWindow.getHeight());

//	Shader &fx_radial_blur = g_shaders.getShader("fx_radial_blur");
//	fx_radial_blur.bind();
//	fx_radial_blur.setUniform1f("time", float(g_system.getTime()));
//	fx_radial_blur.setUniform2fv("resolution", 1, res);
//	g_textures.bindTexture("bmtlogo.png", GL_TEXTURE0_ARB);
//	fx_radial_blur.setUniform1i("tex0", 0);



//	Shader &blit = g_shaders.getShader("postprocess_blit");
//	blit.bind();
//	g_textures.bindTexture("zoom00.png", GL_TEXTURE0_ARB);
//	blit.setUniform1i("tex0", 0);

//	g_system.setOrthoMode(1, 1);

//	glColor4f(1,1,1,1);

//	glBegin(GL_QUADS);
//		glColor4f(1,1,1,1);
//		glTexCoord2f(0,1); glVertex2f(0, 0);//glVertex2f(-1,-1);
//		glTexCoord2f(1,1); glVertex2f(0.5, 0);//glVertex2f(+1,-1);
//		glTexCoord2f(1,0); glVertex2f(0.5, 0.5);//glVertex2f(+1,+1);
//		glTexCoord2f(0,0); glVertex2f(0, 0.5);//glVertex2f(-1,+1);
//	glEnd();

//	g_system.setPerspectiveMode();
}

void PandaZoomScene::draw(float progression, int bias)
{

	glUtil::setBlend(glUtil::ADD, glUtil::SRC_ALPHA, glUtil::ONE_MINUS_SRC_ALPHA,
					 glUtil::MAX, glUtil::ONE, glUtil::ONE);

	if (progression < 0.f) progression = 0.f;
	base = (int)progression;
	frac = progression - base;

	effective = 0;
	for (int i = 0; i < DEPTH; ++i) //TODO: tex.length
	{
		if (i + base < (int)m_steps.size())
		{
			effective++;
			tex[i] = m_steps[i + base].texture();
			mat[i] = m_steps[i + base].matrix();

		}
		else
		{
			tex[i] = string("NULL");
			mat[i] = Matrix4f::Identity();
		}
	}

	glUtil::pushMatrix();
	glUtil::scale(5.0, 5.0, 5.0);

//	g_system.setOrthoMode(1, 1);

	if (effective >= 0)
	{
		Matrix4f mat0Inv = (tex[0] == "NULL") ? Matrix4f::Identity() : invMatrixSmooth(frac);

		glUtil::multMatrix(mat0Inv);
	}

	drawTexture(0);

//	g_system.setPerspectiveMode();

	glUtil::popMatrix();
}

void PandaZoomScene::drawTexture(int i)
{
	glUtil::pushMatrix();
	glUtil::multMatrix(mat[i]);

	if (i < effective - 1)
	{
		drawTexture(i + 1);
	}

	glUtil::popMatrix();

	if (i < (int)tex.size()) //TODO
	{
//		Shader.useDefault();
//		m_blitShader.setSampler("tex", tex[i], 0);
//		m_blitShader.use();

		Shader &blit = g_shaders.getShader("postprocess_blit");
		blit.bind();
		g_textures.bindTexture(tex[i], GL_TEXTURE0_ARB);
		blit.setUniform1i("tex0", 0);

		glColor4f(1,1,1,1);

		glBegin(GL_QUADS);
			glColor4f(1,1,1,1);
			glTexCoord2f(0,1); glVertex2f(-1,-1);
			glTexCoord2f(1,1); glVertex2f(+1,-1);
			glTexCoord2f(1,0); glVertex2f(+1,+1);
			glTexCoord2f(0,0); glVertex2f(-1,+1);
		glEnd();
	}
}

Matrix4f PandaZoomScene::invMatrixSmooth(double frac)
{
	float fracSmooth = Math::calcSmoothStep(frac, 0, 1);

	Vector2f cp0 = Vector2f();
	Vector2f cpm1 = (base > 0) ? m_steps[base-1].centerLogical() : cp0;
	Vector2f cp1 = m_steps[base].centerLogical();
	Vector2f cp2 = (base < (int)m_steps.size()-1) ? m_steps[base+1].centerLogical() : cp1;
	Vector2f c = -hermite(frac, cpm1, cp0, cp1, cp2);

	float a0 = 0.f;
	float am1 = (base > 0) ?  -m_steps[base-1].angle() : a0;
	float a1 = m_steps[base].angle();
	float a2 = (base < (int)m_steps.size()-1) ? m_steps[base+1].angle() + m_steps[base].angle() : m_steps[base].angle();
	float a = hermite(frac, am1, a0, a1, a2);

//			auto scale = Matrix4f.scale(vec3f(pow( 512.0 / m_size, frac )));

	float sm1 = (base > 0) ? (m_steps[base].sFactor() /* 1.f*/ / m_steps[base-1].sFactor()) : 1.f;
	float s0 = 1.f;
	float s1 = 1.f;
	float s2 = ((base < (int)m_steps.size()-1) ? m_steps[base+1].sFactor() / m_steps[base].sFactor() : 1.0) * s1;

	float s = hermite(frac, sm1, s0, s1, s2);

	s = s * pow(m_steps[base].sFactor(), frac);

	Matrix4f scale = Matrix4f::Scaling(Vector3(s));

	Matrix4f rotateZ = Matrix4f::RotationZ(-a);
	Matrix4f translate = Matrix4f::Translation(Vector3(c, 0));
	return scale * rotateZ * translate;
}
