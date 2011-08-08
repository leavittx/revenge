#include "brain.h"

void Brain::init()
{
	g_params.useNamespace("brain");
	m_debug = g_params.getBool("debug");
	brain = mf.loadOBJ("data/meshes/cow.obj");
	a = 0;
}

void Brain::release()
{
}

void Brain::update()
{
	g_params.useNamespace("brain");
}

void Brain::draw()
{
	g_params.useNamespace("brain");

	glLoadIdentity();

	glPushMatrix();
	glTranslated(0, 0, -10);
	glRotated(a += 0.2, 0, 1, 0);
	glScaled(0.5, 0.5, 0.5);
	brain->draw(MESH_DRAW_WIREFRAME);

	FTFont &my_font = g_fonts.font<FTGLOutlineFont>("el&font BLOCK.ttf", 3);
	my_font.Render("COW COW COW");

	FTFont &my_font1 = g_fonts.font<FTGLBitmapFont>("el&font BLOCK.ttf");
	my_font1.Render("Scene by FL3");

	glPopMatrix();
}

