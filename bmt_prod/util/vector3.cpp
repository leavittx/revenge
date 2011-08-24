#include "vector3.h"

Vector3 Vector3::operator * (Matrix const &mat)
{
	float tx, ty, tz;
	tx = mat.data[0][0]*x + mat.data[0][1]*y + mat.data[0][2]*z;
	ty = mat.data[1][0]*x + mat.data[1][1]*y + mat.data[1][2]*z;
	tz = mat.data[2][0]*x + mat.data[2][1]*y + mat.data[2][2]*z;
	return Vector3(tx, ty, tz);
}

Vector3 Vector3::operator *= (Matrix const &mat)
{
	float tx, ty, tz;
	tx = mat.data[0][0]*x + mat.data[0][1]*y + mat.data[0][2]*z;
	ty = mat.data[1][0]*x + mat.data[1][1]*y + mat.data[1][2]*z;
	tz = mat.data[2][0]*x + mat.data[2][1]*y + mat.data[2][2]*z;
	x = tx;
	y = ty;
	z = tz;
	return Vector3(x, y, z);
}
