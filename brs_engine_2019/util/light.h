#pragma once

#include "../globals.h"

class Light
{
public:
	Light();
	~Light();

	void setDefaults();
	void use(GLint light = GL_LIGHT0);

	void setPosition(float x, float y, float z);
	void setSpotDirection(float x, float y, float z);
	void setPosition(const Vector3 &position);
	void setSpotDirection(const Vector3& direction);
	void setSpotExponent(float exponent);

	void setDiffuse(Color3& color, float a = 1.0f);
	void setDiffuse(float r, float g, float b, float a);
	void setSpecular(Color3& color, float a = 1.0f);
	void setSpecular(float r, float g, float b, float a);
	void setAmbient(Color3& color, float a = 1.0f);
	void setAmbient(float r, float g, float b, float a);
	void setAttenuation(float constant, float linear, float quadratic);

private:
	float m_position[4];
	float m_spotDirection[4];
	
	float m_diffuse[4];
	float m_ambient[4];
	float m_specular[4];

	float m_constantAttenuation;
	float m_linearAttenuation;
	float m_quadraticAttenuation;

	float m_spotExponent;
};
