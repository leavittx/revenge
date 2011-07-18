//#include <tchar.h>
#include <algorithm>
#include "system.h"

System::System()
{
	m_glWindow = 0;
	m_glSystem = 0;
	m_song = 0;
	m_audio = 0;
	m_loadingScreenSteps = 0;
	m_loadingScreenPosition = 0;
	m_frustumCull = 0;
	m_soundEnabled = true;
	m_dialogSoundEnabled = true;
}
System::~System()
{
	if (m_glWindow)
	{
		delete m_glWindow;
	}
	if (m_glSystem)
	{
		delete m_glSystem;
	}
	if (m_song)
	{
		delete m_song;
	}
	if (m_audio)
	{
		delete m_audio;
	}

//	clearImages();

}

void System::init()
{
//	m_textureParameters.clear();
	m_meshInfos.clear();
	m_glWindow = new GLWindow();
	m_glSystem = new GLSystem();
	m_beats.clear();
//	m_textures.clear();
//	m_images.clear();
//	m_shaders.clear();
	m_cameras.clear();
	m_loadingScreenSteps = 0;
	m_loadingScreenPosition = 0;

	m_frameCount = 0;
	m_frameTimer = 0;
	m_framePreviousTime = 0;
	m_FPS = 0.0f;

	m_frustumCull = new FrustumCull();
	m_timer = new Timer();
	setEndTime(100);
}

void System::clear()
{
	m_beats.clear();
	m_triggers.clear();
	m_events.clear();
	m_cameras.clear();
/*
	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		m_lastBoundTexture[i] = "no texture";
	}
*/
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Windowing and general stuff
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void System::kill()
{
    m_glWindow->kill();
}
float System::getAspectRatio()
{
	return m_glSystem->getScreenAspectRatio();
}
bool System::createWindow(Config &cfg)
{
	int xres = cfg.getScreenX();
	int yres = cfg.getScreenY();
    if(!m_glWindow->createWindow(xres, yres, 32, cfg.getFullscreen(), cfg.getOnTop(), 
        cfg.getFsaa(), cfg.getFrequency()))
    {
        g_debug << "ERROR! could not create window!\n" << endl;
		return false;
    }
	return true;
}
bool System::getRandomized()
{
	return m_randomized;
}
bool System::createSystem(Config &cfg)
{
    if (!m_glSystem->init(cfg.getScreenX(), cfg.getScreenY(), cfg.getAspectRatio()))
	{
		g_debug << "ERROR! could not initialize OpenGL!\n" << endl;
		return false;
	}
	m_dialogSoundEnabled = cfg.getSound();
	m_randomized = cfg.getRandomized();
	g_debug << "getSound = " << cfg.getSound() << "\n";
	g_debug << "m_dialogSoundEnabled " << m_dialogSoundEnabled << "\n";

	return true;
}

bool System::demoRunning()
{
	//running if not pressed esc or the song hasn't ended
#ifdef _WIN32
	return !getKeyDown(VK_ESCAPE) && (getTime() < (m_song->getLength() - m_endTime));
#else
    //todo
    return (getTime() < (m_song->getLength() - m_endTime));
#endif
}

void System::setEndTime(int time)
{
	m_endTime = time;
}

bool System::getKeyDown(int i)
{
	return m_glWindow->getKeyDown(i);
}
bool System::getKeyPressed(int i)
{
	return m_glWindow->getKeyPressed(i);
}
GLWindow& System::getGLWindow()
{
	return *m_glWindow;
}
GLSystem& System::getGLSystem()
{
	return *m_glSystem;
}

FrustumCull& System::getFrustum()
{
	return *m_frustumCull;
}

bool System::initOpenGL(Config &cfg)
{
    g_debug << "sound loaded, creating window" << endl;
	if (!createWindow(cfg))
	{
        g_debug << "ERROR! could not create window!\n" << endl;
		return false;
	}
    g_debug << "window created, starting up openGL" << endl;

    // Init opengl system
	if (!createSystem(cfg))
	{
		return false;
	}
    g_debug << "openGL started, starting up GLEW" << endl;
	//init GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		g_debug << "cannot initialize GLEW!" << endl;
	}
	return true;
}

