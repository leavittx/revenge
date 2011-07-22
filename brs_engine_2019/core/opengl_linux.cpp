#include "opengl.h"
#include "../globals.h"

#include <X11/X.h>

#include <time.h>

#include <GL/gl.h>
#include <GL/glx.h>

#include <stdio.h>
#include <string.h>

//#include "GL/glus.h"

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

static GLUSuint g_major = 1;
static GLUSuint g_minor = 0;
static GLUSint g_flags = 0;

static GLUSboolean g_fullscreen = GL_FALSE;
static GLUSboolean g_active = GL_FALSE;
static GLUSboolean g_initdone = GL_FALSE;

static GLUSuint g_width = 640;
static GLUSuint g_height = 480;

static Display* g_Display = None;
static Window g_Window = 0;
static GLXContext g_Context = None;
static Atom g_DeleteMessage;
static Atom g_StateMessage;
static Atom g_FullscreenMessage;

//static GLUSboolean (*glusInit)(GLUSvoid) = None;
//static GLUSvoid (*glusReshape)(GLUSuint width, GLUSuint height) = None;
//static GLUSboolean (*glusUpdate)(GLUSfloat time) = None;
//static GLUSvoid (*glusTerminate)(GLUSvoid) = None;

//static GLUSvoid (*glusKey)(GLUSboolean pressed, GLUSuint key) = NULL;
//static GLUSvoid (*glusMouse)(GLUSboolean pressed, GLUSuint button, GLUSuint xPos, GLUSuint yPos) = NULL;
//static GLUSvoid (*glusMouseWheel)(GLUSuint buttons, GLUSint ticks, GLUSuint xPos, GLUSuint yPos) = NULL;
//static GLUSvoid
//(*glusMouseMove)(GLUSuint buttons, GLUSuint xPos, GLUSuint yPos) = NULL;


GLUSvoid GLUSAPIENTRY glusPrepareContext(GLUSuint major, GLUSuint minor, GLUSint flags)
{
    g_major = major >= 1 ? major : 0;
    g_minor = minor;
    g_flags = flags;
}

GLUSfloat glusGetElapsedTime(GLUSvoid)
{
    static GLUSboolean init = GLUS_FALSE;
    static GLUSfloat lastTime;
    static GLUSfloat currentTime;

    static struct timespec initial;

    GLUSfloat measuredTime;

    struct timespec now;

    if (!init)
    {
        clock_gettime(CLOCK_REALTIME, &initial);
    }

    clock_gettime(CLOCK_REALTIME, &now);

    measuredTime = (GLUSfloat) (now.tv_sec - initial.tv_sec) + (GLUSfloat) now.tv_nsec / 1000000000.0f;

    if (!init)
    {
        lastTime = measuredTime;

        currentTime = measuredTime;

        init = GLUS_TRUE;
    }
    else
    {
        lastTime = currentTime;

        currentTime = measuredTime;
    }

    return currentTime - lastTime;
}

GLUSvoid GLUSAPIENTRY glusDestroyWindow(GLUSvoid)
{
    if (g_fullscreen && g_Display && g_Window) // Are We In Fullscreen Mode?
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

    g_initdone = GL_FALSE;
}

