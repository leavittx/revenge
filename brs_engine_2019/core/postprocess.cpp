#include "postprocess.h"


PostProcess::PostProcess()
{
}

PostProcess::~PostProcess()
{
}
void PostProcess::clearTexture(RenderTarget target)
{
	init(target, true);
	unInit();
}

string PostProcess::getRenderTargetName(RenderTarget target)
{
	switch(target)
	{
	case RENDERTARGET_0:
		return "postprocess_1024";
		break;
	case RENDERTARGET_1:
		return "postprocess_1024_2";
		break;
	case RENDERTARGET_2:
		return "postprocess_1024_3";
		break;
	case RENDERTARGET_3:
		return "postprocess_1024_4";
		break;
	}
	return ""; //make compiler happy
}


void PostProcess::init(RenderTarget target, bool clear)
{
	g_textures.bindTextureFBO(getRenderTargetName(target));//"postprocess_1024");
	if (clear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}
void PostProcess::unInit()
{
	g_textures.unbindFBO();
}

void PostProcess::blur(RenderTarget source, int steps, float xstep, float ystep, float bluralpha, float mixalpha)
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_BLEND);
	glColor3f(1,1,1);
	glUtil::blit(getRenderTargetName(source), "postprocess_512");
	glUtil::blit("postprocess_512", "postprocess_256");
	//	glUtil::blit("postprocess_256", "postprocess_128");
	//at this point, postprocess_128 contains a downsampled version of the full image

	//get shaders
	Shader& blurx = g_shaders.getShader("postprocess_blurx");
	Shader& blury = g_shaders.getShader("postprocess_blury");
	Shader& combine = g_shaders.getShader("postprocess_blurcombine");

	//then do the blur
	for (int i = 0; i < steps; i++)
	{
		const float t = i / (float)steps;
		//draw to the other 128x128 texture with the horizontal blur shader
		blurx.bind();
		blurx.setUniform1f("blurscale", xstep * t);
		blurx.setUniform1f("bluralpha", bluralpha);
		blurx.setUniform1i("tex", 0);
		//		glUtil::blit("postprocess_128", "postprocess_128_2");
		glUtil::blit("postprocess_256", "postprocess_256_2");

		//and then back to the original one with the vertical blur
		blury.bind();
		blury.setUniform1f("blurscale", ystep * t);
		blury.setUniform1f("bluralpha", bluralpha);
		blury.setUniform1i("tex", 0);
		//		glUtil::blit("postprocess_128_2", "postprocess_128");
		glUtil::blit("postprocess_256_2", "postprocess_256");
	}

	//enable blend so that the alpha channel will work if necessary
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);
	glColor4f(1,1,1,1);

	//combine the two textures
	combine.bind();
	combine.setUniform1i("tex0", 0);
	combine.setUniform1i("tex1", 1);
	combine.setUniform1f("bluralpha", mixalpha);
	g_textures.bindTexture(getRenderTargetName(source), GL_TEXTURE0_ARB);
	//	g_system.bindTexture("postprocess_128", GL_TEXTURE1_ARB);
	g_textures.bindTexture("postprocess_256", GL_TEXTURE1_ARB);

	g_system.setOrthoMode(1, 1);
	glBegin(GL_QUADS);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
	glVertex2f(0, 0);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
	glVertex2f(1, 0);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
	glVertex2f(1, 1);
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
	glVertex2f(0, 1);
	glEnd();

	g_system.setPerspectiveMode();

	g_shaders.unbindShader();
	glDepthMask(GL_TRUE);
	g_textures.clearTextureUnits();
	glDisable(GL_BLEND);
}

