#pragma once

#include "../globals.h"

class Timer
{
public:
    Timer();
    ~Timer();

    void init();
    void start();
    void stop();
    void setPaused(bool paused);
    void addTime(int add);
    void update(int audioTime);
    int getTime();

private:
    bool m_paused;
    int m_initialTime;
    int m_previousTime;
    int m_elapsedTime;

};
