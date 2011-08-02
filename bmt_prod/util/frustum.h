#pragma once

#include "../globals.h"

enum FrustumSide
{
	FRUSTUM_SIDE_RIGHT = 0,
	FRUSTUM_SIDE_LEFT = 1,
	FRUSTUM_SIDE_BOTTOM = 2,
	FRUSTUM_SIDE_TOP = 3,
	FRUSTUM_SIDE_BACK = 4,
	FRUSTUM_SIDE_FRONT = 5,
};

class BoundingSphere
{
	friend class FrustumCull;
public:
	BoundingSphere() { m_pos = Vector3(0, 0, 0); m_radius = 0.0f; }
	BoundingSphere(Vector3 &pos, float radius) { set(pos, radius); }
	~BoundingSphere() {}

	void set(Vector3 &pos, float radius) { m_pos = pos; m_radius = radius; }
private:

	Vector3 m_pos;
	float m_radius;
};

class BoundingBox
{
	friend class FrustumCull;
public:
	BoundingBox() { m_pos = Vector3(0, 0, 0); m_size = 0.0f; }
	BoundingBox(Vector3 &pos, float size) { set(pos, size); }
	~BoundingBox() {}
private:

	void set(Vector3 &pos, float size) { m_pos = pos; m_size = size; }

	Vector3 m_pos;
	float m_size;


};


class FrustumPlane
{
public:
	FrustumPlane() { m_a = 0; m_b = 0; m_c = 0; m_d = 0;};
	~FrustumPlane() {};

	void normalize();

	float m_a;
	float m_b;
	float m_c;
	float m_d;
};

class FrustumCull
{
public:
	FrustumCull();
	~FrustumCull();

	void setup();

	bool testPoint(Vector3 &point);
	bool testSphere(Vector3 &coord, float radius);
	bool testSphere(BoundingSphere &sphere);
	bool testBox(BoundingBox &box);

private:

	FrustumPlane **m_frustum;



};

