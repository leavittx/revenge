#pragma once

#include "../globals.h"
#include "config.h"


class GLWindow;
class GLSystem;
class Demo;
class BPM;
class TriggerSystem;
class Event;
class Image;
class Texture;
class TextureParameters;
class Camera;
class FBOManager;
class Mesh;
class MeshInfo;
class Material;
class FrustumCull;
class Timer;

class System
{
public:
	System();
	~System();

	//initializing
	void init();
	void clear(); //clear out parameters & such
	void clearImages();
	void kill(); //free stuff

	//window handling and system
	bool initOpenGL(Config& cfg);
	GLWindow& getGLWindow();
	GLSystem& getGLSystem();

	//poll window events
	bool pollEvents();

	bool getKeyDown(int i);
	bool getKeyPressed(int i);
	void setWindowTitle(const string title);
	void swapBuffers();
	void resetViewport();
	bool demoRunning();

	void updateFPS();
	float getFPS();

	//projection
	void setOrthoMode(int width = 1024, int height = 768);
	void setPerspectiveMode();
	float getAspectRatio();

	//loading screen
	void addLoadingScreenSteps(int count);
	void advanceLoadingScreen(int count);
	void drawLoadingScreen();

	//update demo
	void handleInput(Demo *demo);
	void update();
	void updateSync();

	//timer
	int getTime();
	void addTime(int adjust);


	//sounds
	void setSoundEnabled(bool enabled);
	bool getSoundEnabled();
	void setSongFilename(string filename);
	void setEndTime(int time);
	string getSongFilename();
	bool initSound(const string& filename);
	void freeSound();
	void startSong();
	void stopSong();
	void pauseSong(bool pause);
	void getWaveData(float *array, int size);
	void getSpectrum(float *array, int size);
	float getSpectrumSum(int start = 0, int end = 512);

	//	void changeSongPosition(int adjust);
	//	int getSongPosition();


	//meshes
	void loadMeshes();
	void uploadMeshes();
	void addMeshInfo(string name, MeshInfo *info);
	void addMesh(string name, Mesh *mesh);
	void addMaterial(string name, Material *material);
	Mesh& mesh(string name);
	Material& material(string name);

	//additional stuff
	void addBeat(string name, BPM *bpm);
	void addTriggers(string name, TriggerSystem *triggers);
	void addEvent(string name, Event *e);
	void addCamera(string name, Camera *c);

	//accessors
	BPM& beat(string name);
	TriggerSystem& triggers(string name);
	Event& event(string name);
	Camera& camera(string name);

	//frustum
	FrustumCull& getFrustum();

	//debug
	void glDebug(string text);
	void dumpState();

	//demo specific
	bool getRandomized();

private:
	//loading screen
	int m_loadingScreenSteps;
	int m_loadingScreenPosition;

	//FPS
	int m_frameCount;
	int m_framePreviousTime;
	int m_frameTimer;
	float m_FPS;

	//GL stuff
	bool createWindow(Config &cfg);
	bool createSystem(Config &cfg);
	class GLWindow *m_glWindow;
	class GLSystem *m_glSystem;

	int m_endTime;

	//sound
	Sound *m_audio;
	Song *m_song;
	string m_songFile;
	bool m_soundEnabled;
	bool m_dialogSoundEnabled;
	bool m_randomized;
	int getAudioPosition();

	//timer
	Timer *m_timer;

	//textures
	map<string, MeshInfo *> m_meshInfos;

	//other included stuff
	map<string, BPM*> m_beats;
	map<string, TriggerSystem*> m_triggers;
	map<string, Event*> m_events;
	map<string, Camera*> m_cameras;
	map<string, Mesh*> m_meshes;
	map<string, Material*> m_materials;

	//frustum
	FrustumCull *m_frustumCull;
};


