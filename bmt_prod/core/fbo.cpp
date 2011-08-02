#include "fbo.h"

//dirty hack
#if !defined(_MSC_VER)
#define sprintf_s snprintf
#endif

FBOManager::FBOManager()
{
}

FBOManager::~FBOManager()
{
}


void FBOManager::init()
{
	char buf[512];
	sprintf_s(buf, 512, "%s", glGetString(GL_VENDOR));
	string vendor = string(buf);
	sprintf_s(buf, 512, "%s", glGetString(GL_RENDERER));
	string renderer = string(buf);
	sprintf_s(buf, 512, "%s", glGetString(GL_VERSION));
	string version = string(buf);

	g_debug << "Your graphics card vendor is: " << vendor << endl;
	g_debug << "                    renderer: " << renderer << endl;
	g_debug << "                     version: " << version << endl;

	//create framebuffer objects
	glGenFramebuffersEXT(1, &m_frameBufferTexture2048);
	glGenFramebuffersEXT(1, &m_frameBufferTexture1024);
	glGenFramebuffersEXT(1, &m_frameBufferTexture512);
	glGenFramebuffersEXT(1, &m_frameBufferTexture256);
	glGenFramebuffersEXT(1, &m_frameBufferTexture128);
	glGenFramebuffersEXT(1, &m_frameBufferTexture64);
	glGenFramebuffersEXT(1, &m_frameBufferTexture32);

	//and depth
	glGenRenderbuffersEXT(1, &m_depthRenderBufferTexture2048);
	glGenRenderbuffersEXT(1, &m_depthRenderBufferTexture1024);
	glGenRenderbuffersEXT(1, &m_depthRenderBufferTexture512);
	glGenRenderbuffersEXT(1, &m_depthRenderBufferTexture256);
	glGenRenderbuffersEXT(1, &m_depthRenderBufferTexture128);
	glGenRenderbuffersEXT(1, &m_depthRenderBufferTexture64);
	glGenRenderbuffersEXT(1, &m_depthRenderBufferTexture32);

	glGenFramebuffersEXT (1, &m_depthFBO);

	// initialize color texture for depth buffer
	glGenTextures(1, &m_depthColTex);
	glBindTexture(GL_TEXTURE_2D, m_depthColTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	logFBOErrors("init depth color buffer");

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_depthFBO);

	// Bind color buffer
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_depthColTex, 0);
	logFBOErrors("init depth FBO - attach color");

	GLuint depths[3] = { GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT16 };

	// Create depth texture (Find a version that works on this gfx card and/or manufacturer)
	int index = 0;
	for (index = 0; index < 3; index++)
	{
		// Create depth texture
		glGenTextures(1, &m_depthTex);
		glBindTexture(GL_TEXTURE_2D, m_depthTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, depths[index], 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, m_depthTex, 0);
		if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT)
		{
			break;
		}

		glDeleteTextures(1, &m_depthTex);
	}
	if (index == 3)
	{
		g_debug << "Cannot find suitable depth texture format! Shit will probably now hit the fan..." << endl;
	}
	else
	{
	}

	logFBOErrors("init depth FBO - attach depth");

	glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);

	// Initialize the render-buffer for usage as a depth buffer.
	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_depthRenderBufferTexture2048);
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 2048, 2048);

	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_depthRenderBufferTexture1024);
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 1024, 1024);

	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_depthRenderBufferTexture512);
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 512, 512);

	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_depthRenderBufferTexture256);
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 256, 256);

	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_depthRenderBufferTexture128);
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 128, 128);

	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_depthRenderBufferTexture64);
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 64, 64);

	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_depthRenderBufferTexture32);
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 32, 32);

	GLenum status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );

	switch( status )
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		//all ok
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		g_debug << "GL_FRAMEBUFFER_UNSUPPORTED_EXT!\nYour graphics card does not support framebuffer objects.\nYou either need to upgrade drivers or buy a better one\n";
		exit(0);
		break;
	default:
		g_debug << "something strange happened\n" << endl;
		exit(0);
	}
	// Disable framebuffer object
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
}

