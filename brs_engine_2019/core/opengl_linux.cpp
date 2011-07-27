#include "opengl.h"
#include "../globals.h"

#include <time.h>

#include <stdio.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/X.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#ifndef GLX_CONTEXT_DEBUG_BIT_ARB
#define GLX_CONTEXT_DEBUG_BIT_ARB 0x0001
#endif
#ifndef GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#define GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#endif
#ifndef GLX_CONTEXT_MAJOR_VERSION_ARB
#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#endif
#ifndef GLX_CONTEXT_MINOR_VERSION_ARB
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092
#endif
#ifndef GLX_CONTEXT_FLAGS_ARB
#define GLX_CONTEXT_FLAGS_ARB 0x2094
#endif

typedef GLXContext (* PFNGLXCREATECONTEXTATTRIBSARBPROCTEMP)(Display* dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list);

static unsigned int g_major = 1;
static unsigned int g_minor = 0;
static int          g_flags = 0;

static Atom g_DeleteMessage;
static Atom g_StateMessage;
static Atom g_FullscreenMessage;

/**
  OpenGL window class
  */

GLWindow::GLWindow()
{
    g_Display = None;
    g_Window = 0;
    g_Context = None;

    // Default setup
    width = 640;
    height = 480;
    bpp = 32;
    zbpp = 16;
    sbpp = 8;
    fullscreen = false;
    active = true;
    title = "Brainstorm :: 2019";
    className = "DemoGL";
    onTop = false;
    fsaa = 0; //TODO - 2 ???

    verticalSyncFlag = false;
    verticalSync = 0;

    for (int i = 0; i < 256; i++)
    {
        keysDown[i] = false;
        keysPressed[i] = false;
    }
}

GLWindow::~GLWindow()
{
    kill();
}

void GLWindow::prepareGLXContext(unsigned int major, unsigned int minor, int flags)
{
    g_major = major >= 1 ? major : 0;
    g_minor = minor;
    g_flags = flags;
}

