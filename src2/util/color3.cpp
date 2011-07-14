#include <gl/glew.h>
#include "Color3.h"

Color3::Color3()
{
	r = g = b = 0.0f;
}
Color3::Color3(float r, float g, float b):r(r),g(g),b(b)
{
}

Color3::~Color3()
{

}
Color3::Color3(unsigned int rgb)
{
	r = ((rgb & 0xFF0000) >> 16) / 255.0f;
	g = ((rgb & 0xFF00) >> 8) / 255.0f;
	b = ((rgb & 0xFF)) / 255.0f;
}


Color3 Color3::operator + (Color3 const &col)
{
	return Color3(r+col.r, g+col.g, b+col.b);
}
Color3 Color3::operator - (Color3 const &col)
{
	return Color3(r-col.r, g-col.g, b-col.b);
}
Color3 Color3::operator += (Color3 const &col)
{
	r += col.r;
	g += col.g;
	b += col.b;

	return Color3(r, g, b);
}
Color3 Color3::operator -= (Color3 const &col)
{
	r -= col.r;
	g -= col.g;
	b -= col.b;

	return Color3(r, g, b);
}
Color3 Color3::operator * (float const &value)
{
    float tr, tg, tb;
	tr = r * value;
	tg = g * value;
	tb = b * value;

	return Color3(tr, tg, tb);
}
Color3 Color3::operator *= (float const &value)
{
	r *= value;
	g *= value;
	b *= value;

	return Color3(r, g, b);
}
void Color3::operator = (Color3 const &col)
{
	r = col.r;
	g = col.g;
	b = col.b;
}

Color3 Color3::normalize()
{
    float l = sqrtf(r*r + g*g + b*b);
	if (l < 0.00001f && l > -0.00001f)
		return Color3(0,0,0);

	r /= l;
	g /= l;
	b /= l;

    return Color3(r, g, b);
}

void Color3::use()
{
	glColor3f(r, g, b);
}
void Color3::useWithAlpha(float alpha)
{
	glColor4f(r, g, b, alpha);
}
void Color3::useModulated(float modulation)
{
	glColor3f(r * modulation, g * modulation, b * modulation);
}
void Color3::useWithAlphaModulated(float alpha, float modulation)
{
	glColor4f(r * modulation, g * modulation, b * modulation, alpha);
}
/*
Color3 Color3::randomColor()
{
	return Color3(Math::randFloat(), Math::randFloat(), Math::randFloat());
}
*/