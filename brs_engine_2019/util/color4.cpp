#include <GL/glew.h>
#include "color4.h"
#include "../core/debugger.h"
extern Debugger g_debug;

Color4::Color4()
{
    r = g = b = 0.0f;
    a = 1.0f;
    m_alphamodulation = 1.0f;
}
Color4::Color4(float r, float g, float b, float a):r(r),g(g),b(b),a(a)
{
    m_alphamodulation = 1.0f;
}

Color4::~Color4()
{

}

Color4 Color4::operator * (float const &value)
{
    float tr, tg, tb;
    tr = r * value;
    tg = g * value;
    tb = b * value;

    return Color4(tr, tg, tb, a);
}
Color4 Color4::operator *= (float const &value)
{
    r *= value;
    g *= value;
    b *= value;

    return Color4(r, g, b, a);
}
void Color4::operator = (Color4 const &col)
{
    r = col.r;
    g = col.g;
    b = col.b;
    a = col.a;
    m_alphamodulation = col.m_alphamodulation;
}

void Color4::use() const
{
    //	g_debug << "alphamod = " << m_alphamodulation << ", total = " << a * m_alphamodulation << "\n";
    glColor4f(r, g, b, a * m_alphamodulation);
}


Color4 Color4::normalize()
{
    float l = sqrtf(r*r + g*g + b*b);
    if (l < 0.00001f && l > -0.00001f)
        return Color4(0,0,0,a);

    r /= l;
    g /= l;
    b /= l;

    return Color4(r, g, b, a);
}
