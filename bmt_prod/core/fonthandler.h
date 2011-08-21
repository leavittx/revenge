#pragma once

#include "../globals.h"

#ifdef _WIN32
#include "FTGL/ftgl.h"
#else
#include <FTGL/ftgl.h>
#endif

/**
  DataBuffer - buffer for storing file data
*/
struct DataBuffer
{
	DataBuffer();
	DataBuffer(string path);
	~DataBuffer();

	void read(string path);
	void release();

	unsigned char* m_data;
	long m_size;
};

/**
  FontHandler
*/
typedef map<string, DataBuffer*> FontBufferList;
typedef map<string, vector<FTFont*> > FontList;
typedef FontList::const_iterator FontIter;

extern class FontHandler g_fonts;

class FontHandler
{
public:
	//TODO: singleton
//	static FontHandler& instance();
	FontHandler() {}
	~FontHandler();

	void loadFonts();
	void addFontBuffer(string name, DataBuffer *buffer);
	void addFont(string name, FTFont *font);
	DataBuffer& fontBuffer(string name);

	/**
	  Possible FontType's:
		FTGLBitmapFont
		FTBufferFont
		FTGLExtrdFont
		FTGLOutlineFont
		FTGLPixmapFont
		FTGLPolygonFont
		FTGLTextureFont
	  */
	template <class FontType> FontType& font(string name, unsigned int size);

private:
	//hide these 'cause this is a singleton
//	FontHandler() {}
//	FontHandler(const FontHandler&) {}
//	FontHandler& operator = (const FontHandler&) { return *this; }

	//container for fonts
	FontList m_fonts;
	//container for fonts raw data
	FontBufferList m_buffer_fonts;
};