GLUSboolean GLUSAPIENTRY glusCreateWindow(const char* title, GLUSuint width, GLUSuint height, GLUSboolean fullscreen)
{
    Window RootWindow = 0;
    XVisualInfo* VisualInfo = None;
    Colormap CurrentColorMap = 0;
    XSetWindowAttributes CurrentSetWindowAttibutes;
    GLUSint fbcCount = 0;
    GLXFBConfig* fbc = None;

    GLUSint
            visualAttribList[] =
    { GLX_X_RENDERABLE, True, GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8, GLX_DEPTH_SIZE, 16, GLX_STENCIL_SIZE, 8, GLX_DOUBLEBUFFER, True, None };

    g_Display = XOpenDisplay(None);

    if (g_Display == None)
    {
        return GLUS_FALSE;
    }

    RootWindow = DefaultRootWindow(g_Display);

    if (!(fbc = glXChooseFBConfig(g_Display, DefaultScreen(g_Display), visualAttribList, &fbcCount)))
    {
        glusDestroyWindow();

        return GLUS_FALSE;
    }

    VisualInfo = glXGetVisualFromFBConfig(g_Display, fbc[0]);

    if (VisualInfo == None)
    {
        XFree(fbc);

        glusDestroyWindow();

        return GLUS_FALSE;
    }

    CurrentColorMap = XCreateColormap(g_Display, RootWindow, VisualInfo->visual, AllocNone);

    CurrentSetWindowAttibutes.colormap = CurrentColorMap;
    CurrentSetWindowAttibutes.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask;

    g_Window
            = XCreateWindow(g_Display, RootWindow, 0, 0, width, height, 0, VisualInfo->depth, InputOutput, VisualInfo->visual, CWColormap | CWEventMask, &CurrentSetWindowAttibutes);

    if (g_Window == 0)
    {
        XFree(fbc);

        glusDestroyWindow();

        return GLUS_FALSE;
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

        //

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

        g_fullscreen = fullscreen;
    }

    g_Context = glXCreateContext(g_Display, VisualInfo, None, GLUS_TRUE);

    if (g_Context == None)
    {
        XFree(fbc);

        glusDestroyWindow();

        return GLUS_TRUE;
    }

    if (!glXMakeCurrent(g_Display, g_Window, g_Context))
    {
        XFree(fbc);

        glusDestroyWindow();

        return GLUS_TRUE;
    }

    if (g_major >= 3)
    {
        GLXContext Context = None;
        PFNGLXCREATECONTEXTATTRIBSARBPROCTEMP glXCreateContextAttribsARBTemp = None;

        GLUSint attribList[] = { GLX_CONTEXT_MAJOR_VERSION_ARB, 1, GLX_CONTEXT_MINOR_VERSION_ARB, 0, GLX_CONTEXT_FLAGS_ARB, 0, 0 };

        attribList[1] = g_major;
        attribList[3] = g_minor;
        attribList[5] = g_flags;

        if (!(glXCreateContextAttribsARBTemp = (PFNGLXCREATECONTEXTATTRIBSARBPROCTEMP) glXGetProcAddress((const GLUSubyte *) "glXCreateContextAttribsARB")))
        {
            XFree(fbc);

            glusDestroyWindow();

            return GLUS_FALSE; // Return FALSE
        }

        if (!(Context = glXCreateContextAttribsARBTemp(g_Display, fbc[0], 0, True, attribList)))
        {
            XFree(fbc);

            glusDestroyWindow();

            return GLUS_FALSE; // Return FALSE
        }
        XFree(fbc);

        if (!glXMakeCurrent(g_Display, None, None))
        {
            glXDestroyContext(g_Display, Context);

            glusDestroyWindow();

            return GLUS_FALSE; // Return FALSE
        }

        glXDestroyContext(g_Display, g_Context);

        g_Context = Context;

        if (!glXMakeCurrent(g_Display, g_Window, g_Context))
        {
            glusDestroyWindow();

            return GLUS_TRUE;
        }
    }

    g_width = width;
    g_height = height;

    return GLUS_TRUE; // Success
}

