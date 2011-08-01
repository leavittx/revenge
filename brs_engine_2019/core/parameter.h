#pragma once

#include <string>
#include <vector>
#include <map>

#include "../globals.h"

using namespace std;

/*
	One single "parameter namespace" that contains parameters for one scene.

*/
class ParameterSpace
{
public:
	ParameterSpace();
	~ParameterSpace();

	void clear();

	//insertion methods
	void addFloat(string name, float value);
	void addInt(string name, int value);
	void addVector3(string name, Vector3& value);
	void addColor3(string name, Color3& value);
	void addColor4(string name, Color4& value);
	void addString(string name, string &value);
	void addBool(string name, bool value);
	void addBlendMode(string name, GLenum blendmode);
	void addRange(string name, Range& range);

	//accessors
	float getFloat(string name);
	int getInt(string name);
	Vector3 getVector3(string name);
	Color3 getColor3(string name);
	Color4 getColor4(string name);
	Range getRange(string name);
	string getString(string name);
	bool getBool(string name);
	GLenum getBlendMode(string name);

private:
	//these maps contain the parameters, one map per type.
	map<string, float> m_floats;
	map<string, int> m_ints;
	map<string, Vector3> m_vectors;
	map<string, Color3> m_color3s;
	map<string, Color4> m_color4s;
	map<string, string> m_strings;
	map<string, bool> m_bools;
	map<string, GLenum> m_blendModes;
	map<string, Range> m_ranges;
};

/*
	A global object that contains all the parameters read from the script file.

*/

class Parameters
{
public:
	Parameters();
	~Parameters();

	//sets the current namespace so you can reference params only by their name
	void useNamespace(string space);
	//clear it all out
	void clear();
	//write it into a file
	void save(string filename);

	//insertion
	void addFloat(string space, string name, float value);
	void addInt(string space, string name, int value);
	void addVector3(string space, string name, Vector3 &value);
	void addColor3(string space, string name, Color3 &value);
	void addColor4(string space, string name, Color4 &value);
	void addString(string space, string name, string &value);
	void addBool(string space, string name, bool value);
	void addBlendMode(string space, string name, GLenum blendmode);
	void addRange(string space, string name, Range &range);

	//accessors
	float getFloat(string space, string name);
	float getFloat(string name);
	int getInt(string space, string name);
	int getInt(string name);
	string getString(string space, string name);
	string getString(string name);
	Range getRange(string space, string name);
	Range getRange(string name);
	Vector3 getVector3(string space, string name);
	Vector3 getVector3(string name);
	Color3 getColor3(string space, string name);
	Color3 getColor3(string name);
	Color4 getColor4(string space, string name);
	Color4 getColor4(string name);
	bool getBool(string space, string name);
	bool getBool(string name);
	GLenum getBlendMode(string space, string name);
	GLenum getBlendMode(string name);

private:
	string m_currentName;
	map<string, ParameterSpace> m_params;


};


