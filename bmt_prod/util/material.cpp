#include "material.h"

Material::Material()
{
	setDefaults();
}

Material::~Material()
{
}

void Material::setDefaults()
{
	//default material is matte white
	setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	setAmbient(1.0f, 1.0f, 1.0f, 1.0f);
	setSpecular(0.0f, 0.0f, 0.0f, 1.0f);
	setEmission(0.0f, 0.0f, 0.0f, 1.0f);
	setShininess(1.0f);
}

void Material::use(bool diffuse, bool ambient, bool specular, bool emission, bool shininess)
{
	if (diffuse)
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
	if (ambient)
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient);
	if (specular)
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
	if (emission)
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emission);
	if (shininess)
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
}

void Material::setDiffuse(float r, float g, float b, float a)
{
	m_diffuse[0] = r;
	m_diffuse[1] = g;
	m_diffuse[2] = b;
	m_diffuse[3] = a;
}
void Material::setAmbient(float r, float g, float b, float a)
{
	m_ambient[0] = r;
	m_ambient[1] = g;
	m_ambient[2] = b;
	m_ambient[3] = a;
}
void Material::setSpecular(float r, float g, float b, float a)
{
	m_specular[0] = r;
	m_specular[1] = g;
	m_specular[2] = b;
	m_specular[3] = a;
}
void Material::setEmission(float r, float g, float b, float a)
{
	m_emission[0] = r;
	m_emission[1] = g;
	m_emission[2] = b;
	m_emission[3] = a;
}

void Material::setDiffuse(const Color3 &color, float a)
{
	setDiffuse(color.r, color.g, color.b, a);
}
void Material::setAmbient(const Color3 &color, float a)
{
	setAmbient(color.r, color.g, color.b, a);
}
void Material::setSpecular(const Color3 &color, float a)
{
	setSpecular(color.r, color.g, color.b, a);
}
void Material::setEmission(const Color3 &color, float a)
{
	setEmission(color.r, color.g, color.b, a);
}
void Material::setShininess(float shininess)
{
	m_shininess = shininess;
}