void FBOManager::release() 
{
	// Free framebuffers
	glDeleteFramebuffersEXT(1, &m_frameBufferTexture2048);
	glDeleteFramebuffersEXT(1, &m_frameBufferTexture1024);
	glDeleteFramebuffersEXT(1, &m_frameBufferTexture512);
	glDeleteFramebuffersEXT(1, &m_frameBufferTexture256);
	glDeleteFramebuffersEXT(1, &m_frameBufferTexture128);
	glDeleteFramebuffersEXT(1, &m_frameBufferTexture64);
	glDeleteFramebuffersEXT(1, &m_frameBufferTexture32);

	// Free renderbuffers
	glDeleteRenderbuffersEXT(1, &m_depthRenderBufferTexture2048);
	glDeleteRenderbuffersEXT(1, &m_depthRenderBufferTexture1024);
	glDeleteRenderbuffersEXT(1, &m_depthRenderBufferTexture512);
	glDeleteRenderbuffersEXT(1, &m_depthRenderBufferTexture256);
	glDeleteRenderbuffersEXT(1, &m_depthRenderBufferTexture128);
	glDeleteRenderbuffersEXT(1, &m_depthRenderBufferTexture64);
	glDeleteRenderbuffersEXT(1, &m_depthRenderBufferTexture32);

	glDeleteRenderbuffersEXT(1, &m_depthFBO);
}

void FBOManager::bindDepthFBO()
{
	glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, m_depthFBO);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, m_depthTex, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D, m_depthColTex, 0);

	logFBOErrors("bindDepthTextureFBO1024");
	glViewport( 0, 0, 1024, 1024);
}

void FBOManager::bindTextureFBO2048(GLuint textureID)
{
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_frameBufferTexture2048);
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D,
							   textureID, 0 );
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
								  GL_RENDERBUFFER_EXT, m_depthRenderBufferTexture2048);
	logFBOErrors("bindTextureFBO2048");
	glViewport( 0, 0, 2048, 2048);
}
void FBOManager::bindTextureFBO1024(GLuint textureID)
{
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_frameBufferTexture1024);
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D,
							   textureID, 0 );

	//    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_depthFBO);
	//    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 1024, 1024);

	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
								  GL_RENDERBUFFER_EXT, m_depthRenderBufferTexture1024);
	logFBOErrors("bindTextureFBO1024");
	glViewport( 0, 0, 1024, 1024);
}

void FBOManager::bindTextureFBO512(GLuint textureID)
{
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_frameBufferTexture512);
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D,
							   textureID, 0 );
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
								  GL_RENDERBUFFER_EXT, m_depthRenderBufferTexture512);

	logFBOErrors("bindTextureFBO512");
	glViewport( 0, 0, 512, 512);
}

void FBOManager::bindTextureFBO256(GLuint textureID)
{
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_frameBufferTexture256);
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D,
							   textureID, 0 );
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
								  GL_RENDERBUFFER_EXT, m_depthRenderBufferTexture256);

	logFBOErrors("bindTextureFBO256");
	glViewport( 0, 0, 256, 256);
}

void FBOManager::bindTextureFBO128(GLuint textureID)
{
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_frameBufferTexture128);
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D,
							   textureID, 0 );
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
								  GL_RENDERBUFFER_EXT, m_depthRenderBufferTexture128);

	logFBOErrors("bindTextureFBO128");
	glViewport( 0, 0, 128, 128);
}
void FBOManager::bindTextureFBO64(GLuint textureID)
{
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_frameBufferTexture64);
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D,
							   textureID, 0 );
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
								  GL_RENDERBUFFER_EXT, m_depthRenderBufferTexture64);

	logFBOErrors("bindTextureFBO64");
	glViewport( 0, 0, 64, 64);
}
void FBOManager::bindTextureFBO32(GLuint textureID)
{
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_frameBufferTexture32);
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D,
							   textureID, 0 );
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
								  GL_RENDERBUFFER_EXT, m_depthRenderBufferTexture32);

	logFBOErrors("bindTextureFBO32");
	glViewport( 0, 0, 32, 32);
}


void FBOManager::unbindFBO()
{
	//zero is the regular rendering context
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
	g_system.resetViewport();
}


bool FBOManager::logFBOErrors() 
{
	return logFBOErrors("place undefined:");
}

bool FBOManager::logFBOErrors(string str) 
{
	int status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
	switch( status )
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		//alles ok
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		g_debug << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT - " << str << endl;
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		g_debug << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT - " << str << endl;
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		g_debug << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT - " << str << endl;
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		g_debug << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT - " << str << endl;
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		g_debug << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT - " << str << endl;
		return false;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		g_debug << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT - " << str << endl;
		return false;
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		g_debug << "GL_FRAMEBUFFER_UNSUPPORTED_EXT - " << str << endl;
		break;

	default:
		g_debug << "Something really fucked in FBO initialization - " << str << endl;
		break;
	}
	return true;
}
