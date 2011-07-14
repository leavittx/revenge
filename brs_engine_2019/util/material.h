#pragma once

#include "../globals.h"

class Material
{
public:
	Material();
	~Material();

	void setDiffuse(float r, float g, float b, float a);
	void setDiffuse(Color3 &olor, float a = 1.0f);
	void setAmbient(float r, float g, float b, float a);
	void setAmbient(Color3 &olor, float a = 1.0f);
	void setSpecular(float r, float g, float b, float a);
	void setSpecular(Color3 &olor, float a = 1.0f);
	void setEmission(float r, float g, float b, float a);
	void setEmission(Color3 &olor, float a = 1.0f);
	void setShininess(float shininess);
	
	void use(bool diffuse = true, bool ambient = true, bool specular = true, bool emission = true, bool shininess = true);
	void setDefaults();
	
private:
	//components for the material
	float m_diffuse[4];
	float m_ambient[4];
	float m_specular[4];
	float m_emission[4];
	float m_shininess;
};

