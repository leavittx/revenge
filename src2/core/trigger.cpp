#include "trigger.h"

TriggerSystem::TriggerSystem()
{
	m_triggers.clear();
}

TriggerSystem::~TriggerSystem()
{
}

bool triggerComparisonFunction(const Trigger& a, const Trigger& b)
{
	return a.m_time < b.m_time;
}

void TriggerSystem::addTrigger(Trigger &trigger)
{
	m_triggers.push_back(trigger);
	sort(m_triggers.begin(), m_triggers.end(), triggerComparisonFunction);
}

float TriggerSystem::getValue()
{
	float value = 0.0;
	int time = g_system.getTime();
	vector<Trigger>::iterator it;

	for (it = m_triggers.begin(); it < m_triggers.end(); it++)
	{
		Trigger &t = *it;
		if (time >= t.m_time && time < t.m_time + t.m_length)
		{
			//1 kun time == t.time
			//0 kun time == t.time + length
			float pos = 1.0f - (time - t.m_time) / (float)t.m_length;
			value += pos;

		}
	}
	//clamp
	value = min(value, 1);
	return value;
}

void TriggerSystem::getTrigger(int index, int& time, int &length)
{
	int size = getCount();
	if (index < 0)
	{
		g_debug << "trying to get trigger that has the index " << index << endl;
		return;
	}
	if (index >= size)
	{
		g_debug << "trying to get trigger " << index << " from the system that has only " << size << " triggers" << endl;
		return;
	}
	time = m_triggers[index].m_time;
	length = m_triggers[index].m_length;
}


int TriggerSystem::getPosition()
{
	int time = g_system.getTime();
	vector<Trigger>::iterator it;

	int count = 0;
	int size = (int)m_triggers.size();

	for (it = m_triggers.begin(); it < m_triggers.end(); it++)
	{
		Trigger &t = *it;
		if (time < t.m_time)
		{
			break;
		}
		count++;
	}
	return count;

}

int TriggerSystem::getCount()
{
	return m_triggers.size();
}