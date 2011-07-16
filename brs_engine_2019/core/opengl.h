#pragma once

//--------------------------------------------------------------------------------------------
//  Headers
//--------------------------------------------------------------------------------------------

#ifdef _WIN32
#include <windows.h>
#endif

#include "../globals.h"

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

    bool getFullscreen();
    bool getActive();
    unsigned int getWidth();
    unsigned int getHeight();

#if defined(_WIN32)
    HWND getHandle();
    HINSTANCE getInstance();
    HDC getHDC();
#endif /* defined(_WIN32) */

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

#if defined(_WIN32)
    friend LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif /* defined(_WIN32) */

    bool getKeyDown(int i);
    bool getKeyPressed(int i);

    bool	leftMouse;
    bool	rightMouse;
    float	mouseX, mouseY;


private:

    //	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
    //	PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;

    unsigned int bpp, zbpp, sbpp;		// Bit Depths for buffers
    unsigned int width, height;			// Window width and height
    bool fullscreen;					// Fullscreen flag
    char *title;						// Window name
    char *className;					// Class name for registeration

    int fsaa;							// Fullscreenalias multisamples
    bool onTop;							// Always-on-top
    bool active;
    bool verticalSyncFlag;				// Flag for Vertical retrace
    int verticalSync;					// Holds the value of vertical retrace (on/off)

    bool keysPressed[256];
    bool keysDown[256];

#if defined(_WIN32)
    HWND hwnd;
    HINSTANCE hinstance;
    HDC hdc;
    HGLRC hrc;

    int fetchFSAAMode(HDC hdc,int suggestedFormat, PIXELFORMATDESCRIPTOR p, int requestedmultisamples);
#endif /* defined(_WIN32) */
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

    bool init(int w, int h, int aspectratio=0);
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

