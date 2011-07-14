#pragma once

#include "stdio.h"
#include <vector>
#include <windows.h>

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
		
		static int ConfigProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		static int resolution, bpp, gamma, fsaa, aspectratio, glasses, frequency;
		static bool fullscreen, sound, vsync, runFlag, anaglyphic, alwaysOnTop, nv7, randomized;
		static int smDesktopWidth, smDesktopHeight;

	private: 

};

