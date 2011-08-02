#include "timer.h"

#ifdef _WIN32
#include <mmsystem.h>
#else
#include <sys/time.h>
#endif

Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::init()
{
#ifdef _WIN32
	m_initialTime = timeGetTime();
#else
	static struct timespec initial;
	clock_gettime(CLOCK_REALTIME, &initial);
	m_initialTime = (int)(initial.tv_nsec / 1000000.0);
#endif

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
#ifdef _WIN32
	int currentTime = timeGetTime();
#else
	static struct timespec current;
	clock_gettime(CLOCK_REALTIME, &current);
	int currentTime = (int)(current.tv_nsec / 1000000.0);
#endif

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
