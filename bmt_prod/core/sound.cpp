//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <string.h>

#ifdef _DEBUG
#include <stdlib.h>
//#include "mmgr.h"
#endif

#include "sound.h"
#include "../globals.h"

#ifdef _MSC_VER
#pragma warning ( disable : 4996)
#endif


//--------------------------------------------------------------------------------------------
//  Static members
//--------------------------------------------------------------------------------------------

bool Sound::createdFlag = false;
bool Sound::enabled = false;
bool Sound::syncFlag = false;
Sound *Sound::instance = NULL;
FMOD::System *Sound::fmodSystem = NULL;

//--------------------------------------------------------------------------------------------
//  Sound class code
//--------------------------------------------------------------------------------------------

Sound::~Sound()
{

}

Sound *Sound::create()
{
	if(!createdFlag)
	{
		instance = new Sound;
		if(!instance)
		{
			throw "Sound::create(): Memory allocation error";
			return NULL;
		}

		createdFlag = true;

		//		dmsMsg("Sound::create(): Sound instance created\n");
	}
	else
	{
		throw "One instance of class Sound allready running";
		return NULL;
	}

	return instance;
}

bool Sound::init()
{		
	FMOD_RESULT result;
	FMOD_CAPS   caps;
	FMOD_SPEAKERMODE speakermode;
	unsigned int version;
	char	name[256];

	result = FMOD::System_Create( &fmodSystem );
	if( result != FMOD_OK )
	{
		//        dmsMsg( "Sound::init() error: FMOD error! (%d) %s\n", result, FMOD_ErrorString( result ) );
		return false;
	}

	result = fmodSystem->getVersion( &version );
	if( result != FMOD_OK )
	{
		//        dmsMsg( "Sound::init() error: FMOD error! (%d) %s\n", result, FMOD_ErrorString( result ) );
		return false;
	}

	// Check for correct version
	if( version < FMOD_VERSION )
	{
		//		dmsMsg( "Sound::init() error: invalid FMOD version! This program requires %08x\n", FMOD_VERSION );
		return false;
	}

#if (FMOD_VERSION >= 0x00043600)
	result = fmodSystem->getDriverCaps(0, &caps, 0, &speakermode);
#else
	result = fmodSystem->getDriverCaps(0, &caps, 0, 0, &speakermode);
#endif
	checkError(result);

	result = fmodSystem->setSpeakerMode(speakermode);
	checkError(result);


	if (caps & FMOD_CAPS_HARDWARE_EMULATED)             // The user has the 'Acceleration' slider set to off!  This is really bad for latency!.
	{                                                   // You might want to warn the user about this.
		//        dmsMsg("Sound: Hardware not available. Allocating more buffer space.");
		result = fmodSystem->setDSPBufferSize(1024, 10);    // At 48khz, the latency between issuing an fmod command and hearing it will now be about 213ms.
		checkError(result);
	}

	//this seems to present only in windows/old version
#ifdef _WIN32
	result = fmodSystem->getDriverName(0, name, 256);
	checkError(result);

	if (strstr(name, "SigmaTel"))   // Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it.
	{
		//		dmsMsg("Sound: Running on SigmaTel drivers. Forcing PCM 16 bits");
		result = fmodSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
		checkError(result);
	}
#endif

	// Init FMOD
	result = fmodSystem->init( 100, FMOD_INIT_NORMAL, 0 );
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         // Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo...
	{
		//		dmsMsg( "Sound::init() error: FMOD::init() said: FMOD_ERR_OUTPUT_CREATEBUFFER -> Force stereomode and retry.");
		result = fmodSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		checkError(result);

		result = fmodSystem->init(100, FMOD_INIT_NORMAL, 0); // Replace with whatever channel count and flags you use!
		checkError(result);
	}

	return true;
}

void Sound::checkError(FMOD_RESULT  result)
{
	if (result != FMOD_OK)
	{
		//        dmsMsg("FMOD error (%d): %s ", result, FMOD_ErrorString(result));
	}

}

void Sound::setSync( bool f )
{
	syncFlag = f;
}

void Sound::setEnabled( bool f )
{
	enabled = f;
}

bool Sound::getEnabled( )
{
	return enabled;
}

bool Sound::checkSync()
{		
	return syncFlag;
}

bool Sound::checkEnabled()
{
	return enabled;
}

void Sound::setVolume( Song *music, float i )
{
	if( music )
	{
		if(getEnabled())
			music->volume=i;
		else
			music->volume=0;

		if( music->m_channel )
		{
			music->m_channel->setVolume(music->volume);
		}

	}
}

void Sound::update()
{
	if( instance && fmodSystem)
	{
		fmodSystem->update();
	}
}