GLUSboolean GLUSAPIENTRY glusRun(GLUSvoid)
{
    //MSG		msg;									// Windows Message Structure
    XEvent msg;

    GLUSboolean done = GLUS_FALSE; // Bool Variable To Exit Loop

    GLUSuint mouseButtons = 0;

    GLUSint mouseX, mouseY, mousePrevX, mousePrevY, dummy;

    // Init Engine
//    if (glusInit)
//    {
//        if (!glusInit())
//        {
//            glusDestroyWindow(); // Destroy The Window

//            return GLUS_FALSE; // Exit The Program
//        }
//    }
    // http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clearcolor.html
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/cleardepth.html
    glClearDepth(1.0f);



    g_initdone = GLUS_TRUE;

    // Do the first reshape
//    if (glusReshape)
//    {
//        glusReshape(g_width, g_height);
//    }
    // http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/viewport.html
    glViewport(0, 0, g_width, g_height);



    XQueryPointer(g_Display, g_Window, (Window *) &dummy, (Window *) &dummy, (GLUSint*) &dummy, (GLUSint*) &dummy, &mousePrevX, &mousePrevY, (GLUSuint*) &dummy);

    while (!done) // Loop That Runs While done=FALSE
    {
        if (XPending(g_Display))
        {
            XNextEvent(g_Display, &msg);

            if (msg.type == KeyPress)
            {
                if (msg.xkey.keycode == 0x09) // Have We Received the Esc Key
                {
                    // do nothing
                }
                else
                {
//                    if (glusKey && g_initdone)
//                    {
//                        char c;

//                        if (XLookupString(&msg.xkey, &c, 1, 0, 0) > 0)
//                        {
//                            glusKey(GLUS_TRUE, c);
//                        }
//                    }
                }
            }
            else if (msg.type == KeyRelease)
            {
                if (msg.xkey.keycode == 0x09) // Have We Received the Esc Key
                {
                    done = GLUS_TRUE; // If So done=TRUE
                }
                else
                {
//                    if (glusKey && g_initdone)
//                    {
//                        char c;

//                        if (XLookupString(&msg.xkey, &c, 1, 0, 0) > 0)
//                        {
//                            glusKey(GLUS_FALSE, c);
//                        }
//                    }
                }
            }
            else if (msg.type == ButtonPress)
            {
//                if (glusMouse && g_initdone)
//                {
//                    if (msg.xbutton.button <= 3)
//                    {
//                        mouseButtons |= 1 << (msg.xbutton.button - 1);

//                        glusMouse(GLUS_TRUE, msg.xbutton.button, msg.xbutton.x, msg.xbutton.y);
//                    }
//                    else if (msg.xbutton.button == 4)
//                    {
//                        glusMouseWheel(mouseButtons, 1, msg.xbutton.x, msg.xbutton.y);
//                    }
//                    else if (msg.xbutton.button == 5)
//                    {
//                        glusMouseWheel(mouseButtons, -1, msg.xbutton.x, msg.xbutton.y);
//                    }
//                }
            }
            else if (msg.type == ButtonRelease)
            {
//                if (glusMouse && g_initdone)
//                {
//                    if (msg.xbutton.button <= 3)
//                    {
//                        mouseButtons ^= 1 << (msg.xbutton.button - 1);

//                        glusMouse(GLUS_FALSE, msg.xbutton.button, msg.xbutton.x, msg.xbutton.y);
//                    }
//                }
            }
            else if (msg.type == ClientMessage)
            {
                if (msg.xclient.data.l[0] == g_DeleteMessage) // Have We Received a Delete Message
                {
                    done = GLUS_TRUE; // If So done=TRUE
                }
            }
            else if (msg.type == ConfigureNotify)
            {
                g_width = msg.xconfigure.width;
                g_height = msg.xconfigure.height;

                if (/*glusReshape && */g_initdone)
                {
//                    glusReshape(g_width, g_height);
                    // http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/viewport.html
                    glViewport(0, 0, g_width, g_height);
                }
            }
            else if (msg.type == MapNotify)
            {
                g_active = GLUS_TRUE;
            }
            else if (msg.type == UnmapNotify)
            {
                g_active = GLUS_FALSE;
            }
        }

        // Draw The Scene.
        if (g_active && !done) // Program Active?
        {
//            if (glusUpdate)
//            {
                if (/*glusMouseMove*/0 && g_initdone)
                {
                    XQueryPointer(g_Display, g_Window, (Window *) &dummy, (Window *) &dummy, (GLUSint*) &dummy, (GLUSint*) &dummy, &mouseX, &mouseY, (GLUSuint*) &dummy);

                    if (mouseX != mousePrevX || mouseY != mousePrevY && (mouseX >= 0 && mouseX <= g_width && mouseY >= 0 && mouseY <= g_height))
                    {
                        mousePrevX = mouseX;
                        mousePrevY = mouseY;

//                        glusMouseMove(mouseButtons, mouseX, mouseY);
                    }
                }

                //update the system (sync etc)
                g_system.update();

//                done = !glusUpdate(glusGetElapsedTime());
                // http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clear.html
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                g_demo->update();
                g_demo->draw();

//                g_system.swapBuffers();
                glXSwapBuffers(g_Display, g_Window); // Swap Buffers (Double Buffering)
//            }

//            if (!done)
//            {
//                glXSwapBuffers(g_Display, g_Window); // Swap Buffers (Double Buffering)
//            }
        }
    }

    // Terminate Game
//    if (glusTerminate)
//    {
//        glusTerminate();
//    }

    // Shutdown
    glusDestroyWindow(); // Destroy The Window

    return GLUS_TRUE; // Exit The Program
}



GLWindow::GLWindow()
{
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
    fsaa = 0; //todo - 2 ???

    //	wglSwapIntervalEXT = NULL;
    //	wglGetSwapIntervalEXT = NULL;

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

//todo
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

    glusPrepareContext(3, 1, GLUS_BACKWARD_COMPATIBLE_BIT); //GLUS_FORWARD_COMPATIBLE_BIT

    if (!glusCreateWindow(title, width, height, fullscreen))
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

        glusDestroyWindow();
        return false;
    }

    return true;
}

bool GLWindow::kill()
{
    glusDestroyWindow();

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

bool GLWindow::extensionExist(const char *extension)
{
    // try to find extension from wgl/gl extensions lists
    /*
 PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = 0;
  wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
  if(wglGetExtensionsStringARB)
  {
 const char *winsys_extensions = wglGetExtensionsStringARB(wglGetCurrentDC());
 if (winsys_extensions && (strstr(winsys_extensions,extension)))
   return true;
  }
  const char *glExtensions = (const char*)glGetString(GL_EXTENSIONS);
  if (!glExtensions)
 return false;

  return (strstr(glExtensions,extension) != NULL);
*/
    return true;
}

bool GLWindow::getKeyDown(int i)
{
    return keysDown[i];
}
bool GLWindow::getKeyPressed(int i)
{
    return keysPressed[i];
}

void GLWindow::setWindowTitle(const string title)
{
    //    HWND handle = this->getHandle();
    //    SetWindowText(handle, title.c_str());
}


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
