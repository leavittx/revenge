#include "glutil.h"

void glUtil::drawImage(Vector3& screenPos, float xsize, float ysize)
{
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex2f(screenPos.x, screenPos.y);
	glTexCoord2f(1, 1);
	glVertex2f(screenPos.x + xsize, screenPos.y);
	glTexCoord2f(1, 0);
	glVertex2f(screenPos.x + xsize, screenPos.y + ysize);
	glTexCoord2f(0, 0);
	glVertex2f(screenPos.x, screenPos.y + ysize);
	glEnd();
}

void glUtil::antiRotate(Vector3& x, Vector3& y, Vector3& z)
{
	float matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	
	x = Vector3(matrix[0], matrix[4], matrix[8]);
	y = Vector3(matrix[1], matrix[5], matrix[9]);
	z = Vector3(matrix[2], matrix[6], matrix[10]);
}
void glUtil::camLook(Vector3& cam, Vector3& target, const Vector3& up)
{
	glLoadIdentity();
	gluLookAt(cam.x, cam.y, cam.z,
			  target.x, target.y, target.z,
			  up.x, up.y, up.z);
}

void glUtil::blit(string source, string dest)
{
	g_textures.bindTextureFBO(dest);
	glDisable(GL_BLEND);
	glColor3f(1,1,1);
	fullscreenQuad(source);
	g_textures.unbindFBO();
}

void glUtil::fullscreenQuad(Color4& color)
{
	g_system.setOrthoMode(1, 1);
	color.use();
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(1, 0);
	glVertex2f(1, 1);
	glVertex2f(0, 1);
	glEnd();
	g_system.setPerspectiveMode();
}

void glUtil::fullscreenQuad(string name)
{
	g_textures.bindTexture(name, GL_TEXTURE0_ARB);
	g_system.setOrthoMode(1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(0, 0);
	glTexCoord2f(1, 0);
	glVertex2f(1, 0);
	glTexCoord2f(1, 1);
	glVertex2f(1, 1);
	glTexCoord2f(0, 1);
	glVertex2f(0, 1);
	glEnd();
	g_system.setPerspectiveMode();
}
