#pragma once

/*
    A constant framerate timer / frameskipping.
    
*/


class FrameTimer
{
public:
    FrameTimer();
	~FrameTimer() {};
    //first number = interval between updates (ms), second number is the maximum number of iterations
    FrameTimer(int updatetime, int maxiterations);

    int getDeltaTime();
    void set(int updatetime, int maxiterations);

    //needs to be called once every frame
    void update(int currenttime);
    //during update loop until this returns false
    bool stepsLeft();
    //this goes to the end of the step
    void endStep();

private:
    int m_dt; //delta time 
    int m_prevtime; //time previous frame
    int m_cumultime; //accumulated time
    int m_updatetime; //how often the update is done (ms)
    int m_maxiterations; //maximum updates
};


