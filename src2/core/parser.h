#pragma once

#include "../globals.h"

/*
    Parser that reads the script file and does the appropriate stuff.
*/

class Parser
{
public:
    Parser();
    ~Parser();

    //parses a script and inserts it into params
    bool parse(string filename, Parameters &params, System &system);

private:
	// methods to read the file into a buffer and access the buffer
	bool readFile(string filename);
	bool getNextLine(vector<string>& tokens);
	vector<pair<int, string>> m_file;
	int m_currentLineIndex;
	int m_totalLineCount;

//    bool readLine(ifstream &stream, vector<string>&tokens);

	void error(const char *msg);

    //parse methods for individual blocks
    bool parseConfig(System &params);
    bool parseNamespace(Parameters &params);
    bool parseTexture(TextureParameters &params);
	bool parseMesh(MeshInfo &info);
	bool parseMaterial(Material &material);
	bool parseBPM(BPM &beat);
	bool parseTrigger(TriggerSystem &triggers);
	bool parseEvent(Event &e);
	bool parseRenderTarget(Image &image);
	bool parseCamera(Camera *camera);

    int m_line;
    string m_currentnamespace;

	//converts 
	Vector3 convertToVector3(string token1, string token2, string token3);

};

