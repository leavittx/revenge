#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#define VC_LEANMEAN
#endif

#include "globals.h"

//scenes
#include "fx/heart.h"
#include "fx/metablob.h"
#include "fx/brain.h"

//#include "fx/tree.h"

System g_system;
ShaderHandler g_shaders;
TextureHandler g_textures;
Parameters g_params;
PostProcess g_postprocess;

Demo *g_demo; //bad, mmkay

#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow) {
#else
int main(void) {
#endif

	//first initialize system
	g_system.init();
	//and then the demo
	Demo *demo = new Demo("data/script.txt");

	//all ok?
	if (demo->initOk())
	{
		g_demo = demo;

		//initalize managers
		g_textures.init();
		g_shaders.init();

		g_textures.loadImages();
		g_system.loadMeshes();

		//initialize the actual demo
		demo->addScene("heart", new Heart());
		demo->addScene("metablob", new Metablob());
		demo->addScene("brain", new Brain());

//					demo->addScene("treescene", new TreeScene());

		//construct the timeline
		demo->addSceneToTimeline("heart",         0,   7000, 1000);
		demo->addSceneToTimeline("metablob",   7000,  14000, 2000);
		demo->addSceneToTimeline("brain",     14000, 260000, 3000);

//					demo->addSceneToTimeline("treescene", 0, 240500, 666);

		//init the demo and play
		g_textures.uploadImages();
		g_system.uploadMeshes();
		g_shaders.loadShaders();
		demo->initEffects();

		g_system.dumpState();
		demo->start();

		bool demorunning = true;
		while (demorunning)
		{
			//process keyboard input and adjust demo accordingly
			g_system.handleInput(demo);
			//update the system (sync etc)
			g_system.update();

			stringstream ss;
			ss << "time = " << g_system.getTime() << " fps = " << g_system.getFPS();
			g_system.setWindowTitle(ss.str());
//			g_system.setWindowTitle("BMT :: Demo");

			demorunning =
					g_system.pollEvents() &&
					g_system.demoRunning();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			demo->update();
			demo->draw();

			g_system.swapBuffers();
		}
		demo->stop();
		g_textures.dumpUnusedImages();
	}

	delete demo;
	return 0;
}
