#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#define VC_LEANMEAN
#endif

#include "globals.h"

//scenes
//#include "fx/start.h"

#include "fx/tree.h"

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
        //demo->addScene("background11", new Background11());
        //demo->addScene("disco", new Disco());

        demo->addScene("treescene", new TreeScene());

        /*
  demo->addScene("start", new Start());
  demo->addScene("underground", new Underground());
  demo->addScene("freefall", new Freefall());
  demo->addScene("lucifer", new Lucifer());
  demo->addScene("magellan", new Magellan());
  demo->addScene("kohina", new Kohina());
*/
#define COMPLETE_DEMO
#ifdef COMPLETE_DEMO

        //construct the timeline
        demo->addSceneToTimeline("treescene", 0, 240500, 666);
        /*
  demo->addSceneToTimeline("start", 0, 55000, 5000);
  demo->addSceneToTimeline("underground", 55000, 97000, 6000);
  demo->addSceneToTimeline("freefall", 90000, 130000, 6000);
  demo->addSceneToTimeline("lucifer", 120000, 190000, 4000);
  demo->addSceneToTimeline("magellan", 180000, 243000, 7000);
  demo->addSceneToTimeline("start", 243000, 296000, 7000);

*/
        //		demo->addSceneToTimeline("kohina", 0, 275000, 666*666);
        //		demo->addSceneToTimeline("disco", 68000, 96000, 5000); //disko
#else
#endif
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

            //			stringstream ss;
            //			ss << "time = " << g_system.getTime() << " fps = " << g_system.getFPS();
            //			g_system.setWindowTitle(ss.str());
            g_system.setWindowTitle("Brainstorm :: 2019");

//#ifdef _WIN32

            demorunning =
                g_system.pollEvents() &&
                g_system.demoRunning();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            demo->update();
            demo->draw();

            g_system.swapBuffers();
//#else

//            glWindow.glusRun();
//#endif
        }
        demo->stop();
        g_textures.dumpUnusedImages();
    }

    delete demo;

//#ifdef _WIN32
//    return msg.wParam;
//#else
    return 0;
//#endif
}
