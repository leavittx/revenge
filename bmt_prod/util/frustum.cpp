#include "frustum.h"

void FrustumPlane::normalize()
{
	float mag = sqrtf(m_a * m_a + m_b * m_b + m_c * m_c);

	m_a *= 1.0f / mag;
	m_b *= 1.0f / mag;
	m_c *= 1.0f / mag;
	m_d *= 1.0f / mag;


}

FrustumCull::FrustumCull()
{
	int i = 0;
	m_frustum = new FrustumPlane*[6];

	for (i = 0; i < 6; i++)
	{
		m_frustum[i] = new FrustumPlane();

	}
}

FrustumCull::~FrustumCull()
{
	int i = 0;

	for (i = 0; i < 6; i++)
	{
		delete m_frustum[i];
	}
	delete [] m_frustum;

}

void FrustumCull::setup()
{
	//TODO: 4x4 matriisit?
	float   projectionMatrix[16];
	float   modelViewMatrix[16];
	float   clippingPlanes[16];

	glGetFloatv( GL_PROJECTION_MATRIX, projectionMatrix );
	glGetFloatv( GL_MODELVIEW_MATRIX, modelViewMatrix );

	//clipplanet = modelview * projection

	clippingPlanes[0] = modelViewMatrix[0] * projectionMatrix[0] + modelViewMatrix[1] * projectionMatrix[4] + modelViewMatrix[2] * projectionMatrix[8] + modelViewMatrix[3] * projectionMatrix[12];
	clippingPlanes[1] = modelViewMatrix[0] * projectionMatrix[1] + modelViewMatrix[1] * projectionMatrix[5] + modelViewMatrix[2] * projectionMatrix[9] + modelViewMatrix[3] * projectionMatrix[13];
	clippingPlanes[2] = modelViewMatrix[0] * projectionMatrix[2] + modelViewMatrix[1] * projectionMatrix[6] + modelViewMatrix[2] * projectionMatrix[10] + modelViewMatrix[3] * projectionMatrix[14];
	clippingPlanes[3] = modelViewMatrix[0] * projectionMatrix[3] + modelViewMatrix[1] * projectionMatrix[7] + modelViewMatrix[2] * projectionMatrix[11] + modelViewMatrix[3] * projectionMatrix[15];

	clippingPlanes[4] = modelViewMatrix[4] * projectionMatrix[0] + modelViewMatrix[5] * projectionMatrix[4] + modelViewMatrix[6] * projectionMatrix[8] + modelViewMatrix[7] * projectionMatrix[12];
	clippingPlanes[5] = modelViewMatrix[4] * projectionMatrix[1] + modelViewMatrix[5] * projectionMatrix[5] + modelViewMatrix[6] * projectionMatrix[9] + modelViewMatrix[7] * projectionMatrix[13];
	clippingPlanes[6] = modelViewMatrix[4] * projectionMatrix[2] + modelViewMatrix[5] * projectionMatrix[6] + modelViewMatrix[6] * projectionMatrix[10] + modelViewMatrix[7] * projectionMatrix[14];
	clippingPlanes[7] = modelViewMatrix[4] * projectionMatrix[3] + modelViewMatrix[5] * projectionMatrix[7] + modelViewMatrix[6] * projectionMatrix[11] + modelViewMatrix[7] * projectionMatrix[15];

	clippingPlanes[8] = modelViewMatrix[8] * projectionMatrix[0] + modelViewMatrix[9] * projectionMatrix[4] + modelViewMatrix[10] * projectionMatrix[8] + modelViewMatrix[11] * projectionMatrix[12];
	clippingPlanes[9] = modelViewMatrix[8] * projectionMatrix[1] + modelViewMatrix[9] * projectionMatrix[5] + modelViewMatrix[10] * projectionMatrix[9] + modelViewMatrix[11] * projectionMatrix[13];
	clippingPlanes[10] = modelViewMatrix[8] * projectionMatrix[2] + modelViewMatrix[9] * projectionMatrix[6] + modelViewMatrix[10] * projectionMatrix[10] + modelViewMatrix[11] * projectionMatrix[14];
	clippingPlanes[11] = modelViewMatrix[8] * projectionMatrix[3] + modelViewMatrix[9] * projectionMatrix[7] + modelViewMatrix[10] * projectionMatrix[11] + modelViewMatrix[11] * projectionMatrix[15];

	clippingPlanes[12] = modelViewMatrix[12] * projectionMatrix[0] + modelViewMatrix[13] * projectionMatrix[4] + modelViewMatrix[14] * projectionMatrix[8] + modelViewMatrix[15] * projectionMatrix[12];
	clippingPlanes[13] = modelViewMatrix[12] * projectionMatrix[1] + modelViewMatrix[13] * projectionMatrix[5] + modelViewMatrix[14] * projectionMatrix[9] + modelViewMatrix[15] * projectionMatrix[13];
	clippingPlanes[14] = modelViewMatrix[12] * projectionMatrix[2] + modelViewMatrix[13] * projectionMatrix[6] + modelViewMatrix[14] * projectionMatrix[10] + modelViewMatrix[15] * projectionMatrix[14];
	clippingPlanes[15] = modelViewMatrix[12] * projectionMatrix[3] + modelViewMatrix[13] * projectionMatrix[7] + modelViewMatrix[14] * projectionMatrix[11] + modelViewMatrix[15] * projectionMatrix[15];

	m_frustum[FRUSTUM_SIDE_RIGHT]->m_a = clippingPlanes[3] - clippingPlanes[0];
	m_frustum[FRUSTUM_SIDE_RIGHT]->m_b = clippingPlanes[7] - clippingPlanes[4];
	m_frustum[FRUSTUM_SIDE_RIGHT]->m_c = clippingPlanes[11] - clippingPlanes[8];
	m_frustum[FRUSTUM_SIDE_RIGHT]->m_d = clippingPlanes[15] - clippingPlanes[12];
	m_frustum[FRUSTUM_SIDE_RIGHT]->normalize();

	m_frustum[FRUSTUM_SIDE_LEFT]->m_a = clippingPlanes[3] + clippingPlanes[0];
	m_frustum[FRUSTUM_SIDE_LEFT]->m_b = clippingPlanes[7] + clippingPlanes[4];
	m_frustum[FRUSTUM_SIDE_LEFT]->m_c = clippingPlanes[11] + clippingPlanes[8];
	m_frustum[FRUSTUM_SIDE_LEFT]->m_d = clippingPlanes[15] + clippingPlanes[12];
	m_frustum[FRUSTUM_SIDE_LEFT]->normalize();

	m_frustum[FRUSTUM_SIDE_BOTTOM]->m_a = clippingPlanes[3] + clippingPlanes[1];
	m_frustum[FRUSTUM_SIDE_BOTTOM]->m_b = clippingPlanes[7] + clippingPlanes[5];
	m_frustum[FRUSTUM_SIDE_BOTTOM]->m_c = clippingPlanes[11] + clippingPlanes[9];
	m_frustum[FRUSTUM_SIDE_BOTTOM]->m_d = clippingPlanes[15] + clippingPlanes[13];
	m_frustum[FRUSTUM_SIDE_BOTTOM]->normalize();

	m_frustum[FRUSTUM_SIDE_TOP]->m_a = clippingPlanes[3] - clippingPlanes[1];
	m_frustum[FRUSTUM_SIDE_TOP]->m_b = clippingPlanes[7] - clippingPlanes[5];
	m_frustum[FRUSTUM_SIDE_TOP]->m_c = clippingPlanes[11] - clippingPlanes[9];
	m_frustum[FRUSTUM_SIDE_TOP]->m_d= clippingPlanes[15] - clippingPlanes[13];
	m_frustum[FRUSTUM_SIDE_TOP]->normalize();

	m_frustum[FRUSTUM_SIDE_BACK]->m_a = clippingPlanes[3] - clippingPlanes[2];
	m_frustum[FRUSTUM_SIDE_BACK]->m_b = clippingPlanes[7] - clippingPlanes[6];
	m_frustum[FRUSTUM_SIDE_BACK]->m_c = clippingPlanes[11] - clippingPlanes[10];
	m_frustum[FRUSTUM_SIDE_BACK]->m_d = clippingPlanes[15] - clippingPlanes[14];
	m_frustum[FRUSTUM_SIDE_BACK]->normalize();

	m_frustum[FRUSTUM_SIDE_FRONT]->m_a = clippingPlanes[3] + clippingPlanes[2];
	m_frustum[FRUSTUM_SIDE_FRONT]->m_b = clippingPlanes[7] + clippingPlanes[6];
	m_frustum[FRUSTUM_SIDE_FRONT]->m_c = clippingPlanes[11] + clippingPlanes[10];
	m_frustum[FRUSTUM_SIDE_FRONT]->m_d = clippingPlanes[15] + clippingPlanes[14];
	m_frustum[FRUSTUM_SIDE_FRONT]->normalize();

}

