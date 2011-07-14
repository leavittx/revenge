#include "linearcurve.h"

LinearCurve::LinearCurve()
{
}

LinearCurve::~LinearCurve()
{
}

Vector3 LinearCurve::getValue(float t)
{
	int size = m_points.size();
	if (size > 1)
	{
		//there are at least two points on the curve

		int index1 = (int)floor(t * size);
		int index2 = index1 + 1;

		Vector3 &p1 = m_points[index1];
		Vector3 &p2 = m_points[index2];

		return Vector3(0, 0, 0);

	}
	else
	{
		return Vector3(0, 0, 0);
	}
}

Vector3 LinearCurve::getTangent(float t)
{
	return Vector3(0, 0, 0);
}

void LinearCurve::begin()
{
	m_points.clear();
}

void LinearCurve::end()
{
}

void LinearCurve::addPoint(Vector3 &point)
{
	m_points.push_back(point);
}
