#include "bezier.h"


BezierCurve::BezierCurve()
{
	points = 0;
	tangents = 0;
}

BezierCurve::~BezierCurve()
{
	this->destroy();
}

void BezierCurve::destroy()
{
	if (points != 0)
	{
		delete [] points;
	}
	if (tangents != 0)
	{
		delete [] tangents;
	}
}
void BezierCurve::startCreation()
{
	this->destroy();

}
void BezierCurve::addPoint(Vector3 &point, Vector3 &tangent)
{
	this->insertedPoints.push_back(point);
	this->insertedTangents.push_back(tangent);
}

void BezierCurve::endCreation()
{
	//destroy() kutsuttu jo aikaisemmin
	this->pcount = this->insertedPoints.size();

	this->points = new Vector3[this->pcount];
	this->tangents = new Vector3[this->pcount];

	std::vector<Vector3>::iterator i;

	int pointoffset = 0;
	for (i = this->insertedPoints.begin(); i != this->insertedPoints.end(); i++)
	{
		Vector3 p = *i;
		this->points[pointoffset++] = p;
	}

	pointoffset = 0;
	for (i = this->insertedTangents.begin(); i != this->insertedTangents.end(); i++)
	{
		Vector3 p = *i;
		this->tangents[pointoffset++] = p;
	}
	this->insertedPoints.clear();
	this->insertedTangents.clear();
}


Vector3 BezierCurve::getValue(float pos)
{
	//    float val = (pos * (pcount-3)) + 1;
	//    const int index = ((int)floorf(pos * (pcount-3))) + 1;

	if (pos > 1.0000f)
	{
		return Vector3(0, 0, 0);
	}
	float val = pos * (pcount-1);
	const int index = (int)floorf(pos * (pcount-1));

	float t = val - index;

	float t2 = t*t;
	float t3 = t2*t;

	Vector3 p0 = points[index];
	Vector3 p1 = tangents[index];
	Vector3 p2 = tangents[index + 1];
	Vector3 p3 = points[index + 1];

	Vector3 result = p0*(1-t)*(1-t)*(1-t) + p1*3*t*(1-t)*(1-t) + p2*3*t2*(1-t)+p3*t3;
	return result;

}
