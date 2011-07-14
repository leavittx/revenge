#pragma once

#include <math.h>
#include <stdlib.h>

class Range
{
    public:

		Range();
		Range(float minimum, float maximum);
		Range(const class Range &r) { m_minimum = r.m_minimum; m_maximum = r.m_maximum; }
		~Range();

		void set(float minimum, float maximum);
		float getValue(float t);
		float lerp(float t);
		float getRandomValue();
		float getMinimum() { return m_minimum; }
		float getMaximum() { return m_maximum; }

	private:
		float m_minimum;
		float m_maximum;
};

