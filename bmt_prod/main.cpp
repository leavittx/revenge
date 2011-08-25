#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#define VC_LEANMEAN
#endif

#include "globals.h"

//scenes
#include "fx/heart.h"
#include "fx/metablob.h"

#include "fx/brain.h"
#include "fx/ear.h"
#include "fx/pandazoom.h"
#include "fx/credits.h"

//#include "fx/tree.h"

System g_system;
ShaderHandler g_shaders;
TextureHandler g_textures;
FontHandler g_fonts;
Parameters g_params;
PostProcess g_postprocess;
PitchDetector g_pitch;

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
//		g_system.loadMeshes();
		g_fonts.loadFonts();

		//initialize the actual demo
		demo->addScene("heart",     new Heart());
		demo->addScene("metablob",  new Metablob());
		demo->addScene("brain",     new Brain());

		demo->addScene("ear",       new EarScene());
		demo->addScene("pandazoom", new PandaZoomScene());
		demo->addScene("credits",   new CreditsScene());

//					demo->addScene("treescene", new TreeScene());

		//construct the timeline
//		demo->addSceneToTimeline("heart",         0,   4470, 1000);
//		demo->addSceneToTimeline("metablob",   4470,  26180, 2000);
//		demo->addSceneToTimeline("brain",     26180,  62300, 3000);

		demo->addSceneToTimeline("pandazoom",     0, 260000, 2000);
//		demo->addSceneToTimeline("ear",			  0, 260000, 2000);

//		demo->addSceneToTimeline("credits",       0, 260000, 2000);


		//Наш примерный таймлайн
//   начало     конец  длител   музыка                рисуется
//	      0      4670    4670   mute                  хз
//     4670     26180   21510   beat			      сцена1
//    26180     62300   36120   пиу-пиу+beat1+beat2   сцена2
//    62300     69840    7540   пиу-пиу+beat1         сцена2'
//    69840     72320    2480   дзззз                 какой-то переход
//    72320     82100    9780   новая пищалка         сцена3
//    82100     90160    8060   она же+ещё эффект     сцена3'
//    90160    114389   24229   то же+бубен фигачит   сцена3''
//   114389    122159    7770   то же без пищ соло    хз
//   122159    146200   24041   фигачит мягко         сцена4
//   146200    170090   23890   фигачит космос        сцена5
//   170090    182370   12280   трааанс               сцена6
//   182370    246258   63888   транс+дисторшн        сцена7
//   246258    260000   13742   просто дисторшн       титры


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
			//update fft data
			g_pitch.update();

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
