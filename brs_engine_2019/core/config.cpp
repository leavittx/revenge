//-------------------------------------------------------
// Includes
//-------------------------------------------------------

#ifdef _WIN32
#include <windows.h>
#include <commctrl.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "../globals.h"
#ifdef _WIN32
#include "../resources/resource.h"
#endif
#include "config.h"

#ifdef _MSC_VER
#pragma warning (disable : 4996)
#endif

//-------------------------------------------------------
//	Globals and static members
//-------------------------------------------------------

const int RELEASE_BUILD = true; //if set to true, use proper resolution handling. Otherwise use DEV_RESOLUTION

const int DEV_RESOLUTION_X = 1024;
const int DEV_RESOLUTION_Y = 768;

const int DEFAULT_MIN_X = 640;

#ifdef _WIN32
std::vector<DEVMODE> devModes;
#endif

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

#ifdef _WIN32
    DEVMODE currentDevMode;
    currentDevMode.dmSize = sizeof(DEVMODE);
    currentDevMode.dmDriverExtra = 0;
    EnumDisplaySettingsEx(NULL, ENUM_CURRENT_SETTINGS, &currentDevMode, 0);

    smDesktopWidth  = currentDevMode.dmPelsWidth;
    smDesktopHeight = currentDevMode.dmPelsHeight;

    int  nModeExist;
    DEVMODE devMode;
    int lastW=0, lastH=0;
    for (int i=0; ;i++)
    {
        ZeroMemory(&devMode, sizeof(DEVMODE));
        devMode.dmSize = sizeof(DEVMODE);
        nModeExist = EnumDisplaySettings(NULL, i, &devMode);


        if (nModeExist != 1)
        {
            break;
        }
        else if(devMode.dmBitsPerPel >= 32
                && devMode.dmPelsWidth >= DEFAULT_MIN_X)
            //&& devMode.dmPelsWidth <= DEFAULT_MAX_X)
        {
            devModes.push_back(devMode);
        }
    }
#else
    //todo
#endif
}
Config::~Config()
{
}

//-------------------------------------------------------
//	Public functions
//-------------------------------------------------------

bool Config::run()
{
#ifdef _WIN32
    if(DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_SETUPDLG), NULL, (DLGPROC)ConfigProc) == 1)
    {
        return false;
    }
#endif

    if(!getRunFlag())
        return false;

    return true;
}

int Config::getScreenX()
{
#ifdef _WIN32
    return devModes[resolution].dmPelsWidth;
#else
    //todo
    return 0;
#endif
}

