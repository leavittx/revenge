#pragma once

#ifdef _MSC_VER
//fix visual studio "debug information truncated to 255 bytes"-whining
#pragma warning ( disable : 4786)
//no warnings about unsafe functions
#define _CRT_SECURE_NO_WARNINGS
#endif

#define GLEW_STATIC /* use static version and not the DLL */

#ifdef _WIN32
#include <windows.h>
#else
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned int UNINT32;
#endif

#include <GL/glew.h>

#if !defined(_WIN32)
#include "core/GLUS/GL/glus.h"
#endif

#include <map>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

/**
  Global stuff
  */

extern class Debugger g_debug;
extern class Parameters g_params;
extern class PostProcess g_postprocess;
extern class System g_system;
extern class Demo *g_demo;

/**
  Engine headers
  */

#include "util/stringutils.h"
#include "util/vector2.h"
#include "util/vector3.h"
#include "util/color3.h"
#include "util/color4.h"
#include "util/range.h"
#include "util/matrix.h"
#include "util/curve.h"
#include "util/linearcurve.h"
#include "util/catmullrom.h"
#include "util/bezier.h"
#include "util/mathematics.h"
#include "util/glutil.h"
#include "util/particlesystem.h"
#include "util/billboard.h"
#include "util/mesh.h"
#include "util/meshfactory.h"
#include "util/material.h"
#include "util/materialfactory.h"
#include "util/primitives.h"
#include "util/frustum.h"
#include "util/skybox.h"
#include "util/light.h"
#include "util/material.h"
#include "util/entity.h"

#include "core/debugger.h"
#include "core/scene.h"
#include "core/demo.h"
#include "core/opengl.h"
#include "core/sound.h"
#include "core/bpm.h"
#include "core/parameter.h"
#include "core/bpm.h"
#include "core/trigger.h"
#include "core/event.h"
#include "core/image.h"
#include "core/imagefactory.h"
#include "core/texture.h"
#include "core/cubemap.h"
#include "core/texturehandler.h"
#include "core/fbo.h"
#include "core/shader.h"
#include "core/shaderhandler.h"
#include "core/postprocess.h"
#include "core/camera.h"
#include "core/timer.h"
#include "core/system.h"

#include "core/keyboard.h"

#include "core/fonthandler.h"
#include "core/pitchdetector.h"
