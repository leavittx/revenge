#pragma once

//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

//#include "fmod.h"
//#include "fmod_errors.h"

#ifdef _WIN32
#include "fmod/fmod.hpp"
#include "fmod/fmod_errors.h"
#else
#include <fmodex/fmod.hpp>
#include <fmodex/fmod_errors.h>
#endif

class Song;

class Sound
{
	friend class Song;

public:

	~Sound();

	static Sound *create();
	static bool checkEnabled();
	bool init();

	void setVolume( Song *music, float i );
	void close();
	void update();

	static bool checkSync();
	static void setEnabled(bool f);
	static bool getEnabled();
	static void setSync(bool f);

private:

	static bool createdFlag;
	static bool enabled;
	static Sound *instance;
	static bool syncFlag;
	static FMOD::System *fmodSystem;

	void checkError(FMOD_RESULT result);

	Sound() {}
	Sound(const Sound &) {}
	//Sound& operator = (const Sound &) {}
};


class Song
{
	friend class Sound;

public:

	Song();
	~Song();

	bool loadStream(char *name);
	bool loadStream2(char *memdata, int length);

	void start();
	void stop();
	void release();

	void changePosition(int delta);
	void setPosition(unsigned int offset);
	void setPaused(bool f);
	void togglePause();

	int getLength();
	int getPosition();
	bool checkPlaying();
	void getWaveData(float *array, int size); //pcm
	void getSpectrum(float *array, int size); //spectrum

private:
	bool isPlaying;
	FMOD::Sound *stream;
	float volume;
	char *data;
	unsigned int length;
	FMOD::Channel *m_channel;
};


