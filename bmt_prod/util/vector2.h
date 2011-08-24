#pragma once

#include <math.h>

/*
	A simple two-dimensional vector class
*/

class Vector2
{
public:

	Vector2(float px = 0, float py = 0) : x(px), y(py) {}
	Vector2(const class Vector2 &v) { this->x = v.x; this->y = v.y; }
	~Vector2() {}

	float dotProduct(Vector2 &vect) { return x*vect.x + y*vect.y; }

	Vector2 minim(Vector2 &vect)
	{
		return Vector2((vect.x < x ? vect.x : x),
					   (vect.y < y ? vect.y : y));
	}
	Vector2 maxim(Vector2 &vect)
	{
		return Vector2((vect.x > x ? vect.x : x),
					   (vect.y > y ? vect.y : y));
	}
	Vector2 clamp(Vector2 &min, Vector2 &max)
	{
		Vector2 t = this->minim(min);
		return t.maxim(max);
	}

	Vector2 operator + (Vector2 const &vect)
	{
		float tx, ty, tz;
		tx = x + vect.x;
		ty = y + vect.y;
		return Vector2(tx, ty);
	}
	Vector2 operator - (Vector2 const &vect)
	{
		float tx, ty;
		tx = x - vect.x;
		ty = y - vect.y;
		return Vector2(tx, ty);
	}
	Vector2 operator += (Vector2 const &vect)
	{
		x += vect.x;
		y += vect.y;
		return Vector2(x, y);
	}
	Vector2 operator -= (Vector2 const &vect)
	{
		x -= vect.x;
		y -= vect.y;
		return Vector2(x, y);
	}

	Vector2 operator * (const float &value)
	{
		float tx, ty;
		tx = x * value;
		ty = y * value;
		return Vector2(tx, ty);
	}
	Vector2 operator *= (float const &value)
	{
		x *= value;
		y *= value;
		return Vector2(x, y);
	}
	Vector2 operator / (const float &value)
	{
		if (value == 0) return Vector2(0.0f, 0.0f);
		float tx, ty;
		tx = x / value;
		ty = y / value;
		return Vector2(tx, ty);
	}

	bool operator == (Vector2 const &vect) { return (vect.x == x && vect.y == y); }
	Vector2& operator = (const Vector2 &vect)
	{
		if (this == &vect)
			return *this;
		x = vect.x;
		y = vect.y;
		return *this;
	}

	Vector2 normalize()
	{
		float l = length();
		if (l < 0.00001f && l > -0.00001f)
			return Vector2(0, 0);
		x /= l;
		y /= l;
		return Vector2(x, y);
	}

	float squaredLength() { return (float)(x*x + y*y); }
	float length() { return (float)sqrt(x*x + y*y); }

	float x, y;

private:

};
