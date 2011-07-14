#include "timer.h"
#include <mmsystem.h>

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::init()
{
	m_initialTime = timeGetTime();
	m_previousTime = m_initialTime;
	m_paused = false;
	m_elapsedTime = 0;
}
void Timer::start()
{
}
void Timer::stop()
{
}
void Timer::addTime(int add)
{
	m_elapsedTime += add;
	if (m_elapsedTime < 0)
	{
		m_elapsedTime = 0;
	}
}
void Timer::setPaused(bool paused)
{
	m_paused = paused;
}
void Timer::update(int audioPosition)
{
	int currentTime = timeGetTime();
	int dt = currentTime - m_previousTime;
	m_previousTime = currentTime;

	if (m_paused)
	{
	}
	else
	{
		m_elapsedTime += dt;

		const int LIMIT = 30;
		if (abs(m_elapsedTime - audioPosition) > LIMIT)
		{
			g_debug << "audio out of sync! timer elapsed time = " << m_elapsedTime << " audio timer = " << audioPosition << "\n";
			m_elapsedTime = audioPosition;
		}
	}
}

int Timer::getTime()
{
	return m_elapsedTime;
}