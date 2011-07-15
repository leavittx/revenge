#pragma once

#include "stdio.h"
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

/*
    Config class used for the demo settings. Includes the startup dialog.
*/

class Config
{		
public:

    Config();
    ~Config();

    bool run();

    // getterit
    int getScreenX();
    int getScreenY();
    int getBpp();
    int getGamma();
    int getFsaa();
    int getAspectRatio();
    int getFrequency();
    bool getFullscreen();
    bool getSound();
    bool getVsync();
    bool getOnTop();
    bool getAnaglyphic();
    bool getRunFlag();
    int getGlasses();
    bool getRandomized();

#if defined(_WIN32)
    static int ConfigProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif /* defined(_WIN32) */

    static int resolution, bpp, gamma, fsaa, aspectratio, glasses, frequency;
    static bool fullscreen, sound, vsync, runFlag, anaglyphic, alwaysOnTop, nv7, randomized;
    static int smDesktopWidth, smDesktopHeight;

private:

};

