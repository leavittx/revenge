#include "camera.h"

Camera::Camera()
{
	m_type = NONE;
	m_startTime = 0;
	m_endTime = 0;
	m_positionSpline = 0;
	m_targetSpline = 0;
}

Camera::~Camera()
{
	if (m_positionSpline)
	{
		delete m_positionSpline;
		m_positionSpline = 0;
	}
	if (m_targetSpline)
	{
		delete m_targetSpline;
		m_targetSpline = 0;
	}
}

void Camera::initLinear(Vector3 startPosition, Vector3 endPosition,
						Vector3 startTarget, Vector3 endTarget,
						Vector3 startUp, Vector3 endUp,
						int startTime, int endTime)
{
	m_type = LINEAR;
	m_startPosition = startPosition;
	m_endPosition = endPosition;
	m_startTarget = startTarget;
	m_endTarget = endTarget;
	m_startUp = startUp;
	m_endUp = endUp;
	m_startTime = startTime;
	m_endTime = endTime;
}

void Camera::initCatmullRom(CatmullRomCurve &positionSpline, 
							CatmullRomCurve &targetSpline,
							int startTime, int endTime)
{
	m_type = CATMULL_ROM;
	m_positionSpline = positionSpline.clone();
	m_targetSpline = targetSpline.clone();
	m_startTime = startTime;
	m_endTime = endTime;
}


void Camera::use()
{
	const int time = g_system.getTime();
	float t = Math::calcPosInt(time, m_startTime, m_endTime);

	use(t);
}

void Camera::use(float t)
{
	Vector3 position;
	Vector3 target;
	Vector3 upwards;

	switch(m_type)
	{
	case NONE:
		g_debug << "Trying to use an uninitialized camera!" << endl;

		break;

	case LINEAR:
	{
		position = m_startPosition + (m_endPosition - m_startPosition) * t;
		target = m_startTarget + (m_endTarget - m_startTarget) * t;
		upwards = m_startUp + (m_endUp - m_startUp) * t;
	} break;

	case CATMULL_ROM:
	{
		position = m_positionSpline->getValue(t);
		target = m_targetSpline->getValue(t);
		upwards = Vector3(0, 1, 0);
	} break;

	};
	glLoadIdentity();
	gluLookAt(position.x, position.y, position.z,
			  target.x, target.y, target.z,
			  upwards.x, upwards.y, upwards.z);

}
