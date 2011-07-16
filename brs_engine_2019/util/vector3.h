#pragma once

#include <math.h>
#include "matrix.h"

class Matrix;

/*
    A simple three-dimensional vector class

*/
class Vector3
{
    friend class Matrix;

public:

    Vector3();
    Vector3(float px, float py, float pz);
    Vector3(const class Vector3 &v) { this->x = v.x; this->y = v.y; this->z = v.z; }
    ~Vector3();

    Vector3 crossProduct(Vector3 &vect);
    float dotProduct(Vector3 &vect);

    Vector3 minim(Vector3 &vect);
    Vector3 maxim(Vector3 &vect);
    Vector3 clamp(Vector3 &min, Vector3 &max);

    Vector3 operator + (Vector3 const &vect);
    Vector3 operator - (Vector3 const &vect);
    Vector3 operator += (Vector3 const &vect);
    Vector3 operator -= (Vector3 const &vect);

    Vector3 operator * (Matrix const &mat);
    Vector3 operator *= (Matrix const &mat);
    Vector3 operator * (const float &value);
    Vector3 operator *= (float const &value);
    Vector3 operator / (const float &value);

    bool operator == (Vector3 const &vect);
    void operator = (Vector3 const &vect);

    Vector3 normalize();
    float squaredLength();
    float length();

    float x, y, z;

private:
};