bool GLWindow::createGLXWindow()
{
    Window RootWindow = 0;
    XVisualInfo* VisualInfo = None;
    Colormap CurrentColorMap = 0;
    XSetWindowAttributes CurrentSetWindowAttibutes;
    int fbcCount = 0;
    GLXFBConfig* fbc = None;

    int visualAttribList[] = {
        GLX_X_RENDERABLE, True, GLX_DRAWABLE_TYPE,
        GLX_WINDOW_BIT, GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8, GLX_DEPTH_SIZE, 16, GLX_STENCIL_SIZE, 8,
        GLX_DOUBLEBUFFER, True, None };

    g_Display = XOpenDisplay(None);

    if (g_Display == None)
    {
        return false;
    }

    RootWindow = DefaultRootWindow(g_Display);

    if (!(fbc = glXChooseFBConfig(g_Display, DefaultScreen(g_Display), visualAttribList, &fbcCount)))
    {
        destroyGLXWindow();
        return false;
    }

    VisualInfo = glXGetVisualFromFBConfig(g_Display, fbc[0]);

    if (VisualInfo == None)
    {
        XFree(fbc);
        destroyGLXWindow();
        return false;
    }

    CurrentColorMap = XCreateColormap(g_Display, RootWindow, VisualInfo->visual, AllocNone);

    CurrentSetWindowAttibutes.colormap = CurrentColorMap;
    CurrentSetWindowAttibutes.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask;

    g_Window = XCreateWindow(g_Display, RootWindow, 0, 0, width, height, 0, VisualInfo->depth, InputOutput, VisualInfo->visual, CWColormap | CWEventMask, &CurrentSetWindowAttibutes);

    if (g_Window == 0)
    {
        XFree(fbc);
        destroyGLXWindow();
        return false;
    }

    g_DeleteMessage = XInternAtom(g_Display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(g_Display, g_Window, &g_DeleteMessage, 1);

    XMapWindow(g_Display, g_Window);

    XStoreName(g_Display, g_Window, title);

    if (fullscreen)
    {
        XEvent xev;

        Cursor InvisibleCursor = None;
        char cursorData[32];
        XColor cursorColor;
        Pixmap cursorPixmap;

        memset(cursorData, 0, sizeof(cursorData));

        memset(&cursorColor, 0, sizeof(cursorColor));

        cursorPixmap = XCreateBitmapFromData(g_Display, RootWindow, cursorData, 16, 16);

        if (cursorPixmap != None)
        {
            InvisibleCursor = XCreatePixmapCursor(g_Display, cursorPixmap, cursorPixmap, &cursorColor, &cursorColor, 0, 0);

            XFreePixmap(g_Display, cursorPixmap);

            if (InvisibleCursor != None)
            {
                XDefineCursor(g_Display, g_Window, InvisibleCursor);
            }
        }

        g_StateMessage = XInternAtom(g_Display, "_NET_WM_STATE", False);
        g_FullscreenMessage = XInternAtom(g_Display, "_NET_WM_STATE_FULLSCREEN", False);

        memset(&xev, 0, sizeof(xev));

        xev.xclient.type = ClientMessage;
        xev.xclient.window = g_Window;
        xev.xclient.message_type = g_StateMessage;
        xev.xclient.format = 32;
        xev.xclient.data.l[0] = 1;
        xev.xclient.data.l[1] = g_FullscreenMessage;

        XSendEvent(g_Display, RootWindow, False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
    }

    g_Context = glXCreateContext(g_Display, VisualInfo, None, true);

    if (g_Context == None)
    {
        XFree(fbc);
        destroyGLXWindow();
        return false;
    }

    if (!glXMakeCurrent(g_Display, g_Window, g_Context))
    {
        XFree(fbc);
        destroyGLXWindow();
        return false;
    }

    if (g_major >= 3)
    {
        GLXContext Context = None;
        PFNGLXCREATECONTEXTATTRIBSARBPROCTEMP glXCreateContextAttribsARBTemp = None;

        int attribList[] = { GLX_CONTEXT_MAJOR_VERSION_ARB, 1, GLX_CONTEXT_MINOR_VERSION_ARB, 0, GLX_CONTEXT_FLAGS_ARB, 0, 0 };

        attribList[1] = g_major;
        attribList[3] = g_minor;
        attribList[5] = g_flags;

        if (!(glXCreateContextAttribsARBTemp = (PFNGLXCREATECONTEXTATTRIBSARBPROCTEMP) glXGetProcAddress((const GLUSubyte *) "glXCreateContextAttribsARB")))
        {
            XFree(fbc);
            destroyGLXWindow();
            return false;
        }

        if (!(Context = glXCreateContextAttribsARBTemp(g_Display, fbc[0], 0, True, attribList)))
        {
            XFree(fbc);
            destroyGLXWindow();
            return false;
        }

        XFree(fbc);

        if (!glXMakeCurrent(g_Display, None, None))
        {
            glXDestroyContext(g_Display, Context);
            destroyGLXWindow();
            return false;
        }

        glXDestroyContext(g_Display, g_Context);

        g_Context = Context;

        if (!glXMakeCurrent(g_Display, g_Window, g_Context))
        {
            destroyGLXWindow();
            return false;
        }
    }

    return true;
}

void GLWindow::destroyGLXWindow()
{
    // Are We In Fullscreen Mode?
    if (fullscreen && g_Display && g_Window)
    {
        XEvent xev;

        memset(&xev, 0, sizeof(xev));

        XUndefineCursor(g_Display, g_Window);

        xev.xclient.type = ClientMessage;
        xev.xclient.window = g_Window;
        xev.xclient.message_type = g_StateMessage;
        xev.xclient.format = 32;
        xev.xclient.data.l[0] = 0;
        xev.xclient.data.l[1] = g_FullscreenMessage;

        XSendEvent(g_Display, DefaultRootWindow(g_Display), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
    }

    if (g_Display)
    {
        glXMakeCurrent(g_Display, None, None);
    }

    if (g_Display && g_Context)
    {
        glXDestroyContext(g_Display, g_Context);
        g_Context = None;
    }

    if (g_Display && g_Window)
    {
        XDestroyWindow(g_Display, g_Window);
    }

    if (g_Display)
    {
        XCloseDisplay(g_Display);
        g_Display = None;
    }
}

//TODO: add translateKey on all platforms (look into frontend)
int GLWindow::translateKey(unsigned int vk, bool* spKey)
{
    *spKey = true; //special key

    switch (vk)
    {
    case XK_Tab:                    return KeyTab; break;
    case XK_Return:                 return KeyEnter; break;
    case XK_space: *spKey = false;  return KeySpace; break;
    case XK_Menu:                   return KeyAlt; break;
    case XK_Shift_L:
    case XK_Shift_R:                return KeyShift; break;
    case XK_Control_L:
    case XK_Control_R:              return KeyCtrl; break;
    case XK_Caps_Lock:              return KeyCapsLock; break;
    case XK_Up:                     return KeyUpArrow; break;
    case XK_Down:                   return KeyDownArrow; break;
    case XK_Left:                   return KeyLeftArrow; break;
    case XK_Right:                  return KeyRightArrow; break;
    case XK_BackSpace:              return KeyBackspace; break;
    case XK_F1:                     return KeyF1; break;
    case XK_F2:                     return KeyF2; break;
    case XK_F3:                     return KeyF3; break;
    case XK_F4:                     return KeyF4; break;
    case XK_F5:                     return KeyF5; break;
    case XK_F6:                     return KeyF6; break;
    case XK_F7:                     return KeyF7; break;
    case XK_F8:                     return KeyF8; break;
    case XK_F9:                     return KeyF9; break;
    case XK_F10:                    return KeyF10; break;
    case XK_F11:                    return KeyF11; break;
    case XK_F12:                    return KeyF12; break;
    case XK_F13:                    return KeyF13; break;
    case XK_F14:                    return KeyF14; break;
    case XK_F15:                    return KeyF15; break;
    case XK_F16:                    return KeyF16; break;
    case XK_Prior:                  return KeyPageUp; break;
    case XK_Next:                   return KeyPageDown; break;
    case XK_Home:                   return KeyHome; break;
    case XK_End:                    return KeyEnd; break;
    case XK_Insert:                 return KeyInsert; break;
    case XK_Delete:                 return KeyDelete; break;
    case XK_Escape:                 return KeyEsc; break;
    case 0xFE03:                    return 0; break; // ALT GR
    case XK_s:
    case XK_S:                      return KeyS; //for sound toggle
    default: *spKey = false;        return vk; break;
    }
}

bool GLWindow::pollEvents(void)
{
    XEvent msg;
    bool spKey; //TODO: remove or handle

    if (XPending(g_Display))
    {
        XNextEvent(g_Display, &msg);

        if (msg.type == KeyPress)
        {
            for (int i = 0; i < 256; i++)
            {
                keysPressed[i] = false;
            }
            // Have We Received the Esc Key
//            if (msg.xkey.keycode == 0x09)
//            {
                // do nothing
//            }
//            else
//            {
//                char c;

//                if (XLookupString(&msg.xkey, &c, 1, 0, 0) > 0)
//                {
//                    do smth...
//                }

            keysPressed[translateKey(XLookupKeysym(&msg.xkey, 0), &spKey)] = true;
            keysDown[translateKey(XLookupKeysym(&msg.xkey, 0), &spKey)] = true;
//            }
        }
        else if (msg.type == KeyRelease)
        {
            // Have We Received the Esc Key
//            if (msg.xkey.keycode == 0x09)
//            {
//                return false;
//            }
//            else
//            {
//                char c;
//                if (XLookupString(&msg.xkey, &c, 1, 0, 0) > 0)
//                {
//                    do smth...
//                }
            keysDown[translateKey(XLookupKeysym(&msg.xkey, 0), &spKey)] = false;
//            }
        }
        else if (msg.type == ClientMessage)
        {
            // Have We Received a Delete Message
            if ((unsigned)msg.xclient.data.l[0] == g_DeleteMessage)
            {
                return false;
            }
        }
        else if (msg.type == ConfigureNotify)
        {
            width = msg.xconfigure.width;
            height = msg.xconfigure.height;

            glViewport(0, 0, width, height);

            //do reshape here (not needed)
        }
        else if (msg.type == MapNotify)
        {
            active = true;
        }
        else if (msg.type == UnmapNotify)
        {
            active = false;
        }
    }

    return true;
}

//TODO
bool GLWindow::createWindow()
{
    return true;
}

// width, height, bpp, fullscreen, alwaysontop, fsaa-samples (0,2,4,6,8), freq
bool GLWindow::createWindow(int w, int h, int b, bool screen, bool onTop, int fsaa, int frequency)
{
    width = w;
    height = h;
    bpp = b;
    fullscreen = screen;
    this->onTop = onTop;

    prepareGLXContext(2, 1, GLUS_BACKWARD_COMPATIBLE_BIT); //GLUS_FORWARD_COMPATIBLE_BIT

    if (!createGLXWindow())
    {
        g_debug << "Could not create window!" << endl;
        return false;
    }

    // Init GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Only continue, if OpenGL 3.3 is supported.
    if (!glewIsSupported("GL_VERSION_3_3"))
    {
        g_debug << "OpenGL 3.3 not supported." << endl;

        destroyGLXWindow();
        return false;
    }

    return true;
}

bool GLWindow::kill()
{
    destroyGLXWindow();

    return true;
}

bool GLWindow::getFullscreen()
{
    return fullscreen;
}

bool GLWindow::getActive()
{
    return active;
}

//	Getterit
unsigned int GLWindow::getWidth()
{
    return width;
}

unsigned int GLWindow::getHeight()
{
    return height;
}

Display* GLWindow::getDisplay()
{
    return g_Display;
}

Window GLWindow::getWindow()
{
    return g_Window;
}

// Setterit
void GLWindow::setWidth(unsigned int w)
{
    width = w;
}

void GLWindow::setHeight(unsigned int h)
{
    height = h;
}

void GLWindow::setBpp(unsigned int b)
{
    bpp = b;
}

void GLWindow::setZbpp(unsigned int z)
{
    zbpp = z;
}

void GLWindow::setSbpp(unsigned int s)
{
    sbpp = s;
}

void GLWindow::setFullscreen(bool f)
{
    fullscreen = f;
}

void GLWindow::setActive(bool a)
{
    active = a;
}

void GLWindow::setTitle(char *t)
{
    title = t;
}

void GLWindow::setClassName(char *c)
{
    className = c;
}

void GLWindow::setPerspective2D(void)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,640, 0,480,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void GLWindow::setPerspective2D(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,w,0,h,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void GLWindow::setPerspective3D(void)
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

//TODO
bool GLWindow::extensionExist(const char *extension)
{
    return true;
}

bool GLWindow::getKeyDown(int i)
{
    bool ret = keysDown[i];
    keysDown[i] = false;
    return ret;
}

bool GLWindow::getKeyPressed(int i)
{
    bool ret = keysPressed[i];
    keysPressed[i] = false;
    return ret;
}

void GLWindow::setWindowTitle(const string title)
{
    XStoreName(g_Display, g_Window, title.c_str());
    XFlush(g_Display);
}

/**
  OpenGL system class
  */

GLSystem::GLSystem()
{
    width = 640;
    height = 480;
    aspectratio = 0;

    fov = 45.0f;
    zNear = 0.1f;
    zFar = 14000.0f;
}

GLSystem::~GLSystem()
{
}

void GLSystem::setNormalPerspective()
{
    float aspect = (float)width/(float)height;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

bool GLSystem::init(int w, int h, int aspectratio)
{
    resize(w, h);

    width = w;
    height = h;

    this->aspectratio = aspectratio;

    if(height == 0) height = 1;

    if(aspectratio==1)			// 16:10
        h = int(w*0.625f);
    else if(aspectratio==2)		// 16:9
        h = int(w*0.5625f);

    glViewport(0, (height-h)/2, w, h);
    setNormalPerspective();

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glDisable(GL_NORMALIZE);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_2D);

    return true;
}

float GLSystem::getScreenAspectRatio()
{
    return screenaspectratio;
}

void GLSystem::resize(int w, int h)
{

    width = w;
    height = h;

    if(height == 0) height = 1;

    if(aspectratio==1)			// 16:10
        h = int(w*0.625f);
    else if(aspectratio==2)		// 16:9
        h = int(w*0.5625f);

    float aspect = (float)w/(float)h;
    screenaspectratio = aspect;

    glViewport(0, (height-h)/2, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(45.0f, aspect, 0.1f, 14000.0f);
    gluPerspective(fov, aspect, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLSystem::resetViewport()
{
    int h = height;
    int w = width;

    if(this->aspectratio==1)			// 16:10
        h = int(w*0.625f);
    else if(this->aspectratio==2)		// 16:9
        h = int(w*0.5625f);

    glViewport(0, (height-h)/2, w, h);
}

void GLSystem::setClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void GLSystem::setFov(float fov)
{
    this->fov = fov;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(fov, (float)width/(float)height, 0.5f, 14000.0f);
    gluPerspective(fov, (float)width/(float)height, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLSystem::setPerspective(float fov, float zNear, float zFar)
{
    this->fov = fov;
    this->zNear = zNear;
    this->zFar = zFar;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(fov, (float)width/(float)height, 0.5f, 14000.0f);
    gluPerspective(fov, (float)width/(float)height, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

float GLSystem::getZNear()
{
    return this->zNear;
}

float GLSystem::getZFar()
{
    return this->zFar;
}

float GLSystem::getFov()
{
    return this->fov;
}

int GLSystem::getWidth()
{
    return this->width;
}

int GLSystem::getHeight()
{
    return this->height;
}

int GLSystem::getAspectRatio()
{
    return this->aspectratio;
}