bool FrustumCull::testPoint(Vector3 &point)
{
	int i = 0;

	for (i = 0; i < 6; i++ )
	{
		if (m_frustum[i]->m_a * point.x +
			m_frustum[i]->m_b * point.y +
			m_frustum[i]->m_c * point.z +
			m_frustum[i]->m_d <= 0)
		{
			return false;
		}
	}

	return true;
}

bool FrustumCull::testSphere(Vector3 &coord, float radius)
{

	int i = 0;

	for (i = 0; i < 6; i++ )
	{
		if (m_frustum[i]->m_a * coord.x +
			m_frustum[i]->m_b * coord.y +
			m_frustum[i]->m_c * coord.z +
			m_frustum[i]->m_d <= -radius)
		{
			return false;
		}
	}

	return true;
}

bool FrustumCull::testSphere(BoundingSphere &sphere)
{

	int i = 0;

	for (i = 0; i < 6; i++ )
	{
		if (m_frustum[i]->m_a * sphere.m_pos.x +
			m_frustum[i]->m_b * sphere.m_pos.y +
			m_frustum[i]->m_c * sphere.m_pos.z +
			m_frustum[i]->m_d <= -sphere.m_radius)
		{
			return false;
		}
	}

	return true;
}

bool FrustumCull::testBox(BoundingBox &box)
{
	Vector3& pos = box.m_pos;
	float size = box.m_size;
	for (int i = 0; i < 6; i++)
	{
		if (m_frustum[i]->m_a * (pos.x - size) + m_frustum[i]->m_b * (pos.y - size) + m_frustum[i]->m_c * (pos.z - size)
			+ m_frustum[i]->m_d > 0)
			continue;
		if (m_frustum[i]->m_a * (pos.x - size) + m_frustum[i]->m_b * (pos.y + size) + m_frustum[i]->m_c * (pos.z - size)
			+ m_frustum[i]->m_d > 0)
			continue;
		if (m_frustum[i]->m_a * (pos.x - size) + m_frustum[i]->m_b * (pos.y + size) + m_frustum[i]->m_c * (pos.z + size)
			+ m_frustum[i]->m_d > 0)
			continue;
		if (m_frustum[i]->m_a * (pos.x - size) + m_frustum[i]->m_b * (pos.y - size) + m_frustum[i]->m_c * (pos.z + size)
			+ m_frustum[i]->m_d > 0)
			continue;
		if (m_frustum[i]->m_a * (pos.x + size) + m_frustum[i]->m_b * (pos.y - size) + m_frustum[i]->m_c * (pos.z - size)
			+ m_frustum[i]->m_d > 0)
			continue;
		if (m_frustum[i]->m_a * (pos.x + size) + m_frustum[i]->m_b * (pos.y + size) + m_frustum[i]->m_c * (pos.z - size)
			+ m_frustum[i]->m_d > 0)
			continue;
		if (m_frustum[i]->m_a * (pos.x + size) + m_frustum[i]->m_b * (pos.y + size) + m_frustum[i]->m_c * (pos.z + size)
			+ m_frustum[i]->m_d > 0)
			continue;
		if (m_frustum[i]->m_a * (pos.x + size) + m_frustum[i]->m_b * (pos.y - size) + m_frustum[i]->m_c * (pos.z + size)
			+ m_frustum[i]->m_d > 0)
			continue;

		return false;
	}
	return true;

}

