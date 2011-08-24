#pragma once
#include <cstring>
#include <cmath>

template<typename T>
class Matrix4
{
private:
	T m[4][4];
public:
	Matrix4() { ZeroMatrix(); }
	Matrix4(const Matrix4& rhs) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[i][j] = rhs.m[i][j];
	}
	Matrix4(T* A) {
		for(int i = 0, k = 0; i < 4; i++)
			for(int j = 0; j < 4; j++, k++)
				m[i][j] = A[k];
	}
	~Matrix4() {}
	void ZeroMatrix() {
		memset(m, 0, 16 * sizeof(T));
	}
	void MakeIdentity() {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[i][j] = (i == j);
	}
	static Matrix4 Identity() {
		Matrix4 M;

		M.MakeIdentity();
		return M;
	}
	Matrix4& operator+(const Matrix4& rhs) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[i][j] += rhs.m[i][j];
		return *this;
	}
	Matrix4& operator-(const Matrix4& rhs) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[i][j] -= rhs.m[i][j];
		return *this;
	}
	Matrix4& operator*(float r) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[i][j] *= r;
		return *this;
	}
	Matrix4& operator/(float r) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[i][j] /= r;
		return *this;
	}
	friend Matrix4& operator*(float r, Matrix4& M) {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				M.m[i][j] *= r;
		return M;
	}
	Matrix4& operator*(const Matrix4& rhs) {
		float t[4][4] = {0};
		
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
			{
				for(int k = 0; k < 4; k++)
					t[i][j] += m[i][k] * rhs.m[k][j];
			}
		memcpy(m, t, 16 * sizeof(T));
		return *this;
	}
	Matrix4& operator=(const Matrix4& rhs) {
		if(this != &rhs)
			memcpy(m, rhs.m, 16 * sizeof(T));
		return *this;
	}
	void Transpose() {
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
				m[i][j] = m[j][i];
	}
	static Matrix4 Translation(const Vector3 &V)
	{
		Matrix4 M;

		M.IdentityMatrix();
		M.m[3][0] = V.x;
		M.m[3][1] = V.y;
		M.m[3][2] = V.z;
		return M;
	}
	static Matrix4 Translation(float tx, float ty, float tz)
	{
		Matrix4 M;

		M.IdentityMatrix();
		M.m[3][0] = tx;
		M.m[3][1] = ty;
		M.m[3][2] = tz;
		return M;
	}
	static Matrix4 RotationX(float ang) {
		Matrix4 M;

		M.IdentityMatrix();
		float c = cosf(ang), s = sinf(ang);
		M.m[1][1] = c; M.m[1][2] = -s;
		M.m[2][1] = s; M.m[2][2] = c;
		return M;
	}
	static Matrix4 RotationY(float ang) {
		Matrix4 M;

		M.IdentityMatrix();
		float c = cosf(ang), s = sinf(ang);
		M.m[0][0] = c; M.m[0][2] = s;
		M.m[2][0] = -s; M.m[2][2] = c;
		return M;
	}
	static Matrix4 RotationZ(float ang) {
		Matrix4 M;

		M.IdentityMatrix();
		float c = cosf(ang), s = sinf(ang);
		M.m[0][0] = c; M.m[0][1] = -s;
		M.m[1][0] = s; M.m[1][1] = c;
		return M;
	}
	static Matrix4 RotationWithAxis(const Vector3 &axis, float ang) {
		Matrix4 M;

		M.IdentityMatrix();
		float c = cosf(ang), s = sinf(ang);
		float ax = axis.x, ay = axis.y, az = axis.z;
		M.m[0][0] = c + ax * ax * (1 - c);
		M.m[0][1] = ax * ay * (1 - c) - az * s;
		M.m[0][2] = ax * az * (1 - c) + ay * s;
		M.m[1][0] = ay * ax * (1 - c) + az * s;
		M.m[1][1] = c + ay * ay * (1 - c);
		M.m[1][2] = ay * az * (1 - c) - ax * s;
		M.m[2][0] = az * ax * (1 - c) - ay * s;
		M.m[2][1] = az * ay * (1 - c) + ax * s;
		M.m[2][2] = c + az * az * (1 - c);
		return M;
	}
	static Matrix4 Scaling(const Vector3 &S) {
		Matrix4 M;
		
		M.IdentityMatrix();
		M.m[0][0] = S.x; M.m[1][1] = S.y; M.m[2][2] = S.z;
		return M;
	}
	static Matrix4 Scaling(float sx, float sy, float sz) {
		Matrix4 M;

		M.IdentityMatrix();
		M.m[0][0] = sx; M.m[1][1] = sy; M.m[2][2] = sz;
		return M;
	}
	static Matrix4 Inverse(const Matrix4 &rhs) {
		Matrix4 M;
		/*T l[4][4] = {0}, u[4][4] = {0};
		

		for(int j = 0; j < 4; j++)
			u[0][j] = rhs.m[0][j];
		for(int i = 1; i < 4; i++)
			l[i][0] = rhs.m[i][0] / u[0][0];
		for(int i = 0; i < 4; i++)
			l[i][i] = 1;
		for(int i = 1; i < 4; i++)
			for(int j = 1; j < 4; j++)
			{
				if(i <= j)
				{
					u[i][j] = rhs.m[i][j];
					T sum = 0;
					for(int k = 0; k < i; k++)
						sum += l[i][k] * u[k][j];
					u[i][j] -= sum;
				}
				else
				{
					l[i][j] = rhs.m[i][j];
					T sum = 0;
					for(int k = 0; k < j; k++)
						sum += l[i][k] * u[k][j];
					l[j][i] -= sum;
					l[j][i] /= u[j][j];
				}
			}
		for(int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
			{
				if(i == j)
				{
					T sum = 0;
					for(int k = j + 1; k < 4; k++)
						sum += u[j][k] * M.m[k][j];
					M.m[j][j] = (1 - sum) / u[j][j];
				}
				else if(i < j) {
					T sum = 0;
					for(int k = i + 1; k < 4; k++)
						sum += u[i][k] * M.m[k][j];
					M.m[i][j] = -sum / u[i][i];
				}
				else
				{
					T sum = 0;
					for(int k = j + 1; k < 4; k++)
						sum += M.m[i][k] * l[k][j];
					M.m[i][j] = -sum;
				}
			}*/
			return M;
	}
	T& operator()(int i, int j) {
		return m[i][j];
	}
};

typedef Matrix4<float> Matrix4f;