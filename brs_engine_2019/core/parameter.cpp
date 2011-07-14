#pragma warning ( disable : 4786)
#include "parameter.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                        ParameterSpace
///////////////////////////////////////////////////////////////////////////////////////////////////

ParameterSpace::ParameterSpace()
{
}

ParameterSpace::~ParameterSpace()
{
}

void ParameterSpace::clear()
{
    m_floats.clear();
    m_ints.clear();
    m_vectors.clear();
    m_color3s.clear();
    m_color4s.clear();
    m_strings.clear();
	m_bools.clear();
	m_blendModes.clear();
	m_ranges.clear();
	
}
Vector3 ParameterSpace::getVector3(string name)
{
    if (m_vectors.find(name) == m_vectors.end())
    {
        g_debug << "trying to get Vector3 \"" << name << "\" from params though it doesn't exist!" << endl;
        return Vector3(0, 0, 0);
    }
    else
    {
        return m_vectors[name];
    }
}
Color3 ParameterSpace::getColor3(string name)
{
    if (m_color3s.find(name) == m_color3s.end())
    {
        g_debug << "trying to get Color3 \"" << name << "\" from params though it doesn't exist!" << endl;
        return Color3(0, 0, 0);
    }
    else
    {
        return m_color3s[name];
    }
}
Color4 ParameterSpace::getColor4(string name)
{
    if (m_color4s.find(name) == m_color4s.end())
    {
        g_debug << "trying to get Color4 \"" << name << "\" from params though it doesn't exist!" << endl;
        return Color4(0, 0, 0, 0);
    }
    else
    {
        return m_color4s[name];
    }
}

float ParameterSpace::getFloat(string name)
{
    if (m_floats.find(name) == m_floats.end())
    {
        g_debug << "trying to get float \"" << name << "\" from params though it doesn't exist!" << endl;
        return 0.0f;
    }
    else
    {
        return m_floats[name];
    }
}

Range ParameterSpace::getRange(string name)
{
	if (m_ranges.find(name) == m_ranges.end())
	{
		g_debug << "trying to get range \"" << name << "\" from params though it doesn't exist!" << endl;
		return Range(0, 0);
	}
	else
	{
		return m_ranges[name];
	}
}
int ParameterSpace::getInt(string name)
{
    if (m_ints.find(name) == m_ints.end())
    {
        g_debug << "trying to get int \"" << name << "\" from params though it doesn't exist!" << endl;
        return 0;
    }
    else
    {
        return m_ints[name];
    }
}
GLenum ParameterSpace::getBlendMode(string name)
{
    if (m_blendModes.find(name) == m_blendModes.end())
    {
        g_debug << "trying to get blendmode \"" << name << "\" from params though it doesn't exist!" << endl;
        return 0;
    }
    else
    {
        return m_blendModes[name];
    }
}

string ParameterSpace::getString(string name)
{
    if (m_strings.find(name) == m_strings.end())
    {
        g_debug << "trying to get string \"" << name << "\" from params though it doesn't exist!" << endl;
        return "";
    }
    else
    {
        return m_strings[name];
    }
}
bool ParameterSpace::getBool(string name)
{
    if (m_bools.find(name) == m_bools.end())
    {
        g_debug << "trying to get bool \"" << name << "\" from params though it doesn't exist!" << endl;
        return "";
    }
    else
    {
        return m_bools[name];
    }
}

void ParameterSpace::addFloat(string name, float value)
{
    if (m_floats.find(name) != m_floats.end())
    {
        g_debug << "trying to add float \"" << name << "\" with value " << value << " into params though it already exists!" << endl;
    }
    else
    {
        m_floats[name] = value;
    }
}
void ParameterSpace::addInt(string name, int value)
{
    if (m_ints.find(name) != m_ints.end())
    {
        g_debug << "trying to add int \"" << name << "\" with value " << value << " into params though it already exists!" << endl;
    }
    else
    {
        m_ints[name] = value;
    }
}
void ParameterSpace::addBlendMode(string name, GLenum value)
{
    if (m_blendModes.find(name) != m_blendModes.end())
    {
        g_debug << "trying to add int \"" << name << "\" with value " << value << " into params though it already exists!" << endl;
    }
    else
    {
        m_blendModes[name] = value;
    }
}
void ParameterSpace::addVector3(string name, Vector3 &value)
{
    if (m_vectors.find(name) != m_vectors.end())
    {
        g_debug << "trying to add vector3 \"" << name << "\" into params though it already exists!" << endl;
    }
    else
    {
        m_vectors[name] = value;
    }
}
void ParameterSpace::addRange(string name, Range &value)
{
    if (m_ranges.find(name) != m_ranges.end())
    {
        g_debug << "trying to add range \"" << name << "\" into params though it already exists!" << endl;
    }
    else
    {
        m_ranges[name] = value;
    }
}
void ParameterSpace::addColor3(string name, Color3 &value)
{
    if (m_color3s.find(name) != m_color3s.end())
    {
        g_debug << "trying to add color3 \"" << name << "\" into params though it already exists!" << endl;
    }
    else
    {
        m_color3s[name] = value;
    }
}
void ParameterSpace::addColor4(string name, Color4 &value)
{
    if (m_color4s.find(name) != m_color4s.end())
    {
        g_debug << "trying to add color4 \"" << name << "\" into params though it already exists!" << endl;
    }
    else
    {
        m_color4s[name] = value;
    }
}