void Sound::close()
{
	if( fmodSystem )
	{
		fmodSystem->close();
		fmodSystem->release();
	}

	if(createdFlag)
	{
		createdFlag = false;

		if(instance)
		{
			delete instance;
			instance = NULL;
		}

		//		dmsMsg("Sound::close(): Instance deleted\n");
	}
}

//--------------------------------------------------------------------------------------------
//  Song class code
//--------------------------------------------------------------------------------------------

Song::Song()
{
	stream = NULL;
	data = NULL;
	m_channel = NULL;
	isPlaying = false;
	volume=1.0f;
}

Song::~Song()
{

}

bool Song::loadStream( char *name )
{
	FMOD_RESULT result;
	FILE *fp;

	data = NULL;
	fp = fopen(name, "rb");

	if (!fp)
	{
		g_debug << "Cannot load sound stream " << name << endl;
		return false;
	}

	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	data = new char[length];
	if(!data) return false;

	fread(data, length, 1, fp);
	fclose(fp);

	FMOD_CREATESOUNDEXINFO exinfo;
	memset( &exinfo, 0, sizeof( FMOD_CREATESOUNDEXINFO ) );
	exinfo.cbsize = sizeof( FMOD_CREATESOUNDEXINFO );
	exinfo.length = length;

	//result = Sound::fmodSystem->createSound( name, FMOD_DEFAULT, false, &stream );
	result =  Sound::fmodSystem->createSound( data, FMOD_SOFTWARE | FMOD_OPENMEMORY | FMOD_2D, &exinfo, &stream );
	if( result != FMOD_OK )
	{
		//        dmsMsg( "Sound::init() error: FMOD error! (%d) %s\n", result, FMOD_ErrorString( result ) );
		return false;
	}

	stream->getLength( &length, FMOD_TIMEUNIT_MS );

	return true;
}

bool Song::loadStream2(char *memdata, int length)
{
	FMOD_RESULT result;
	FMOD_CREATESOUNDEXINFO exinfo;

	data = new char[length];
	if( !data )
	{
		//		dmsMsg( "Sound::loadStream2() error: Memory allocation error!\n" );
		return false;
	}

	memset( &exinfo, 0, sizeof( FMOD_CREATESOUNDEXINFO ) );
	exinfo.cbsize = sizeof( FMOD_CREATESOUNDEXINFO );
	exinfo.length = length;

	memcpy( data, memdata, length );

	result =  Sound::fmodSystem->createSound( data, FMOD_SOFTWARE | FMOD_OPENMEMORY | FMOD_2D, &exinfo, &stream );
	if( result != FMOD_OK )
	{
		//        dmsMsg( "Sound::init() error: FMOD error! (%d) %s\n", result, FMOD_ErrorString( result ) );
		return false;
	}

	stream->getLength( &this->length, FMOD_TIMEUNIT_MS );

	return true;
}

void Song::start()
{
	Sound::fmodSystem->playSound( FMOD_CHANNEL_FREE, stream, false, &m_channel );
	if( m_channel == NULL )
	{
		//		dmsMsg( "Song::start() warning: channel is NULL!\n" );
		return;
	}

	m_channel->setVolume(this->volume);

	setPaused( false );
	isPlaying = true;
}

void Song::stop()
{
	setPaused( true );
}

bool Song::checkPlaying()
{
	return isPlaying;
}

void Song::release()
{
	if(stream)
	{
		stream->release();
		stream = NULL;
	}

	if(data)
	{
		delete [] data;
		data = NULL;
	}

	stream = NULL;
	data = NULL;
}

void Song::setPosition(unsigned int offset)
{
	// Tss offsetti on millisekunteina
	if( m_channel )
	{
		m_channel->setPosition( offset, FMOD_TIMEUNIT_MS );
	}
}

void Song::changePosition(int delta)
{
	int time = this->getPosition();
	int ntime = time + delta;

	if (ntime < 0)
		ntime = 0;

	if (ntime >= 0 && ntime < getLength())
	{
		this->setPosition(ntime);
	}

}


void Song::setPaused(bool f)
{
	if( m_channel )
	{
		(f) ? m_channel->setPaused( true ) : m_channel->setPaused( false );
	}
}

int Song::getLength()
{
	return length;
}

int Song::getPosition()
{
	unsigned int time = 0;

	if( m_channel )
	{
		m_channel->getPosition( &time, FMOD_TIMEUNIT_MS );
	}

	return time;
}
void Song::getWaveData(float *array, int size)
{
	m_channel->getWaveData(array, size, 0);
}
void Song::getSpectrum(float *array, int size)
{
	m_channel->getSpectrum(array, size, 0,  FMOD_DSP_FFT_WINDOW_RECT);
}