void System::handleInput(Demo *demo)
{
    //fast forward/rewind
#ifdef _WIN32
    bool slowdown = GetAsyncKeyState(VK_RSHIFT) || GetAsyncKeyState(VK_LSHIFT);
    const int adjust = slowdown ? 2 : 1000;

	if (getKeyDown(VK_LEFT))
    {
        addTime(-adjust);
    }
    if (getKeyDown(VK_RIGHT))
    {
        addTime(adjust);
    }
    //pause/resume
	if (getKeyPressed(VK_SPACE))
    {
        demo->toggleRunning();
    }
#else
    //todo
#endif
}

void System::setWindowTitle(const string title)
{
#ifdef _WIN32
	m_glWindow->setWindowTitle(title);
#endif
        //todo
}
void System::resetViewport()
{
	glViewport(0, 0, m_glWindow->getWidth(), m_glWindow->getHeight());
}
void System::swapBuffers()
{
#ifdef _WIN32
	SwapBuffers(m_glWindow->getHDC());
#endif
        //todo
}

void System::update()
{
	m_timer->update(getAudioPosition());
//	g_debug << "m_timer->getTime() = " << m_timer->getTime() << ", getsongposition = " << getAudioPosition() << "\n";
	updateSync();
	updateFPS();
}

void System::setOrthoMode(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	glOrtho(0,width, 0,height,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void System::setPerspectiveMode()
{
	m_glSystem->setNormalPerspective();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//										    FPS counter
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void System::updateFPS()
{
	int currentTime = getTime();//SongPosition();
	int dt = currentTime - m_framePreviousTime;
	m_framePreviousTime = currentTime;

	if (dt < 0)
		dt = -dt;

	m_frameCount++;
	m_frameTimer += dt;

	//update per second
	if (m_frameTimer > 1000)
	{
		m_FPS = 1000.0f * (m_frameCount / (float)m_frameTimer);
		m_frameCount = 0;
		m_frameTimer = 0;
	}

}
float System::getFPS()
{
	return m_FPS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//										    Loading screen
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void System::addLoadingScreenSteps(int count)
{
	m_loadingScreenSteps += count;
}

void System::advanceLoadingScreen(int steps)
{
	m_loadingScreenPosition += steps;
}

float radiusmod(float a, float t)
{
	return 0.7f + 0.3f * sinf(a * 5 - t * 9);
}
void System::drawLoadingScreen()
{
	if (m_loadingScreenSteps > 0)
	{
		const float percentage = m_loadingScreenPosition / (float)m_loadingScreenSteps;
		const int x = 1024 / 2;
		const int y = 768 / 2;
		const int THICKNESS = 10;
		const int WIDTH = 200;
		const int DISTANCE = 4;
		
		int startX = x - WIDTH/2;
		int endX = startX + (int)(WIDTH*percentage);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		setOrthoMode(1024, 768);

		glDepthMask(GL_FALSE);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		const float alpha = 1.0f - powf(percentage, 3);

		//draw borders
		glColor4f(0.4f,0.4f,0.4f,alpha);
		glBegin(GL_LINE_LOOP);
		glVertex2i(x - WIDTH/2 - DISTANCE, y - THICKNESS/2 - DISTANCE);
		glVertex2i(x + WIDTH/2 + DISTANCE, y - THICKNESS/2 - DISTANCE);
		glVertex2i(x + WIDTH/2 + DISTANCE, y + THICKNESS/2 + DISTANCE);
		glVertex2i(x - WIDTH/2 - DISTANCE, y + THICKNESS/2 + DISTANCE);
		glEnd();

		//draw bar
		glColor4f(1,0.4f,1.0f,alpha);
		glBegin(GL_QUADS);
		glVertex2i(startX, y - THICKNESS/2);
		glVertex2i(endX, y - THICKNESS/2);
		glVertex2i(endX, y + THICKNESS/2);
		glVertex2i(startX, y + THICKNESS/2);
		glEnd();
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);

		setPerspectiveMode();
		glFlush();
		swapBuffers();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//												Audio
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void System::setSongFilename(string filename)
{
	m_songFile = filename;
}
string System::getSongFilename()
{
	return m_songFile;
}

void System::setSoundEnabled(bool enabled)
{
	m_soundEnabled = enabled && m_dialogSoundEnabled;

	if (m_audio)
	{
		m_audio->setVolume(m_song, 1.0f); //actual toggle is done through volume settings
	}
}
bool System::getSoundEnabled()
{
	return m_soundEnabled && m_dialogSoundEnabled;
}
int System::getAudioPosition()
{
	return m_song->getPosition();
}

int System::getTime()
{
	return m_timer->getTime();
}

bool System::initSound(const string& filename)
{
    m_audio = Sound::create();
    m_audio->init();
    m_song = new Song();
    m_song->loadStream((char*)filename.c_str());
    m_audio->setEnabled(m_soundEnabled && m_dialogSoundEnabled); //hmmh
	m_audio->setVolume(m_song, 1.0f); //turns off the sound if necessary
	return true;
}

void System::freeSound()
{
	stopSong();
    m_song->release();
    m_audio->close();
    m_audio = 0;
    m_song = 0;
}

void System::startSong()
{
	m_song->start();
	m_timer->init();
}

void System::stopSong()
{
	m_song->stop();
	m_timer->stop();
}
void System::pauseSong(bool pause)
{
	m_song->setPaused(pause);
	m_timer->setPaused(pause);
}

void System::addTime(int adjust)
{
	m_song->changePosition(adjust);
	m_timer->addTime(adjust);
}

void System::getWaveData(float *array, int size)
{
	m_song->getWaveData(array, size);
	array[size-1] = 0; //linux fix
}
void System::getSpectrum(float *array, int size)
{
	m_song->getSpectrum(array, size);
	array[size-1] = 0; //linux fix
}

float System::getSpectrumSum(int start, int end)
{
	float fft[512];
	float sum = 0.0f;
	m_song->getSpectrum(fft, 512);

	end = min(end, 511); //linux fix
	for (int i = 0; i < end; i++)
	{
		sum += fft[i];
	}
	return sum;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//												Textures
//////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//											   Shaders
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//											    Sync
//////////////////////////////////////////////////////////////////////////////////////////////////////////


void System::updateSync()
{
	//update BPM counters
	map<string, BPM*>::iterator it;
	for (it = m_beats.begin(); it != m_beats.end(); it++)
	{
		BPM *beat = (*it).second;
		beat->update();
	}
}
void System::addBeat(string name, BPM *bpm)
{
	m_beats[name] = bpm;
}
BPM &System::beat(string name)
{
	return *m_beats[name];
}

void System::addTriggers(string name, TriggerSystem *triggers)
{
 	m_triggers[name] = triggers;
}
TriggerSystem& System::triggers(string name)
{
	if (m_triggers.find(name) == m_triggers.end())
	{
		g_debug << "Cannot find triggers " << name << "!" << endl;
	}
	return *m_triggers[name];
}
void System::addEvent(string name, Event *e)
{
	m_events[name] = e;
}
Event& System::event(string name)
{
	if (m_events.find(name) == m_events.end())
	{
		g_debug << "Cannot find event" << name << "!" << endl;
	}
	return *m_events[name];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//											    Sync
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void System::addCamera(string name, Camera *c)
{
	m_cameras[name] = c;
}
Camera& System::camera(string name)
{
	if (m_cameras.find(name) == m_cameras.end())
	{
		g_debug << "Cannot find camera" << name << "!" << endl;
	}
	return *m_cameras[name];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//											  Meshes
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void System::addMeshInfo(string name, MeshInfo* info)
{
	m_meshInfos[name] = info;
}

void System::loadMeshes()
{
	vector<string> filenames;
	string directory = "data\\meshes\\";
	StringUtils::iterateDirectory(directory, filenames);

	//add steps, one for uploading and one for loading
	addLoadingScreenSteps(filenames.size() * 2);

	vector<string>::iterator it;
	for (it = filenames.begin(); it < filenames.end(); it++)
	{
		string filename = *it;
		string path = directory + filename;
		string suffix = filename.substr(filename.length()-3, filename.length());

		if (suffix == "obj")
		{
			Mesh *mesh = MeshFactory::loadOBJ(path);
			addMesh(filename, mesh);
			advanceLoadingScreen(1);
		}
		else if (suffix == "msh")
		{
			Mesh *mesh = MeshFactory::loadMSH(path);
			addMesh(filename, mesh);
			advanceLoadingScreen(1);
		}
		else if (suffix == "mtl")
		{
			Material *material = MaterialFactory::loadMTL(path);
			addMaterial(filename, material);
			advanceLoadingScreen(1);
		}
		else
		{
			g_debug << "non-mesh or material file " << filename << " found in graphics directory!" << endl;
		}
		drawLoadingScreen();
	}
}

void System::uploadMeshes()
{
}

void System::addMesh(string name, Mesh *mesh)
{
	m_meshes[name] = mesh;
}
void System::addMaterial(string name, Material *material)
{
	m_materials[name] = material;
}
Mesh& System::mesh(string name)
{
	if (m_meshes.find(name) == m_meshes.end())
	{
		g_debug << "Cannot find mesh " << name << "!" << endl;
	}
	return *m_meshes[name];
}
Material& System::material(string name)
{
	if (m_materials.find(name) == m_materials.end())
	{
		g_debug << "Cannot find material" << name << "!" << endl;
	}
	return *m_materials[name];
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//											  Utilities
//////////////////////////////////////////////////////////////////////////////////////////////////////////


void System::glDebug(string text)
{
	GLenum error = glGetError();
	switch(error)
	{
		case GL_NO_ERROR:
//			g_debug << text << ": OK!" << endl;
			break;
		
		case GL_INVALID_ENUM:
			g_debug << "OpenGL error! " << text << " : GL_INVALID_ENUM" << endl;
			break;

		case GL_INVALID_VALUE:
			g_debug << "OpenGL error! " << text << " : GL_INVALID_VALUE" << endl;
			break;

		case GL_INVALID_OPERATION:
			g_debug << "OpenGL error! " << text << " : GL_INVALID_OPERATION" << endl;
			break;

		case GL_STACK_OVERFLOW:
			g_debug << "OpenGL error! " << text << " : GL_STACK_OVERFLOW" << endl;
			break;

		case GL_STACK_UNDERFLOW:
			g_debug << "OpenGL error! " << text << " : GL_STACK_UNDERFLOW" << endl;
			break;

		case GL_OUT_OF_MEMORY:
			g_debug << "OpenGL error! " << text << " : GL_OUT_OF_MEMORY" << endl;
			break;
	}
}


void System::dumpState()
{
/*
	//TODO: timeline
	map<string, Image*>::iterator imageIterator;
//	map<string, Shader*>::iterator shaderIterator;

	g_debug << "" << endl;
	g_debug << "System::dumpState()" << endl;
	g_debug << "-------------------" << endl;

	//print out image information
	g_debug << "Images:" << endl;
	g_debug << "-------" << endl;
	for (imageIterator = m_images.begin(); imageIterator != m_images.end(); imageIterator++)
	{
		Image &i = *(*imageIterator).second;
		const string& name = (*imageIterator).first;

		Texture &t = *m_textures[name];
		TextureParameters &p = *m_textureParameters[name];

		g_debug << "  name = " << name << endl;
		if (p.m_upload)
		{
			g_debug << "    OpenGL ID = " << t.getID() << endl;
		}
		else
		{
			g_debug << "    Not uploaded to OpenGL" << endl;
		}
		g_debug << "    resolution = " << i.getWidth() << "x" << i.getHeight() << endl;
		g_debug << "    filter = " << (p.m_linear ? "linear" : "nearest") << endl;
		g_debug << "    retain = " << (p.m_retain ? "true" : "false") << endl;
		g_debug << "" << endl;
	}
*/
/*
	//print out shaders
	g_debug << "Shaders:" << endl;
	g_debug << "--------" << endl;
	for (shaderIterator = m_shaders.begin(); shaderIterator != m_shaders.end(); shaderIterator++)
	{
		Shader &s = *(*shaderIterator).second;
		const string &name = (*shaderIterator).first;
		g_debug << "  name = " << name << endl;
//		g_debug << "" << endl;
	}
	g_debug << "" << endl;
*/
}

