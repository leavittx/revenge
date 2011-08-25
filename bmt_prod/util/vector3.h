#pragma once

#include <math.h>
#include "vector2.h"
#include "matrix.h"

class Matrix;

/*
	A simple three-dimensional vector class
*/

class Vector3
{
	friend class Matrix;

public:

	Vector3()
	{
		x = y = z = 0.0f;
	}
	Vector3(float val)
	{
		x = y = z = val;
	}
	Vector3(float px, float py, float pz)
	{
		x = px;
		y = py;
		z = pz;
	}
	Vector3(Vector2f const &vect, float pz = 0)
	{
		x = vect.x;
		y = vect.y;
		z = pz;
	}
	Vector3(const class Vector3 &v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	}
	~Vector3() {}

	Vector3 crossProduct(Vector3 &vect)
	{
		float tx = y*vect.z-z*vect.y;
		float ty = x*vect.z-z*vect.x;
		float tz = x*vect.y-y*vect.x;
		return Vector3(tx, -ty, tz);
	}
	float dotProduct(Vector3 &vect)
	{
		return x*vect.x + y*vect.y + z*vect.z;
	}

	Vector3 minim(Vector3 &vect)
	{
		return Vector3((vect.x < x ? vect.x : x),
					   (vect.y < y ? vect.y : y),
					   (vect.z < z ? vect.z : z));
	}
	Vector3 maxim(Vector3 &vect)
	{
		return Vector3((vect.x > x ? vect.x : x),
					   (vect.y > y ? vect.y : y),
					   (vect.z > z ? vect.z : z));
	}
	Vector3 clamp(Vector3 &min, Vector3 &max)
	{
		Vector3 t = this->minim(min);
		return t.maxim(max);
	}

	Vector3 operator + (Vector3 const &vect)
	{
		float tx, ty, tz;
		tx = x + vect.x;
		ty = y + vect.y;
		tz = z + vect.z;
		return Vector3(tx, ty, tz);
	}
	Vector3 operator - (Vector3 const &vect)
	{
		float tx, ty, tz;
		tx = x - vect.x;
		ty = y - vect.y;
		tz = z - vect.z;
		return Vector3(tx, ty, tz);
	}
	Vector3 operator += (Vector3 const &vect)
	{
		x += vect.x;
		y += vect.y;
		z += vect.z;
		return Vector3(x, y, z);
	}
	Vector3 operator -= (Vector3 const &vect)
	{
		x -= vect.x;
		y -= vect.y;
		z -= vect.z;
		return Vector3(x, y, z);
	}

	Vector3 operator * (Matrix const &mat);
	Vector3 operator *= (Matrix const &mat);

	Vector3 operator * (const float &value)
	{
		float tx, ty, tz;
		tx = x * value;
		ty = y * value;
		tz = z * value;
		return Vector3(tx, ty, tz);
	}
	Vector3 operator *= (float const &value)
	{
		x *= value;
		y *= value;
		z *= value;
		return Vector3(x, y, z);
	}
	Vector3 operator / (const float &value)
	{
		if (value == 0) return Vector3(0.0f, 0.0f, 0.0f);
		float tx, ty, tz;
		tx = x / value;
		ty = y / value;
		tz = z / value;
		return Vector3(tx, ty, tz);
	}

	bool operator == (Vector3 const &vect)
	{
		return (vect.x == x && vect.y == y && vect.z == z);
	}
	Vector3& operator = (Vector3 const &vect)
	{
		if (this == &vect)
			return *this;
		x = vect.x;
		y = vect.y;
		z = vect.z;
		return *this;
	}


	Vector3 normalize()
	{
		float l = length();
		if (l < 0.00001f && l > -0.00001f)
			return Vector3(0, 0, 0);
		x /= l;
		y /= l;
		z /= l;
		return Vector3(x, y, z);
	}
	float squaredLength()
	{
		return (float)(x*x + y*y + z*z);
	}
	float length()
	{
		return (float)sqrt(x*x + y*y + z*z);
	}

	float x, y, z;

private:
};

