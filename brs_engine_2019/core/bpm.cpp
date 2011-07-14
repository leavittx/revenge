#include "bpm.h"

BPM::BPM()
{
}
BPM::~BPM()
{
	m_framecount = 0;
	m_value = 0;
	m_offset = 0;
	m_previousbeatdist = 0;
    m_beat = 0.0f;

}

//  value = beats per min | offset - time between ticks
void BPM::init(float value, int offset)
{
	m_framecount = 0;
	m_value = (float)value;
	m_offset = offset;
	m_previousbeatdist = 0;
    m_beat = 0.0f;

	m_changed = false;

}

void BPM::update()
{
	const int bpmmicroseconds = (int)(60000000/m_value);
	int aika = g_system.getTime();//(int)(dmsGetTimeMs());    
	aika *= 1000;

	m_changed = false;

	//lasketaan etäisyys biittiin
	int beatdist = (aika + m_offset) % bpmmicroseconds;

	if (beatdist < m_previousbeatdist)
	{
		m_changed = true;
		m_framecount++;
	}

	m_beat = (bpmmicroseconds-beatdist)/(float)bpmmicroseconds;
	m_previousbeatdist = beatdist;
}

float BPM::getValue()
{
	return m_beat;
}

int BPM::getBeatcount()
{
	return m_framecount;
}

bool BPM::getChanged()
{
	return m_changed;
}