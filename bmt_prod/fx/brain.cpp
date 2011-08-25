#include "brain.h"

void Brain::init()
{
	g_params.useNamespace("brain");
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
	glRotated(g_system.getTime() * 0.1, 0, 1, 0);
	glScaled(0.5, 0.5, 0.5);

	Mesh& cow = g_system.mesh("cow.obj");
	cow.draw(MESH_DRAW_WIREFRAME);

	FTFont &my_font = g_fonts.font<FTGLOutlineFont>("el&font block.ttf", 3);
	my_font.Render("COW COW COW");

	FTFont &my_font1 = g_fonts.font<FTGLBitmapFont>("el&font block.ttf", 50);
	my_font1.Render("Scene by FL3");

	glPopMatrix();
}

