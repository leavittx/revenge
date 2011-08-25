#pragma once

#include "../globals.h"

// hermite 3 point interpolation
template <class T>
T hermite(float frac_pos, T &xm1, T &x0, T &x1, T &x2)
{
   T c = (x1 - xm1) * 0.5;//static_cast<T>(0.5);
   T v = x0 - x1;
   T w = c + v;
   T a = w + v + (x2 - x0) * 0.5;//static_cast<T>(0.5);
   T b_neg = w + a;
   return ((((a * frac_pos) - b_neg) * frac_pos + c) * frac_pos + x0);
}

class RStep
{
public:
	RStep(string tex, Vector2f center, float size, float angle)
	{
		m_texture = tex;
		m_center  = center;
		m_size    = size;
		m_angle   = angle;
	}
	RStep(string tex)
	{
		m_texture = tex;
		m_center = Vector2f(512, 512);
		m_angle = 0;
	}

	string& texture()
	{
		return m_texture;
	}

	Matrix4f& matrix()
	{
		Vector2f c = (m_center / 512.0) - Vector2f(1, 1);
		return Matrix4f::Translation(Vector3(c, 0)) *
			   Matrix4f::RotationZ(m_angle) *
			   Matrix4f::Scaling(Vector3(m_size / 512.0));
	}

	Vector2f centerLogical()
	{
		return (m_center / 512.0) - Vector2f(1, 1);
	}

	double sFactor()
	{
		return 512.0 / m_size;
	}

	float size() { return m_size; }
	float size(float s)
	{
		if (s > 1024) s = 1024.f;
		//if (s < 64) s = 64.f;
		return m_size = s;
	}

	Vector2f center() { return m_center; }
	Vector2f center(Vector2f s)
	{
		s.x = Math::clamp(s.x, 0.f, 1023.f);
		s.y = Math::clamp(s.y, 0.f, 1023.f);
		return m_center = s;
	}

	float angle() { return m_angle; }
	float angle(float s)
	{
		s = Math::clamp(s, -PIf * 0.5f, PIf * 0.5f);
		return m_angle = s;
	}

private:
	string   m_texture;
	Vector2f m_center;
	float    m_size;
	float    m_angle; // 0 = horizontal
};

class PandaZoomScene : public Scene
{
public:
	PandaZoomScene() {}
	~PandaZoomScene() {}

	void init();
	void update();
	void draw();
	void release();

private:
	typedef vector<RStep> Steps;
	typedef Steps::iterator StepsIter;

	Steps m_steps;
	float m_state;

	static const int DEPTH = 8;
	int base;
	float frac;
	vector<string>   tex;
	vector<Matrix4f> mat;

	int effective;

	void draw(float progression, int bias);
	void drawTexture(int i);
	Matrix4f invMatrixSmooth(double frac);
};
