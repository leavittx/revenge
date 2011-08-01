#pragma once

#include <string>
#include <vector>
#include <map>

#include "../globals.h"

class Trigger
{
public:
	Trigger() {};
	~Trigger() {};

	int m_time;
	int m_length;
};

class TriggerSystem
{
public:
	TriggerSystem();
	~TriggerSystem();

	void addTrigger(Trigger &trigger);
	void getTrigger(int index, int& time, int &length);
	float getInterpolatedDistanceToNext();
	float getValue();
	int getPosition();
	int getCount();

private:
	vector<Trigger> m_triggers;



};

