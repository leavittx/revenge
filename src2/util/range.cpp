#include "range.h"

Range::Range()
{
	m_minimum = 0;
	m_maximum = 0;
}
Range::Range(float minimum, float maximum)
{
	set(minimum, maximum);
}
Range::~Range()
{
}

void Range::set(float minimum, float maximum)
{
	m_minimum = minimum;
	m_maximum = maximum;
}


float Range::getValue(float t)
{
	return m_minimum + (m_maximum - m_minimum) * t;
}

float Range::lerp(float t)
{
	if (t < 0.0f)
		return m_minimum;
	if (t > 1.0f)
		return m_maximum;
	return m_minimum + (m_maximum - m_minimum) * t;
}
float Range::getRandomValue()
{
	return m_minimum + (m_maximum - m_minimum) * (float)((rand()%RAND_MAX)/(float)RAND_MAX);
}