void ParameterSpace::addString(string name, string &value)
{
    if (m_strings.find(name) != m_strings.end())
    {
        g_debug << "trying to add string  \"" << name << "\" into params though it already exists!" << endl;
    }
    else
    {
        m_strings[name] = value;
    }
}
void ParameterSpace::addBool(string name, bool value)
{
    if (m_bools.find(name) != m_bools.end())
    {
        g_debug << "trying to add color4 \"" << name << "\" into params though it already exists!" << endl;
    }
    else
    {
        m_bools[name] = value;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                        Parameters
///////////////////////////////////////////////////////////////////////////////////////////////////

Parameters::Parameters()
{
    m_currentName = "";
}

Parameters::~Parameters()
{
}

//utils
void Parameters::clear()
{
    map<string, ParameterSpace>::iterator it;
    for (it = m_params.begin(); it != m_params.end(); it++)
    {
        ParameterSpace &s = (*it).second;
        s.clear();
    }
}

void Parameters::save(string filename)
{
}
void Parameters::useNamespace(string space)
{
	//force lowercase
	transform(space.begin(), space.end(), space.begin(), tolower);
    m_currentName = space;
}

//insertion
void Parameters::addFloat(string space, string name, float value)
{
    ParameterSpace &s = m_params[space];
    s.addFloat(name, value);
}
void Parameters::addBool(string space, string name, bool value)
{
    ParameterSpace &s = m_params[space];
    s.addBool(name, value);
}
void Parameters::addBlendMode(string space, string name, GLenum value)
{
    ParameterSpace &s = m_params[space];
	s.addBlendMode(name, value);
}
void Parameters::addInt(string space, string name, int value)
{
    ParameterSpace &s = m_params[space];
    s.addInt(name, value);
}
void Parameters::addRange(string space, string name, Range &value)
{
    ParameterSpace &s = m_params[space];
    s.addRange(name, value);
}
void Parameters::addVector3(string space, string name, Vector3 &value)
{
    ParameterSpace &s = m_params[space];
    s.addVector3(name, value);
}
void Parameters::addColor3(string space, string name, Color3 &value)
{
    ParameterSpace &s = m_params[space];
    s.addColor3(name, value);
}
void Parameters::addColor4(string space, string name, Color4 &value)
{
    ParameterSpace &s = m_params[space];
    s.addColor4(name, value);
}
void Parameters::addString(string space, string name, string &value)
{
    ParameterSpace &s = m_params[space];
    s.addString(name, value);
}

//get float
float Parameters::getFloat(string space, string name)
{
    ParameterSpace &s = m_params[space];
    return s.getFloat(name);
}
float Parameters::getFloat(string name)
{
    return getFloat(m_currentName, name);
}

//get int
int Parameters::getInt(string space, string name)
{
    ParameterSpace &s = m_params[space];
    return s.getInt(name);
}
int Parameters::getInt(string name)
{
    return getInt(m_currentName, name);
}
//get getblendmode
GLenum Parameters::getBlendMode(string space, string name)
{
    ParameterSpace &s = m_params[space];
    return s.getBlendMode(name);
}
GLenum Parameters::getBlendMode(string name)
{
    return getBlendMode(m_currentName, name);
}

//get bool
bool Parameters::getBool(string space, string name)
{
    ParameterSpace &s = m_params[space];
    return s.getBool(name);
}
bool Parameters::getBool(string name)
{
    return getBool(m_currentName, name);
}

//get vector3
Vector3 Parameters::getVector3(string space, string name)
{
    ParameterSpace &s = m_params[space];
    return s.getVector3(name);
}
Vector3 Parameters::getVector3(string name)
{
    return getVector3(m_currentName, name);
}

//get range
Range Parameters::getRange(string space, string name)
{
    ParameterSpace &s = m_params[space];
    return s.getRange(name);
}
Range Parameters::getRange(string name)
{
    return getRange(m_currentName, name);
}

//get color3
Color3 Parameters::getColor3(string space, string name)
{
    ParameterSpace &s = m_params[space];
    return s.getColor3(name);
}
Color3 Parameters::getColor3(string name)
{
    return getColor3(m_currentName, name);
}

//get color4
Color4 Parameters::getColor4(string space, string name)
{
    ParameterSpace &s = m_params[space];
    return s.getColor4(name);
}
Color4 Parameters::getColor4(string name)
{
    return getColor4(m_currentName, name);
}
//get color4
string Parameters::getString(string space, string name)
{
    ParameterSpace &s = m_params[space];
    return s.getString(name);
}
string Parameters::getString(string name)
{
    return getString(m_currentName, name);
}
