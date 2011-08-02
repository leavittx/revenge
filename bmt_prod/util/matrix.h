#pragma once

#include <math.h>
#include "vector3.h"

class Vector3;

/*
	A simple 3x3 matrix class.

*/
class Matrix
{
	friend class Vector3;

public:
	Matrix();
	Matrix(float val);
	Matrix(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8);
	~Matrix();

	Matrix makeIdentity();
	Matrix makeRotation(float x, float y, float z);
	Matrix makeRotation(Vector3 const &rot);
	Matrix makeScale(float x, float y, float z);
	Matrix makeScale(Vector3 const &sca);
	Matrix makeTranspose();

	static Matrix identity();
	static Matrix& scale(float x, float y, float z);
	static Matrix rotation(float x, float y, float z);
	static Matrix rotation(const Vector3 &v);

	Matrix operator + (Matrix const &mat);
	Matrix operator += (Matrix const &mat);
	Matrix operator - (Matrix const &mat);
	Matrix operator -= (Matrix const &mat);
	Matrix operator * (Matrix const &mat);
	Matrix operator *= (Matrix const &mat);
	Matrix operator * (float const &value);
	Matrix operator *= (float const &value);

	void operator = (Matrix const &mat);
	void operator = (float const &value);

	float data[3][3];
};

