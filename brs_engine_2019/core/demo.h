#pragma once

#include "../globals.h"
#include "sound.h"
#include "config.h"
#include "frametimer.h"

/*
    An entry in the timeline. It has a pointer to the actual effect that it'll run, plus a bunch of other related stuff.
*/

class TimelineEntry
{
public:
    TimelineEntry();
    ~TimelineEntry();
    void destroy();

    //comparison operator for STL sort
    inline bool operator < (const TimelineEntry& otherEntry) const
    {
        return m_priority < otherEntry.m_priority;
    }
    Scene *m_scene; //ptr to the scene
    FrameTimer *m_frametimer; //frameskipper
    int m_startTime; //starts here
    int m_endTime;   //ends then
    int m_priority;  //priority in which the simultaneous effects will be drawn. bigger priority will be drawn first
    string m_name;   //name with what this is referenced
};

/*
    The main class for the demo.
*/


class Demo
{
public:
    Demo(string scriptfile);
    ~Demo();

    //returns true if we are all go for launch
    bool initOk();

    //add a scene to the scene list
    void addScene(string name, Scene* s);
    //add a scene to the timeline
    void addSceneToTimeline(string name, int startTime, int endTime, int priority);
    //get scene from the scene list
    Scene* getScene(string name);

    //inits all effects
    void initEffects();
    //releases all effects
    void releaseEffects();
    //toggles running state
    void toggleRunning();

    //start the demo
    void start();
    //stop the demo
    void stop();

    //draw currently active scenes
    void draw();
    //update currently active scenes
    void update();
    //clear everything
    void destroy();


private:
    //this contains all the effects
    std::map<string, Scene*> m_scenes;
    //this contains all the timeline entries that will be played
    std::vector<TimelineEntry*> m_timeline;

    Config *m_cfg;
    bool m_start;
    string m_scriptName;
    bool m_running;


};

