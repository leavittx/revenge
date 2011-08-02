#pragma once

#include <math.h>
#include "color3.h"

/*
	A simple RGBA color class. Operator for converting a Color3 into a Color4 exists, which puts the alpha as 1.0f

*/
class Color4
{
public:

	Color4();
	Color4(float r, float g, float b, float a);
	Color4(const class Color4 &v) { this->r = v.r; this->g = v.g; this->b = v.b; this->a = v.a; m_alphamodulation = v.m_alphamodulation; }
	Color4(const class Color3 &v) { this->r = v.r; this->g = v.g; this->b = v.b; this->a = 1.0f; m_alphamodulation = 1.0f; }
	~Color4();

	void use() const;
	void setModulation(float mod) { m_alphamodulation = mod; }

	Color4 operator * (const float &value);
	Color4 operator *= (float const &value);

	void operator = (Color4 const &col);

	Color4 normalize();

	float r, g, b, a;

private:
	float m_alphamodulation;
};