int Config::getScreenY()
{
    #ifdef _WIN32
    return devModes[resolution].dmPelsHeight;
#else
    //todo
    return 0;
#endif
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
    //todo
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

#ifdef _WIN32
//-------------------------------------------------------
//	Dialog procedure - handles inputs
//-------------------------------------------------------

int Config::ConfigProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        InitCommonControls();

        RECT rect, dlgRect;
        HWND dlg;

        // Center the dialog box
        dlg = GetDesktopWindow();
        if(dlg)
        {
            GetWindowRect(dlg, &rect);
            GetWindowRect(hwnd, &dlgRect);
            SetWindowPos(hwnd, HWND_TOPMOST, (rect.right/2) - ((dlgRect.right - dlgRect.left) / 2), (rect.bottom/2) - ((dlgRect.bottom - dlgRect.top) / 2), 0, 0, SWP_NOSIZE);
        }

        HWND hwndList;

        hwndList = GetDlgItem(hwnd, IDC_ASPECTRATIO);
        SendMessage(hwndList,CB_ADDSTRING,0,(LPARAM)(LPCSTR)"4:3");
        SendMessage(hwndList,CB_ADDSTRING,1,(LPARAM)(LPCSTR)"16:10");
        SendMessage(hwndList,CB_ADDSTRING,2,(LPARAM)(LPCSTR)"16:9");
        SendMessage(hwndList,CB_SETCURSEL,0,0);
        aspectratio = 0;

        /*
   hwndList = GetDlgItem(hwnd, IDC_GLASSES);
   SendMessage(hwndList,CB_ADDSTRING,0,(LPARAM)(LPCSTR)"No 3D [not recommended]");
   SendMessage(hwndList,CB_ADDSTRING,1,(LPARAM)(LPCSTR)"Red   +  Cyan");
   SendMessage(hwndList,CB_ADDSTRING,2,(LPARAM)(LPCSTR)"Red   +  Blue");
   SendMessage(hwndList,CB_ADDSTRING,3,(LPARAM)(LPCSTR)"Red   +  Green");
   SendMessage(hwndList,CB_SETCURSEL,1,0); // LAST INDEX MUST BE ZERO
   glasses = 1;
   */

        // -- resolution

        hwndList = GetDlgItem(hwnd, IDC_RESOLUTION);

        std::vector<DEVMODE>::iterator it;
        int i=0, z=0, d=0;
        //for (it=devModes.begin(); it!=devModes.end(); it++)
        it=devModes.begin();
        while(it!=devModes.end())
        {
            char buf[100];
            sprintf(buf, "%dx%d 32 bpp %d hz", (*it).dmPelsWidth,  (*it).dmPelsHeight, (*it).dmDisplayFrequency);
            z = SendMessage(hwndList, CB_ADDSTRING, i, (LPARAM)(LPCSTR)buf);

            if (RELEASE_BUILD)
            {
                if((*it).dmPelsWidth==smDesktopWidth  && (*it).dmPelsHeight==smDesktopHeight)
                    d = z;
            }
            else
            {
                if((*it).dmPelsWidth==DEV_RESOLUTION_X && (*it).dmPelsHeight==DEV_RESOLUTION_Y)
                    d = z;
            }

            it++;
            i++;
        }


        SendMessage(hwndList, CB_SETCURSEL, d, 0);
        resolution = d;
        bpp = 32;

        /*
   hwndList = GetDlgItem(hwnd, IDC_FSAA);
   SendMessage(hwndList,CB_ADDSTRING,0,(LPARAM)(LPCSTR)"0");
   SendMessage(hwndList,CB_ADDSTRING,2,(LPARAM)(LPCSTR)"2");
   SendMessage(hwndList,CB_ADDSTRING,4,(LPARAM)(LPCSTR)"4");
   SendMessage(hwndList,CB_ADDSTRING,6,(LPARAM)(LPCSTR)"6");
   SendMessage(hwndList,CB_SETCURSEL,1,0); // LAST INDEX MUST BE ZERO
   */
        fsaa = 4;



        // Initialize values
        /*
   //SendDlgItemMessage(hwnd, IDC_FSAA, WM_SETTEXT, 0, (LPARAM)"No FSAA");

   SendMessage(GetDlgItem(hwnd, IDC_FSAA), CB_ADDSTRING, 0, (LPARAM)"No FSAA");
   SendMessage(GetDlgItem(hwnd, IDC_FSAA), CB_ADDSTRING, 0, (LPARAM)"FSAA 2");
   SendMessage(GetDlgItem(hwnd, IDC_FSAA), CB_ADDSTRING, 0, (LPARAM)"FSAA 4");
   SendMessage(GetDlgItem(hwnd, IDC_FSAA), CB_ADDSTRING, 0, (LPARAM)"FSAA 6");
   SendDlgItemMessage(hwnd, IDC_FSAA, CB_SETCURSEL, 1, 0);

   lResult = SendMessage(// returns LRESULT in lResult
      (HWND) hWndControl,// handle to destination control
      (UINT) WM_SETFONT,// message ID
      (WPARAM) wParam,// = (WPARAM) () wParam;
      (LPARAM) lParam// = (LPARAM) () lParam;
   );
   */

        // initialize nvidia box 7xxx box
        /*
   char *rs = (char *) glGetString(GL_RENDERER);
   MessageBox(0,0,rs,0);
   if(strstr(rs, "GeForce 7") != NULL)
   {
    //SendDlgItemMessage(hwnd, IDC_NVIDIA7, BM_SETCHECK, wParam, 0);
   }
   */

        // Show the dialog
        ShowWindow(hwnd, SW_SHOW);
        SetActiveWindow(hwnd);
        SetForegroundWindow(hwnd);
        SetFocus(hwnd);

        // Set icon
        SendMessage(hwnd,WM_SETICON,ICON_BIG,(LPARAM)LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_ICON1)));

        // Check default values
        if (RELEASE_BUILD)
        {
            SendDlgItemMessage(hwnd, IDC_FULLSCREEN, BM_SETCHECK, wParam, 0);
        }
        //SendDlgItemMessage(hwnd, IDC_RANDOMIZE, BM_SETCHECK, wParam, 0);
        SendDlgItemMessage(hwnd, IDC_SOUND, BM_SETCHECK, wParam, 0);

        // Set ONTOP to disabled or enabled at startup
        if(IsDlgButtonChecked(hwnd, IDC_FULLSCREEN)==BST_CHECKED)
            EnableWindow(GetDlgItem(hwnd, IDC_ONTOP), FALSE);
        else
            EnableWindow(GetDlgItem(hwnd, IDC_ONTOP), TRUE);

    } break;

    case WM_COMMAND:
    {
        if(IsDlgButtonChecked(hwnd, IDC_FULLSCREEN)==BST_CHECKED)
            EnableWindow(GetDlgItem(hwnd, IDC_ONTOP), FALSE);
        else
            EnableWindow(GetDlgItem(hwnd, IDC_ONTOP), TRUE);


        switch(LOWORD(wParam))
        {

        /*
    case IDC_ASPECTRATIO:

     if(HIWORD(wParam) == CBN_SELCHANGE)
      aspectratio = SendMessage(GetDlgItem(hwnd, IDC_ASPECTRATIO), CB_GETCURSEL, 0, 0);
     break;


    case IDC_GLASSES:
     if(HIWORD(wParam) == CBN_SELCHANGE)
      glasses = SendMessage(GetDlgItem(hwnd, IDC_GLASSES), CB_GETCURSEL, 0, 0);
     break;

    case IDC_RESOLUTION:

     if(HIWORD(wParam) == CBN_SELCHANGE)
     {
      resolution = SendMessage(GetDlgItem(hwnd, IDC_RESOLUTION), CB_GETCURSEL, 0, 0);

     }
     break;

    case IDC_FSAA:

     if(HIWORD(wParam) == CBN_SELCHANGE)
     {
      fsaa = SendMessage(GetDlgItem(hwnd, IDC_FSAA), CB_GETCURSEL, 0, 0);
     }

     break;
    */
        case IDOK:
        {
            resolution = SendMessage(GetDlgItem(hwnd, IDC_RESOLUTION), CB_GETCURSEL, 0, 0);
            aspectratio = SendMessage(GetDlgItem(hwnd, IDC_ASPECTRATIO), CB_GETCURSEL, 0, 0);

            if(SendDlgItemMessage(hwnd, IDC_FULLSCREEN, BM_GETSTATE, 0, 0) == BST_CHECKED)
            {
                fullscreen = true;
            }

            if(SendDlgItemMessage(hwnd, IDC_SOUND, BM_GETSTATE, 0, 0) == BST_CHECKED)
            {
                sound = true;
            }
            else
            {
                sound = false;
            }

            if (SendDlgItemMessage(hwnd, IDC_RANDOMIZE, BM_GETSTATE, 0, 0) == BST_CHECKED)
            {
                randomized = true;
            }
            else
            {
                randomized = false;
            }
            /*

     if(SendDlgItemMessage(hwnd, IDC_VSYNC, BM_GETSTATE, 0, 0) == BST_CHECKED)
     {
      vsync = true;
     }
                    if(SendDlgItemMessage(hwnd, IDC_ANAGLYPHIC, BM_GETSTATE, 0, 0) == BST_CHECKED)
     {
      anaglyphic = true;
     }
*/

            if(SendDlgItemMessage(hwnd, IDC_ONTOP, BM_GETSTATE, 0, 0) == BST_CHECKED)
            {
                alwaysOnTop = true;
            }
            /*
     if(SendDlgItemMessage(hwnd, IDC_NVIDIA7, BM_GETSTATE, 0, 0) == BST_CHECKED)
     {
      nv7 = true;
     }
*/
            //gamma = 0.4f;//SendDlgItemMessage(hwnd, IDC_GAMMA, TBM_GETPOS, 0, 0);

            Config::runFlag = true;
            EndDialog(hwnd, 0);
        } break;

        case IDCANCEL:
        {
            Config::runFlag = false;
            SendMessage(hwnd, WM_CLOSE, 0, 0);
        } break;

        } break;

    } break;

    case WM_DESTROY:
    {
        SendMessage(hwnd, WM_CLOSE, 0, 0);
    } break;

    case WM_CLOSE:
    {
        EndDialog(hwnd, 0);
    }
        break;
    }



    return 0;
}
#endif /* defined(_WIN32) */
