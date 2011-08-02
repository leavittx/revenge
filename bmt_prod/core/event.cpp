#include "event.h"

float Event::getValue()
{
	int time = g_system.getTime();
	float value = 0.0f;
	if (time >= m_time && time < m_time + m_length)
	{
		value = (time - m_time) / (float)m_length;
	}
	else if (time >= m_time + m_length)
	{
		value = 1.0f;
	}
	else
	{
		value = 0.0f;
	}
	return value;
}

bool Event::hasPassed()
{
	int time = g_system.getTime();
	
	return time >= m_time;
}
