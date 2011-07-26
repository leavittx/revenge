//-------------------------------------------------------
// Includes
//-------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <iostream>

#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xrender.h>

#include "../globals.h"
#include "config.h"

//-------------------------------------------------------
//	Globals and static members
//-------------------------------------------------------

const int RELEASE_BUILD = true; //if set to true, use proper resolution handling. Otherwise use DEV_RESOLUTION

const int DEV_RESOLUTION_X = 1024;
const int DEV_RESOLUTION_Y = 768;

const int DEFAULT_MIN_X = 640;

std::vector<Config::Settings> settings;

int Config::frequency;
int Config::resolution;
int Config::bpp;
int Config::gamma;
int Config::fsaa;
int Config::aspectratio;
int Config::glasses;
bool Config::fullscreen;
bool Config::sound;
bool Config::vsync;
bool Config::runFlag;
bool Config::anaglyphic;
bool Config::alwaysOnTop;
bool Config::nv7;
bool Config::randomized;
int Config::smDesktopHeight;
int Config::smDesktopWidth;



string exec(char* cmd)
{
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
    while (!feof(pipe))
    {
        if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
    }
    pclose(pipe);
    return result;
}


//-------------------------------------------------------
//	Constructors and Destructor
//-------------------------------------------------------

Config::Config()
{
    frequency = 60;
    resolution = 0;
    bpp = 32;
    gamma = 0;
    fullscreen = false;
    sound = false;
    vsync = false;
    runFlag = false;
    anaglyphic = false;
    alwaysOnTop = false;
    nv7 = false;
    fsaa = 2;
    aspectratio = 0;
    glasses = 0;

    smDesktopWidth = 0;
    smDesktopHeight = 0;

    XRRScreenConfiguration* screenInfo;
    SizeID currentSize;
    Rotation currentRotation;
    short currentRate;

    int sizesCount;
    XRRScreenSize* sizes;

    Display* display = XOpenDisplay(0);

    screenInfo = XRRGetScreenInfo(display, DefaultRootWindow(display));
    sizes = XRRConfigSizes(screenInfo, &sizesCount);
    currentSize = XRRConfigCurrentConfiguration(screenInfo, &currentRotation);
    currentRate = XRRConfigCurrentRate(screenInfo);

    for (int i = 0; i < sizesCount; i++)
    {
            Settings current;
            current.Width = sizes[i].width;
            current.Height = sizes[i].height;
            current.RefreshRate = currentRate; // TODO
            current.BitsPerPixel = 32; // TODO
            settings.push_back(current);
//            sizeIDs[j] = i;
            //rateIDs[j] = r;
            //bppIDs[j] = b;
    }

    XCloseDisplay(display);
}

Config::~Config()
{
}

//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Config::run()
{
    //TODO: check if zenity avaliable on system
    ofstream configScript;
    configScript.open("demo_config.sh");
    configScript <<
        "#!/bin/bash"                                               << endl <<
        "if"                                                        << endl <<
        "zenity \\"                                                 << endl <<
        "   --display=:0.0 \\"                                      << endl <<
//        "   --hide-header \\"                                       << endl <<
        "   --hide-column=1 \\"                                     << endl <<
        "   --width="  << 80                        << " \\"        << endl <<
        "   --height=" << 128 + 24 * settings.size() << " \\"       << endl <<
        "   --list \\"                                              << endl <<
        "   --title=\"BMT :: Demo\" \\"                             << endl <<
        "   --text=\"Screen Resolution\" \\"                        << endl <<
//        "   --column=ID --column=Resolution \\"                     << endl;
        "   --column=ID --column=Width --column=Height \\"          << endl;

    for (int i = 0; i < settings.size(); i++)
    {
        configScript << "   "                       <<
                        i                  << " "   <<
                        settings[i].Width  << " "   <<
                        settings[i].Height << " \\" << endl;
    }

    configScript <<
        "; then "                                                   << endl <<
        "   echo DEMO_START"                                        << endl <<
        "   if"                                                     << endl <<
        "   zenity \\"                                              << endl <<
        "       --display=:0.0 \\"                                  << endl <<
        "       --question --text=\"Run Mode\" \\"                  << endl <<
        "       --ok-label=Fullscreen --cancel-label=Windowed \\"   << endl <<
        "   ; then "                                                << endl <<
        "       echo DEMO_FULLSCREEN_ENABLE"                        << endl <<
        "   fi"                                                     << endl <<
        "   if"                                                     << endl <<
        "   zenity \\"                                              << endl <<
        "       --display=:0.0 \\"                                  << endl <<
        "       --question --text=\"Enable Sound?\" \\"             << endl <<
        "       --ok-label=Yes! --cancel-label=No \\"               << endl <<
        "   ; then "                                                << endl <<
        "       echo DEMO_SOUND_ENABLE"                             << endl <<
        "   fi"                                                     << endl <<
        "else"                                                      << endl <<
        "   echo DEMO_EXIT"                                         << endl <<
        "fi"                                                        << endl;

    string configOutput = exec("/bin/bash ./demo_config.sh");

    resolution = atoi(configOutput.c_str());

    if (configOutput.find("DEMO_EXIT") == configOutput.npos)
    {
        //when user doesn't select any resolution from list,
        //zenity prints no id, so this will be first
        if (configOutput.find("DEMO_START") == 0)
            resolution = 0;
        Config::runFlag = true;
    }
    else
    {
        Config::runFlag = false;
    }


    if (configOutput.find("DEMO_FULLSCREEN_ENABLE") == configOutput.npos)
        fullscreen = false;
    else
        fullscreen = true;

    if (configOutput.find("DEMO_SOUND_ENABLE") == configOutput.npos)
         sound = false;
    else
        sound = true;

    if (!getRunFlag())
        return false;

    return true;
}

int Config::getScreenX()
{
    return settings[resolution].Width;
}

int Config::getScreenY()
{
    return settings[resolution].Height;
}

int Config::getBpp()
{
    return bpp;
}

int Config::getGamma()
{
    return gamma;
}
int Config::getFsaa()
{
    return fsaa;
}
bool Config::getFullscreen()
{
    return fullscreen;
}

bool Config::getSound()
{
    return sound;
}

bool Config::getVsync()
{
    return vsync;
}

int Config::getFrequency()
{
#ifdef _WIN32
    return devModes[resolution].dmDisplayFrequency;
#else
    //TODO
    return 0;
#endif
}

bool Config::getAnaglyphic()
{
    return anaglyphic;
}

bool Config::getOnTop()
{
    return alwaysOnTop;
}

bool Config::getRandomized()
{
    return randomized;
}


bool Config::getRunFlag()
{
    return runFlag;
}

int Config::getAspectRatio()
{
    return aspectratio;
}

int Config::getGlasses()
{
    return glasses;
}
