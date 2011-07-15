#pragma once

#include <string>
#include <vector>
#include <map>

#include "../globals.h"

class Event
{
public:
    Event() {};
    ~Event() {};

    float getValue();
    bool hasPassed();

    int m_time;
    int m_length;
};

