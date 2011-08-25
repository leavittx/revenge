#pragma once

#include <math.h>

/*
	A simple two-dimensional vector class
*/

template <class T>
class Vector2
{
public:

	Vector2()
	{
		x = y = 0;
	}
	Vector2(T val)
	{
		x = y = val;
	}
	Vector2(T px, T py)
	{
		x = px;
		y = py;
	}
	Vector2(const class Vector2 &v)
	{
		this->x = v.x;
		this->y = v.y;
	}
	~Vector2() {}

	T dotProduct(Vector2 &vect)
	{
		return x*vect.x + y*vect.y;
	}

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
		T tx, ty;
		tx = x + vect.x;
		ty = y + vect.y;
		return Vector2(tx, ty);
	}
	Vector2 operator - (Vector2 const &vect)
	{
		T tx, ty;
		tx = x - vect.x;
		ty = y - vect.y;
		return Vector2(tx, ty);
	}
	Vector2 operator - ()
	{
		return Vector2(-x, -y);
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

	Vector2 operator * (const T &value)
	{
		T tx, ty;
		tx = x * value;
		ty = y * value;
		return Vector2(tx, ty);
	}
	Vector2 operator *= (const T &value)
	{
		x *= value;
		y *= value;
		return Vector2(x, y);
	}
	Vector2 operator / (const T &value)
	{
		if (value == 0) return Vector2(0.0f, 0.0f);
		T tx, ty;
		tx = x / value;
		ty = y / value;
		return Vector2(tx, ty);
	}

	bool operator == (Vector2 const &vect)
	{
		return (vect.x == x && vect.y == y);
	}
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
		T l = length();
		if (l < 0.00001f && l > -0.00001f)
			return Vector2(0, 0);
		x /= l;
		y /= l;
		return Vector2(x, y);
	}

	T squaredLength()
	{
		return (T)(x*x + y*y);
	}
	T length()
	{
		return (T)sqrt(x*x + y*y);
	}

	T x, y;

private:

};

typedef Vector2<int>    Vector2i;
typedef Vector2<float>  Vector2f;
typedef Vector2<double> Vector2d;
