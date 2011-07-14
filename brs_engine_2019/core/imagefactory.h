#pragma once

#include "../globals.h"

class ImageFactory
{
public:
	static class Image* loadPNG(string filename);
	static class Image* loadJPG(string filename);
	static class Image* createEmpty(int width, int height);
};

