#pragma once

//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#include "../globals.h"

#ifdef _WIN32
	#include <windows.h>
#else
	#include <GL/glx.h>
	#include <X11/Xlib.h>
#endif

class GLWindow
{	
	friend class GLSystem;

public:

	GLWindow();
	~GLWindow();

	bool init();
	bool kill();

	bool createWindow();
	bool createWindow(int w, int h, int b, bool screen, bool onTop, int fsaa, int frequency);

#ifndef _WIN32
	bool createGLXWindow();
	void destroyGLXWindow();
#endif

	bool getFullscreen();
	bool getActive();
	unsigned int getWidth();
	unsigned int getHeight();

#ifdef _WIN32
	HWND getHandle();
	HINSTANCE getInstance();
	HDC getHDC();
#else
	Display* getDisplay();
	Window   getWindow();
#endif

	void setPerspective2D();
	void setPerspective2D(int w, int h);
	void setPerspective3D();
	void setWidth(unsigned int w);
	void setHeight(unsigned int h);
	void setBpp(unsigned int b);
	void setZbpp(unsigned int z);
	void setSbpp(unsigned int s);
	void setFullscreen(bool f);
	void setTitle(char *t);
	void setClassName(char *c);
	void setActive(bool a);
	void setWindowTitle(const string title);

#ifdef _WIN32
	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#else
	int  translateKey(unsigned int vk, bool* spKey);
	bool pollEvents();
	void toggleFullscreen();
#endif

	bool getKeyDown(int i);
	bool getKeyPressed(int i);

	bool  leftMouse;
	bool  rightMouse;
	float mouseX, mouseY;

private:

	//	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
	//	PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;

	unsigned int bpp, zbpp, sbpp;		// Bit Depths for buffers
	unsigned int width, height;			// Window width and height
	bool fullscreen;                        	// Fullscreen flag
	char *title;                                // Window name
	char *className;                            // Class name for registeration

	int  fsaa;                      		// Fullscreenalias multisamples
	bool onTop;             			// Always-on-top
	bool active;
	bool verticalSyncFlag;                      // Flag for Vertical retrace
	int  verticalSync;				// Holds the value of vertical retrace (on/off)

	bool allowResize;                           // Un/resizable

	bool keysPressed[256];
	bool keysDown[256];

#ifdef _WIN32
	HWND hwnd;
	HINSTANCE hinstance;
	HDC hdc;
	HGLRC hrc;

	int fetchFSAAMode(HDC hdc, int suggestedFormat, PIXELFORMATDESCRIPTOR p, int requestedmultisamples);
#else
	Display* g_Display;
	Window g_Window;
	GLXContext g_Context;

	unsigned int g_major;// = 1;
	unsigned int g_minor;// = 0;
	int          g_flags;// = 0;

	Atom g_DeleteMessage;
	Atom g_StateMessage;
	Atom g_FullscreenMessage;

	void prepareGLXContext(unsigned int major, unsigned int minor, int flags);
	void setResizeMode();
	bool modifyState(bool add, Atom atom1, Atom atom2 = None);
	bool toggleState(Atom atom1, Atom atom2 = None);
#endif

	bool extensionExist(const char *extension);
};

//--------------------------------------------------------------------------------------------
//  OpenGL System class
//--------------------------------------------------------------------------------------------

class GLSystem
{
	friend class GLWindow;

public:

	GLSystem();
	~GLSystem();

	bool init(int w, int h, int aspectratio = 0);
	void resize(int w, int h);
	void resetViewport();
	void setNormalPerspective();

	// Setters
	void setFov(float fov);
	void setPerspective(float fov, float zNear, float zFar);
	void setClearColor(float r, float g, float b, float a);

	float getScreenAspectRatio();
	float getZNear();
	float getZFar();
	float getFov();
	int getWidth();
	int getHeight();
	int getAspectRatio();

private:

	float zNear, zFar, fov;

	int width, height;

	int aspectratio;
	float screenaspectratio;
};

