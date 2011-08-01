#pragma once

#include "../globals.h"

enum CAMERA_TYPE
{
	NONE = 0,
	LINEAR,
	CATMULL_ROM,
	CURVE
};

class CatmullRomCurve;

class Camera
{
	//	friend class Parser;
public:
	Camera();
	~Camera();

	void use();
	void use(float t);

	//this is probably pretty dumb
	void initLinear(Vector3 startPosition, Vector3 endPosition,
					Vector3 startTarget, Vector3 endTarget,
					Vector3 startUp, Vector3 endUp,
					int startTime, int endTime);

	void initCatmullRom(CatmullRomCurve& positionSpline,
						CatmullRomCurve& targetSpline,
						int startTime, int endTime);

private:
	CAMERA_TYPE m_type;
	int m_startTime;
	int m_endTime;

	//catmull-rom spline
	class CatmullRomCurve *m_positionSpline;
	class CatmullRomCurve *m_targetSpline;

	//linear camera
	Vector3 m_startPosition;
	Vector3 m_endPosition;
	Vector3 m_startTarget;
	Vector3 m_endTarget;
	Vector3 m_startUp;
	Vector3 m_endUp;

};
