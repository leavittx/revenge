#include "fonthandler.h"

#include <typeinfo>

/**
  DataBuffer
*/
DataBuffer::DataBuffer() : m_data(NULL), m_size(0)
{
}

DataBuffer::DataBuffer(string path) : m_data(NULL), m_size(0)
{
	read(path);
}

DataBuffer::~DataBuffer()
{
	release();
}

void DataBuffer::read(string path)
{
#if 1
	//c style
	FILE *inputFile;
	inputFile = fopen(path.c_str(), "rb");
	if (!inputFile)
	{
		g_debug << "Unable to open font " << path << " for reading" << endl;
		return;
	}

	fseek(inputFile, 0, SEEK_END);
	m_size = ftell(inputFile);
	fseek(inputFile, 0, SEEK_SET);

	if (m_data)
	{
		release();
	}

	m_data = (unsigned char *)malloc(m_size);
	if (!m_data)
	{
		g_debug << "Too large file" << path << " - data buffer not allocated" << endl;
		return;
	}

	fread(m_data, m_size, 1, inputFile);
	fclose(inputFile);

	//debug: check for difference (e.g. font.ttf and font.ttf.test)
//	FILE *outputFile;
//	outputFile = fopen(string(path + ".test").c_str(), "wb");
//	fwrite(m_data, m_size, 1, outputFile);
//	fclose(outputFile);
#else
	//c++ style

//	basic_ifstream<unsigned char> inputFile;
	ifstream inputFile;
	//open file in binary reading mode and seek to end
	inputFile.open(path, ios_base::binary | ios_base::ate);
	if (!inputFile)
	{
		g_debug << "Unable to open font " << path << " for reading" << endl;
		return;
	}

	//buffer already contains data
	if (m_data)
	{
		release();
	}

	//get file size
	m_size = inputFile.tellg();
	//TODO: check for empty file
	//allocate memory for storing data
	m_data = new unsigned char [m_size];
	if (!m_data)
	{
		g_debug << "Too large file" << path << " - data buffer not allocated" << endl;
		return;
	}

	//go to the file beginning
	inputFile.seekg(0, ios_base::beg);
	//read all data from file
	inputFile.read(reinterpret_cast<char *>(m_data), m_size);

//	if (inputFile.gcount() != m_size)
//	{
//		g_debug << "gcount() = " << inputFile.gcount() << "; size = " << m_size << endl;
//	}

	inputFile.close();
#endif
}

void DataBuffer::release()
{
#if 1
	//c style
	free(m_data);
#else
	//c++ style
	delete m_data;
#endif
	m_data = NULL;
}

/**
  FontHandler
*/
//FontHandler& FontHandler::instance()
//{
//	static FontHandler tm;
//	return tm;
//}

FontHandler::~FontHandler()
{
//	FontIter font;
//	for (font = m_fonts.begin(); font != m_fonts.end(); font++)
//	{
//		delete (*font).second;
//	}

//	m_fonts.clear();
}

void FontHandler::loadFonts()
{
	vector<string> filenames;

#ifdef _WIN32
	string directory = "data\\fonts\\";

	StringUtils::iterateDirectory(directory, filenames);
#else
	string directory = "data/fonts/";

	StringUtils::iterateDirectory(directory, filenames, false);
#endif

	//add steps, one for uploading and one for loading
	g_system.addLoadingScreenSteps(filenames.size() * 2);

	vector<string>::iterator it;
	for (it = filenames.begin(); it < filenames.end(); it++)
	{
		string filename = *it;
		string path = directory + filename;
		string suffix = filename.substr(filename.length()-3, filename.length());

		if (suffix == "ttf")
		{
			DataBuffer *buffer = new DataBuffer(path);
			if (!buffer->m_data)
			{
				//something went wrong, read other fonts
				g_system.advanceLoadingScreen(1);
				continue;
			}

			addFontBuffer(filename, buffer);
			g_system.advanceLoadingScreen(1);
		}
		else
		{
			g_debug << "non-font file " << filename << " found in fonts directory!" << endl;
		}
		g_system.drawLoadingScreen();
	}
}

void FontHandler::addFontBuffer(string name, DataBuffer *buffer)
{
	m_buffer_fonts[name] = buffer;
}

//void FontHandler::addFont(string name, FTFont *font)
//{
//	m_fonts[name] = font;
//}

//DataBuffer& FontHandler::fontBuffer(string name)
//{
//	if (m_buffer_fonts.find(name) == m_buffer_fonts.end())
//	{
//		g_debug << "Cannot find font data buffer for" << name << "!" << endl;
//	}

//	return *m_buffer_fonts[name];
//}

template <class FontType>
	FontType& FontHandler::font(string name, unsigned int size)
{
	//font already initialized
	if (m_fonts.find(name) != m_fonts.end())
	{
		for (int i = 0; i < (int)(m_fonts[name].size()); i++)
//			if (typeid (FontType) == typeid (m_fonts[name][i]))
			if (dynamic_cast<FontType*>(m_fonts[name][i]))
			{
				m_fonts[name][i]->FaceSize(size);
				return *((FontType *)m_fonts[name][i]);
			}
	}

	if (m_buffer_fonts.find(name) == m_buffer_fonts.end())
	{
		g_debug << "Cannot find font data buffer for" << name << "!" << endl;
		//TODO: avoid program crash after that
	}

	FTFont* font = new FontType(m_buffer_fonts[name]->m_data, m_buffer_fonts[name]->m_size);

	//TODO: handle all errors (see freetype file fterrdef.h)
	//TODO: avoid program crash if error occurred
	int freetypeError = font->Error();
	switch (freetypeError)
	{
	case 0x00:
		//all ok
		break;
	case 0x01:
		g_debug << "Cannot initialize font " << name << " (cannot open resource)" << endl;
		break;
	case 0x02:
		g_debug << "Cannot initialize font " << name << " (unknown file format)" << endl;
		break;
	case 0x03:
		g_debug << "Cannot initialize font " << name << " (broken file)" << endl;
		break;
	case 0x04:
		g_debug << "Cannot initialize font " << name << " (invalid FreeType version)" << endl;
		break;
	default:
		g_debug << "Cannot initialize font " << name << endl;
		break;
	}

	if (!font->FaceSize(size))
	{
		g_debug << "Font " << name << " failed to set size " << size << endl;
		delete font;
		//TODO: avoid program crash after that
	}

	m_fonts[name].push_back(font);

	return *((FontType *)m_fonts[name].back());
}

//this allows to define templates outside header (http://www.parashift.com/c++-faq-lite/templates.html#faq-35.13)
template FTGLBitmapFont&  FontHandler::font<FTGLBitmapFont> (string name, unsigned int size);
template FTBufferFont&    FontHandler::font<FTBufferFont>   (string name, unsigned int size);
template FTGLExtrdFont&   FontHandler::font<FTGLExtrdFont>  (string name, unsigned int size);
template FTGLOutlineFont& FontHandler::font<FTGLOutlineFont>(string name, unsigned int size);
template FTGLPixmapFont&  FontHandler::font<FTGLPixmapFont> (string name, unsigned int size);
template FTGLPolygonFont& FontHandler::font<FTGLPolygonFont>(string name, unsigned int size);
template FTGLTextureFont& FontHandler::font<FTGLTextureFont>(string name, unsigned int size);
