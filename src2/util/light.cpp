#include "light.h"

Light::Light()
{
	setDefaults();
}

Light::~Light()
{
}

void Light::setDefaults()
{
	setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	setSpecular(0.0f, 0.0f, 0.0f, 1.0f);
	setAmbient(0.0f, 0.0f, 0.0f, 0.0f);
	setAttenuation(1.0f, 0.0f, 0.0f); //?

	setPosition(Vector3(1.0f, 0.0f, 0.0f));
	setSpotDirection(Vector3(0.0f, 0.0f, 0.0f));
	setSpotExponent(0.0f);
}

void Light::use(GLint light)
{
	glLightfv(light, GL_POSITION, m_position);
	glLightfv(light, GL_DIFFUSE, m_diffuse);
	glLightfv(light, GL_AMBIENT, m_ambient);
	glLightfv(light, GL_SPECULAR, m_specular);
	glLightfv(light, GL_SPOT_DIRECTION, m_spotDirection);

	glLightf(light, GL_SPOT_EXPONENT, m_spotExponent);
	glLightf(light, GL_CONSTANT_ATTENUATION, m_constantAttenuation);
	glLightf(light, GL_LINEAR_ATTENUATION, m_linearAttenuation);
	glLightf(light, GL_QUADRATIC_ATTENUATION, m_quadraticAttenuation);
}


void Light::setPosition(float x, float y, float z)
{
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
	m_position[3] = 1;
}
void Light::setPosition(Vector3 &position)
{
	m_position[0] = position.x;
	m_position[1] = position.y;
	m_position[2] = position.z;
	m_position[3] = 1.0f;
}

void Light::setSpotDirection(float x, float y, float z)
{
	m_spotDirection[0] = x;
	m_spotDirection[1] = y;
	m_spotDirection[2] = z;
	m_spotDirection[3] = 1;
}
void Light::setSpotDirection(Vector3 &direction)
{
	m_spotDirection[0] = direction.x;
	m_spotDirection[1] = direction.y;
	m_spotDirection[2] = direction.z;
	m_spotDirection[3] = 1.0f;
}
void Light::setDiffuse(float r, float g, float b, float a)
{
	m_diffuse[0] = r;
	m_diffuse[1] = g;
	m_diffuse[2] = b;
	m_diffuse[3] = a;
}
void Light::setDiffuse(Color3& color, float a)
{
	m_diffuse[0] = color.r;
	m_diffuse[1] = color.g;
	m_diffuse[2] = color.b;
	m_diffuse[3] = a;
}
void Light::setSpecular(float r, float g, float b, float a)
{
	m_specular[0] = r;
	m_specular[1] = g;
	m_specular[2] = b;
	m_specular[3] = a;
}
void Light::setSpecular(Color3& color, float a)
{
	m_specular[0] = color.r;
	m_specular[1] = color.g;
	m_specular[2] = color.b;
	m_specular[3] = a;
}
void Light::setAmbient(float r, float g, float b, float a)
{
	m_ambient[0] = r;
	m_ambient[1] = g;
	m_ambient[2] = b;
	m_ambient[3] = a;
}
void Light::setAmbient(Color3& color, float a)
{
	m_ambient[0] = color.r;
	m_ambient[1] = color.g;
	m_ambient[2] = color.b;
	m_ambient[3] = a;
}
void Light::setAttenuation(float constant, float linear, float quadratic)
{
	m_constantAttenuation = constant;
	m_linearAttenuation = linear;
	m_quadraticAttenuation = quadratic;
}

void Light::setSpotExponent(float exponent)
{
	m_spotExponent = exponent;
}