void PostProcess::glow(RenderTarget source, int steps, float xstep, float ystep, float bluralpha, float glowexp, bool combine)
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_BLEND);
	glColor3f(1,1,1);
	glUtil::blit(getRenderTargetName(source), "postprocess_512");
	glUtil::blit("postprocess_512", "postprocess_256");

	if (glowexp > 0.00001f)
	{
		Shader &exponential = g_shaders.getShader("postprocess_exp");
		exponential.bind();
		exponential.setUniform1f("expfactor", glowexp);
		exponential.setUniform1i("tex0", 0);
		glUtil::blit("postprocess_256", "postprocess_128");
	}
	else
	{
		glUtil::blit("postprocess_256", "postprocess_128");
	}

	//at this point, postprocess_128 contains a downsampled version of the full image

	//get shaders
	Shader& blurx = g_shaders.getShader("postprocess_blurx");
	Shader& blury = g_shaders.getShader("postprocess_blury");
	Shader& combineShader = g_shaders.getShader("postprocess_glowcombine");

	//then do the blur
	for (int i = 0; i < steps; i++)
	{
		const float t = i / (float)steps;
		//draw to the other 128x128 texture with the horizontal blur shader
		blurx.bind();
		blurx.setUniform1f("blurscale", xstep * t);
		blurx.setUniform1f("bluralpha", bluralpha);
		blurx.setUniform1i("tex", 0);
		glUtil::blit("postprocess_128", "postprocess_128_2");

		//and then back to the original one with the vertical blur
		blury.bind();
		blury.setUniform1f("blurscale", ystep * t);
		blury.setUniform1f("bluralpha", bluralpha);
		blury.setUniform1i("tex", 0);
		glUtil::blit("postprocess_128_2", "postprocess_128");
	}

	//enable blend so that the alpha channel will work if necessary
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);
	glColor4f(1,1,1,1);

	g_system.setOrthoMode(1, 1);
	if (combine)
	{
		//combine the two textures
		combineShader.bind();
		combineShader.setUniform1i("tex0", 0);
		combineShader.setUniform1i("tex1", 1);
		g_textures.bindTexture(getRenderTargetName(source), GL_TEXTURE0_ARB);
		g_textures.bindTexture("postprocess_128", GL_TEXTURE1_ARB);

		glBegin(GL_QUADS);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
		glVertex2f(0, 0);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
		glVertex2f(1, 0);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
		glVertex2f(1, 1);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
		glVertex2f(0, 1);
		glEnd();
	}
	else
	{
		//just draw the blurred one on top
		g_textures.bindTexture("postprocess_128", GL_TEXTURE0_ARB);

		glBegin(GL_QUADS);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
		glVertex2f(0, 0);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
		glVertex2f(1, 0);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
		glVertex2f(1, 1);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
		glVertex2f(0, 1);
		glEnd();
	}

	g_system.setPerspectiveMode();

	g_shaders.unbindShader();
	glDepthMask(GL_TRUE);
	g_textures.clearTextureUnits();
	glDisable(GL_BLEND);
}

void PostProcess::pixelize(RenderTarget source, Color4& color, int blendmode)
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	//first scale down from 1024 to a nearest filtered 32
	glDisable(GL_BLEND);
	glColor3f(1,1,1);
	glUtil::blit(getRenderTargetName(source), "postprocess_512");
	glUtil::blit("postprocess_512", "postprocess_256");
	glUtil::blit("postprocess_256", "postprocess_128");
	glUtil::blit("postprocess_128", "postprocess_64");
	//	glUtil::blit("postprocess_64", "postprocess_32");

	//draw original on the screen
	glUtil::fullscreenQuad(getRenderTargetName(source));

	//the set blending and draw the pixeled one on top
	glBlendFunc(GL_SRC_ALPHA, blendmode);
	glEnable(GL_BLEND);
	glColor4fv((float *)&color);
	glUtil::fullscreenQuad("postprocess_64");
	glDisable(GL_BLEND);

	glDepthMask(GL_TRUE);
}

void PostProcess::invert(RenderTarget source, float blendfactor)
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	g_textures.unbindFBO();

	Shader &s = g_shaders.getShader("postprocess_invert");
	s.bind();
	s.setUniform1f("blendfactor", blendfactor);
	s.setUniform1i("tex", 0);

	//shader bound and set, draw fullscreen quad that contain the image
	glDisable(GL_BLEND);
	glColor3f(1,1,1);
	//draw original on the screen
	glUtil::fullscreenQuad(getRenderTargetName(source));
	glDepthMask(GL_TRUE);

	g_shaders.unbindShader();
}

