#pragma once

#include "../globals.h"

class LinearCurve : public Curve
{
public:
	LinearCurve();
	~LinearCurve();

	Vector3 getValue(float t);
	Vector3 getTangent(float t);

	void begin();
	void addPoint(Vector3 &point);
	void end();

private:
	vector<Vector3> m_points;

};