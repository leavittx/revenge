#pragma once

#include <math.h>

/*
	A simple RGB color class.

*/  
class Color3
{
public:

	Color3();
	Color3(float r, float g, float b);
	Color3(const class Color3 &v) { this->r = v.r; this->g = v.g; this->b = v.b; }
	Color3(unsigned int rgb);
	~Color3();

	Color3 operator + (Color3 const &col);
	Color3 operator - (Color3 const &col);
	Color3 operator += (Color3 const &col);
	Color3 operator -= (Color3 const &col);

	Color3 operator * (const float &value);
	Color3 operator *= (float const &value);

	void operator = (Color3 const &col);
	Color3 normalize();

	void use();
	void useWithAlpha(float alpha = 1.0f);
	void useModulated(float modulation = 1.0f);
	void useWithAlphaModulated(float alpha = 1.0f, float modulation = 1.0f);

	float r, g, b;

private:
};

