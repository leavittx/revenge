#include <algorithm>
#include "demo.h"
#include "parser.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          TimelineEntry
//////////////////////////////////////////////////////////////////////////////////////////////////////////

TimelineEntry::TimelineEntry()
{
	m_endTime = 0;
	m_startTime = 0;
	m_scene = 0;
	m_priority = 0;
	m_name = "";
	m_frametimer = NULL;
}
TimelineEntry::~TimelineEntry()
{
	destroy();
}

void TimelineEntry::destroy()
{
	if (m_frametimer != NULL)
	{
		delete m_frametimer;
		m_frametimer = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                Demo
//////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Demo::initOk()
{
	return m_start;
}
Demo::Demo(string scriptfile):m_scriptName(scriptfile)
{
	g_debug << "initializing demo" << endl;

	//initialize configuration dialog
	m_cfg = new Config();
	m_cfg->run();

	if (m_cfg->getRunFlag())
	{
		//parse script
		Parser parser;
		parser.parse(scriptfile, g_params, g_system);

		g_system.initOpenGL(*m_cfg);

		//		string filename = "data\\audio\\bitarts_nomoretomorrow.ogg";
		//		string filename = "data\\audio\\bitarts_nomoretomorrow.ogg";
#ifdef _WIN32
		string songPath = "data\\audio\\";
#else
		string songPath = "data/audio/";
#endif
		string songFile = g_system.getSongFilename();
		g_debug << "loading sound " << songFile << endl;
		m_start = true;

		const int SOUND_LOADING_STEPS = 30; //put a lot in it since it takes long
		const int SOUND_PRESTEP = 2;
		g_system.addLoadingScreenSteps(SOUND_LOADING_STEPS);
		g_system.advanceLoadingScreen(SOUND_PRESTEP);

		//draw almost empty loading screen
		g_system.drawLoadingScreen();

		g_system.initSound(songPath + songFile);
		g_system.advanceLoadingScreen(SOUND_LOADING_STEPS-SOUND_PRESTEP);
	}
	else
	{
		m_start = false;
	}

}

Demo::~Demo()
{
	g_debug << "demo quitting.." << endl;
	destroy();
	g_system.kill();
}

void Demo::start()
{
	g_debug << "starting demo now!" << endl;
	m_running = true;
	g_system.startSong();
}

void Demo::stop()
{
	g_system.stopSong();
	g_debug << "demo stopped!" << endl;
}

void Demo::destroy()
{
	if (m_start)
	{
		map<string, Scene *>::iterator it;
		vector<TimelineEntry*>::iterator it2;

		//clear out all the scenes in the demo
		for (it = m_scenes.begin(); it != m_scenes.end(); it++)
		{
			delete (*it).second;
		}
		//and the timeline
		for (it2 = m_timeline.begin(); it2 < m_timeline.end(); it2++)
		{
			(*it2)->destroy();
		}
		m_timeline.clear();
		m_scenes.clear();

		//kill audio
		g_system.freeSound();
	}
}

void Demo::addScene(string name, Scene *s)
{
	//insert the scene to the map
	m_scenes[name] = s;
	g_debug << "added scene \"" << name << "\" to the demo" << endl;
}

Scene* Demo::getScene(string name)
{
	if (m_scenes.find(name) != m_scenes.end())
	{
		return m_scenes[name];
	}
	else
	{
		g_debug << "error! Trying to fetch scene " << name << " but it does not exist\n";
		return 0;
	}
}


void Demo::addSceneToTimeline(string name, int startTime, int endTime, int priority)
{
	//check if such a scene exists
	if (m_scenes.find(name) != m_scenes.end())
	{
		//create timeline entry for it
		TimelineEntry* entry = new TimelineEntry();

		entry->m_scene = m_scenes[name];
		entry->m_startTime = startTime;
		entry->m_endTime = endTime;
		entry->m_priority = priority;
		entry->m_name = name;
		const int FPS = 100;
		const int MAXITERATIONS = 30;
		entry->m_frametimer = new FrameTimer(1000/FPS, MAXITERATIONS); //
		g_debug << "created a timeline entry for scene \"" << name << "\" at [" << startTime << "," << endTime << "]" << endl;

		m_timeline.push_back(entry);
	}
	else
	{
		g_debug << "ERROR! no scene with name \"" << name << "\" exists!" << endl;
		//no such scene exists
	}
}

void Demo::initEffects()
{
	//call init on all effects
	map<string, Scene *>::iterator it;

	for (it = m_scenes.begin(); it != m_scenes.end(); it++)
	{
		(*it).second->init();
	}
}

void Demo::releaseEffects()
{
	//call init on all effects
	map<string, Scene *>::iterator it;

	for (it = m_scenes.begin(); it != m_scenes.end(); it++)
	{
		(*it).second->release();
	}
}


//we need this since STL doesn't know how to sort vectors of pointers 
bool timelineEntryComparisonFunction(const TimelineEntry *a, const TimelineEntry *b)
{
	return a->m_priority < b->m_priority;
}

void Demo::draw()
{
	vector<TimelineEntry*>::iterator it;
	vector<TimelineEntry*> activeEffects;

	int time = g_system.getTime();
	for (it = m_timeline.begin(); it < m_timeline.end(); it++)
	{
		TimelineEntry *e = *it;

		if (time >= e->m_startTime && time < e->m_endTime)
		{
			activeEffects.push_back(e);
		}
	}
	sort(activeEffects.begin(), activeEffects.end(), timelineEntryComparisonFunction);
	for (it = activeEffects.begin(); it < activeEffects.end(); it++)
	{
		TimelineEntry *e = *it;
		e->m_scene->draw();
	}

}

void Demo::toggleRunning()
{
	if (m_running)
	{
		m_running = false;
		g_system.pauseSong(true);
	}
	else
	{
		m_running = true;
		g_system.pauseSong(false);
	}
}
void Demo::update()
{
#ifdef _WIN32
	bool F1 = GetAsyncKeyState(VK_F1) != 0; //reload params & shaders
	bool F2 = GetAsyncKeyState(VK_F2) != 0; //reload params & shaders & reinit effects
	bool F3 = GetAsyncKeyState(VK_F3) != 0; //reload resources
	bool S = GetAsyncKeyState('S') != 0;	//toggle sound

	if (F1 || F2)
	{
		Parser parser;
		parser.parse(m_scriptName, g_params, g_system);
		g_shaders.freeShaders();
		g_shaders.loadShaders();
	}
	if (F2)
	{
		releaseEffects();
		initEffects();
	}
	if (F3)
	{
	}

	if (S)
	{
		bool sound = g_system.getSoundEnabled();
		g_system.setSoundEnabled(!sound);
	}

	static bool returnDown = false;
	static int returnTime = 0;
	if (GetAsyncKeyState(VK_RETURN))
	{
		if (!returnDown)
		{
			//write down time if return was pressed
			returnDown = true;
			returnTime = g_system.getTime();
		}
	}
	else
	{
		if (returnDown)
		{
			//return was raised
			int returnLength = g_system.getTime() - returnTime;
			stringstream output;
			output << returnTime;
			output << " ";
			output << returnLength;
			g_debug + (output.str());//(returnTime + " " + returnLength);
			returnDown = false;
		}
	}
#else
	//TODO: that all doesn't work at all :(
	bool F1 = g_system.getKeyDown(KeyF1) != 0;  //reload params & shaders
	bool F2 = g_system.getKeyDown(KeyF2) != 0;  //reload params & shaders & reinit effects
	bool F3 = g_system.getKeyDown(KeyF3) != 0;  //reload resources
	bool S = g_system.getKeyDown(KeyS) != 0;	//toggle sound
	bool F11 = g_system.getKeyDown(KeyF11) != 0; //reload fullscreen

	if (F1 || F2)
	{
		Parser parser;
		parser.parse(m_scriptName, g_params, g_system);
		g_shaders.freeShaders();
		g_shaders.loadShaders();
	}
	if (F2)
	{
		releaseEffects();
		initEffects();
	}
	if (F3)
	{
	}

	if (S)
	{
		bool sound = g_system.getSoundEnabled();
		g_system.setSoundEnabled(!sound);
	}

	if (F11)
	{
		//Segmentation fault on glxSwapBuffers() call
		//        GLWindow glWindow = g_system.getGLWindow();
		//        glWindow.toggleFullscreen();
	}
#endif

	if (m_running)
	{
		vector<TimelineEntry*>::iterator it;
		int time = g_system.getTime();

		for (it = m_timeline.begin(); it < m_timeline.end(); it++)
		{
			TimelineEntry *e = *it;
			if (time >= e->m_startTime && time < e->m_endTime)
			{
				float t = Math::calcPosInt(time, e->m_startTime, e->m_endTime);

				e->m_scene->setPosition(t);
				e->m_frametimer->update(time);
				while (e->m_frametimer->stepsLeft())
				{
					e->m_scene->update();
					e->m_frametimer->endStep();
				}
			}
		}
	}
}
