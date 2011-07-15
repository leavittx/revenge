#if defined(_MSC_VER)
#pragma warning ( disable : 4786)
#endif

#include <string>
#include <algorithm>

#include "parser.h"

Parser::Parser()
{
}

Parser::~Parser()
{
}

//prints out an error message
void Parser::error(const char *msg)
{
    //the getNextLine() has always been called before this is, so it's safe to use the -1 indexing
    g_debug << "!! parse error on line(" << m_file[m_currentLineIndex-1].first << ") : " << string(msg) << endl;
}

bool Parser::readFile(string filename)
{
    ifstream script(filename.c_str(), ios::in);
    int linenumber = 0;

    if (script.is_open())
    {
        //clear out old file
        m_file.clear();
        bool comment = false;

        //read in new file
        while (!script.eof())
        {
            string line;
            getline(script, line);
            linenumber++;

            //skip empty lines
            if (!line.empty())
            {
                //check for a comment block
                if (line.length() > 1)
                {
                    if (line[0] == '/' && line[1] == '*') //comment starts
                    {
                        comment = true;
                        continue; //go to next line without processing
                    }
                    else if (line[0] == '*' && line[1] == '/') //comment ends
                    {
                        comment = false;
                        continue; //go to next line without processing
                    }
                }

                //check that the line is not a single comment or inside a comment block
                if (!comment && (line[0] != ';' && line[0] != '#'))
                {
                    //convert it into lowercase
//                    transform(line.begin(), line.end(), line.begin(), tolower);
                    //remove all crap that the programmer might put there by custom
                    StringUtils::removeChar(line, ';');
                    StringUtils::removeChar(line, '\t'); //remove tabs

                    //TODO: check for lines that are only whitespace
                    //push the processed line into the file
                    if (line.size() > 0)
                    {
                        m_file.push_back(pair<int, string>(linenumber, line));
                    }
                }
            }
        }
        m_totalLineCount = (int)m_file.size();
        //		g_debug << "linenumber = " << linenumber << " totalLineCount = " << m_totalLineCount << endl;
    }
    else
    {
        g_debug << "Parser::readFile() - cannot open file " << filename << " for reading!" << endl;
        return false;
    }
    script.close();

    return true;
}

bool Parser::getNextLine(vector<string>& tokens)
{
    if (m_currentLineIndex < m_totalLineCount)
    {
        //fetch the next line and split it into tokens. The tokenization is done here because it would
        //convolute the data structure further if it was done during loading. It's as fast anyway..
        tokens.clear();
        string& line = m_file[m_currentLineIndex].second;
        StringUtils::tokenize(line, tokens);
        m_currentLineIndex++;
        //		g_debug << "getNextLine : " << line << endl;
        return true;
    }
    else
    {
        //we're done
        return false;
    }
}
bool Parser::parseNamespace(Parameters &params)
{
    vector<string> tokens;

    getNextLine(tokens);
    if (tokens[0] != "{")
    {
        error("namespace must start with {");
        return false;
    }

    //read until the closing curly brace
    while (true)
    {
        tokens.clear();
        getNextLine(tokens);
        //break on ending curly brace
        if (tokens[0] == "}")
        {
            break;
        }
        //check for the equal sign that is in every namespace value
        if (tokens[2] != "=")
        {
            error("syntax error");
        }
        //read in all kinds of values
        if (tokens[0] == "float")
        {
            float value = 0.0f;

            if (StringUtils::convertStringToNumber<float>(value, tokens[3], std::dec))
            {
                if (m_currentnamespace != "")
                    params.addFloat(m_currentnamespace, tokens[1], value);
                else
                    error("trying to add parameter to an empty namespace!");
            }
            else
            {
                error("cannot parse float");
            }
        }
        else if (tokens[0] == "int")
        {
            int value = 0;

            if (StringUtils::convertStringToNumber<int>(value, tokens[3], std::dec))
            {
                if (m_currentnamespace != "")
                    params.addInt(m_currentnamespace, tokens[1], value);
                else
                    error("trying to add parameter to an empty namespace!");
            }
            else
            {
                error("cannot parse int");
            }
        }
        else if (tokens[0] == "bool")
        {
            int value = 0;

            if (m_currentnamespace != "")
            {
                if (tokens[3] == "true")
                {
                    params.addBool(m_currentnamespace, tokens[1], true);
                }
                else if (tokens[3] == "false")
                {
                    params.addBool(m_currentnamespace, tokens[1], false);
                }
                else
                {
                    error("cannot parse bool");
                }
            }
            else
            {
                error("trying to add parameter to an empty namespace!");
            }
        }
        else if (tokens[0] == "blendmode")
        {
            int value = 0;

            if (m_currentnamespace != "")
            {
                if (tokens[3] == "gl_zero")
                    params.addBlendMode(m_currentnamespace, tokens[1], GL_ZERO);
                else if (tokens[3] == "gl_one")
                    params.addBlendMode(m_currentnamespace, tokens[1], GL_ONE);
                else if (tokens[3] == "gl_dst_color")
                    params.addBlendMode(m_currentnamespace, tokens[1], GL_DST_COLOR);
                else if (tokens[3] == "gl_one_minus_dst_color")
                    params.addBlendMode(m_currentnamespace, tokens[1], GL_ONE_MINUS_DST_COLOR);
                else if (tokens[3] == "gl_src_alpha")
                    params.addBlendMode(m_currentnamespace, tokens[1], GL_SRC_ALPHA);
                else if (tokens[3] == "gl_one_minus_src_alpha")
                    params.addBlendMode(m_currentnamespace, tokens[1], GL_ONE_MINUS_SRC_ALPHA);
                else if (tokens[3] == "gl_one_minus_src_color")
                    params.addBlendMode(m_currentnamespace, tokens[1], GL_ONE_MINUS_SRC_COLOR);
                else if (tokens[3] == "gl_dst_alpha")
                    params.addBlendMode(m_currentnamespace, tokens[1], GL_DST_ALPHA);
                else if (tokens[3] == "gl_one_minus_dst_alpha")
                    params.addBlendMode(m_currentnamespace, tokens[1], GL_ONE_MINUS_DST_ALPHA);
                else if (tokens[3] == "gl_src_alpha_saturate")
                    params.addBlendMode(m_currentnamespace, tokens[1], GL_SRC_ALPHA_SATURATE);
                else
                    error("cannot parse blendmode");
            }
            else
            {
                error("trying to add parameter to an empty namespace!");
            }
        }
        else if (tokens[0] == "vector3")
        {
            float x, y, z;
            if (StringUtils::convertStringToNumber<float>(x, tokens[3], std::dec) &&
                    StringUtils::convertStringToNumber<float>(y, tokens[4], std::dec) &&
                    StringUtils::convertStringToNumber<float>(z, tokens[5], std::dec))
            {
                Vector3 v = Vector3(x, y, z);
                if (m_currentnamespace != "")
                    params.addVector3(m_currentnamespace, tokens[1], v);
                else
                    error("trying to add parameter to an empty namespace!");
            }
            else
            {
                error("cannot parse vector3");
            }
        }
        else if (tokens[0] == "range")
        {
            float minimum, maximum;
            if (StringUtils::convertStringToNumber<float>(minimum, tokens[3], std::dec) &&
                    StringUtils::convertStringToNumber<float>(maximum, tokens[4], std::dec))
            {
                Range r = Range(minimum, maximum);
                if (m_currentnamespace != "")
                    params.addRange(m_currentnamespace, tokens[1], r);
                else
                    error("trying to add parameter to an empty namespace!");
            }
            else
            {
                error("cannot parse range");
            }
        }
        else if (tokens[0] == "color3")
        {
            float r, g, b;
            if (StringUtils::convertStringToNumber<float>(r, tokens[3], std::dec) &&
                    StringUtils::convertStringToNumber<float>(g, tokens[4], std::dec) &&
                    StringUtils::convertStringToNumber<float>(b, tokens[5], std::dec))
            {
                Color3 col = Color3(r, g, b);
                if (m_currentnamespace != "")
                    params.addColor3(m_currentnamespace, tokens[1], col);
                else
                    error("trying to add parameter to an empty namespace!");
            }
            else
            {
                error("cannot parse color3");
            }
        }
        else if (tokens[0] == "color4")
        {
            float r, g, b, a;
            if (StringUtils::convertStringToNumber<float>(r, tokens[3], std::dec) &&
                    StringUtils::convertStringToNumber<float>(g, tokens[4], std::dec) &&
                    StringUtils::convertStringToNumber<float>(b, tokens[5], std::dec) &&
                    StringUtils::convertStringToNumber<float>(a, tokens[6], std::dec))
            {
                Color4 col = Color4(r, g, b, a);
                if (m_currentnamespace != "")
                    params.addColor4(m_currentnamespace, tokens[1], col);
                else
                    error("trying to add parameter to an empty namespace!");
            }
            else
            {
                error("cannot parse color4");
            }
        }
        else if (tokens[0] == "string")
        {
            if (m_currentnamespace != "")
            {
                string str = tokens[3];

                //check for multiple words. They will be separated with space.
                bool multiple = false;
                for (vector<string>::iterator it = tokens.begin() + 4; it < tokens.end(); it++)
                {
                    if (!multiple)
                    {
                        str += " "; //add space after the first word that was stripped while tokenization. Ugly..
                        multiple = true;
                    }
                    str += *it;
                    str += " ";
                }
                //take out the last space (yes, this is kind of hackish but..)
                if (multiple)
                {
                    str = str.substr(0, str.size() -1);
                }

                params.addString(m_currentnamespace, tokens[1], str);
            }
            else
                error("trying to add parameter to an empty namespace!");
        }
        else
        {
            string err = string("unknown symbol \"")+tokens[0]+string("\"");
            error(err.c_str());
        }
    }

    return true;
}
Vector3 Parser::convertToVector3(string token1, string token2, string token3)
{
    float x, y, z;
    if (StringUtils::convertStringToNumber<float>(x, token1, std::dec) &&
            StringUtils::convertStringToNumber<float>(y, token2, std::dec) &&
            StringUtils::convertStringToNumber<float>(z, token3, std::dec))
    {
        Vector3 v = Vector3(x, y, z);
        return v;
    }
    else
    {
        g_debug << "cannot parse {" << token1 << "," << token2 << "," << token3 << "} into a Vector3!" << endl;
        return Vector3(0, 0, 0);
    }
}

bool Parser::parseCamera(Camera *c)
{
    int startTime, endTime;
    CAMERA_TYPE type;

    Vector3 startPosition;
    Vector3 endPosition;
    Vector3 startTarget;
    Vector3 endTarget;
    Vector3 startUp;
    Vector3 endUp;

    CatmullRomCurve positionSpline = CatmullRomCurve();
    CatmullRomCurve targetSpline = CatmullRomCurve();

    positionSpline.begin();
    targetSpline.begin();

    vector<string> tokens;
    getNextLine(tokens);

    if (tokens[0] != "{")
    {
        error("camera must start with {");
        return false;
    }

    while (true)
    {
        tokens.clear();
        getNextLine(tokens);
        //read until the closing curly brace
        if (tokens[0] == "}")
        {
            break;
        }
        if (tokens[1] != "=")
        {
            error("syntax error");
            return false;
        }
        //read in values
        if (tokens[0] == "type")
        {
            if (tokens[2] == "linear")
            {
                type = LINEAR;
            }
            else if (tokens[2] == "catmull_rom")
            {
                //TODO
                type = CATMULL_ROM;
            }
            else
            {
                error("cannot parse camera type");
            }
        }
        else if (tokens[0] == "startposition")
        {
            startPosition = convertToVector3(tokens[2], tokens[3], tokens[4]);
        }
        else if (tokens[0] == "endposition")
        {
            endPosition = convertToVector3(tokens[2], tokens[3], tokens[4]);
        }
        else if (tokens[0] == "starttarget")
        {
            startTarget = convertToVector3(tokens[2], tokens[3], tokens[4]);
        }
        else if (tokens[0] == "endtarget")
        {
            endTarget = convertToVector3(tokens[2], tokens[3], tokens[4]);
        }
        else if (tokens[0] == "startup")
        {
            startUp = convertToVector3(tokens[2], tokens[3], tokens[4]);
        }
        else if (tokens[0] == "endup")
        {
            endUp = convertToVector3(tokens[2], tokens[3], tokens[4]);
        }
        else if (tokens[0] == "camerapoint")
        {
            positionSpline.addPoint(convertToVector3(tokens[2], tokens[3], tokens[4]));
        }
        else if (tokens[0] == "targetpoint")
        {
            targetSpline.addPoint(convertToVector3(tokens[2], tokens[3], tokens[4]));
        }
        else if (tokens[0] == "starttime")
        {
            if (StringUtils::convertStringToNumber<int>(startTime, tokens[2], std::dec))
            {
            }
            else
            {
                error("cannot parse startTime");
            }
        }
        else if (tokens[0] == "endtime")
        {
            if (StringUtils::convertStringToNumber<int>(endTime, tokens[2], std::dec))
            {
            }
            else
            {
                error("cannot parse endTime");
            }
        }

        else
        {
            string err = string("unknown symbol \"")+tokens[0]+string("\"");
            error(err.c_str());
        }
    }

    positionSpline.end();
    targetSpline.end();

    switch(type)
    {
    case NONE:
        error("camera type was not defined"); return false;

    case LINEAR:
        c->initLinear(startPosition, endPosition,
                      startTarget, endTarget,
                      startUp, endUp,
                      startTime, endTime);
        break;
    case CATMULL_ROM:
        //			error("catmull rom not yet implemented :");
        c->initCatmullRom(positionSpline, targetSpline, startTime, endTime);
        return false;
    }

    return true;
}

bool Parser::parseConfig(System &system)
{
    vector<string> tokens;

    getNextLine(tokens);
    if (tokens[0] != "{")
    {
        error("config must start with {");
        return false;
    }

    //read until the closing curly brace
    while (true)
    {
        tokens.clear();
        getNextLine(tokens);
        if (tokens[0] == "}")
        {
            break;
        }
        if (tokens[1] != "=")
        {
            error("syntax error");
            return false;
        }

        //read in values
        if (tokens[0] == "sound")
        {
            if (tokens[2] == "true")
            {
                system.setSoundEnabled(true);
            }
            else if (tokens[2] == "false")
            {
                system.setSoundEnabled(false);
            }
            else
            {
                error("cannot parse sound");
            }
        }
        else if (tokens[0] == "debug")
        {
            if (tokens[2] == "true")
            {
            }
            else if (tokens[2] == "false")
            {
            }
            else
            {
                error("cannot parse debug");
            }
        }
        else if (tokens[0] == "song")
        {
            system.setSongFilename(tokens[2]);
        }
        else if (tokens[0] == "endtime")
        {
            int time = 100;
            if (!StringUtils::convertStringToNumber<int>(time, tokens[2], std::dec))
            {
                error("cannot parse endtime number");
                time = 100; //default value
            }
            system.setEndTime(time);

        }

        else
        {
            string err = string("unknown symbol \"")+tokens[0]+string("\"");
            error(err.c_str());
        }
    }
    return true;
}

bool Parser::parseMesh(MeshInfo &info)
{
    vector<string> tokens;

    getNextLine(tokens);
    if (tokens[0] != "{")
    {
        error("mesh must start with {");
        return false;
    }

    //read until the closing curly brace
    while (true)
    {
        tokens.clear();
        getNextLine(tokens);
        if (tokens[0] == "}")
        {
            break;
        }
        if (tokens[1] != "=")
        {
            error("syntax error");
            return false;
        }

        info.setDefaults();

        //read in values
        if (tokens[0] == "usevbo")
        {
            if (tokens[2] == "true")
            {
                info.m_useVBO = true;
            }
            else if (tokens[2] == "false")
            {
                info.m_useVBO = false;
            }
            else
            {
                error("cannot parse mesh vbo");
            }
        }
        else if (tokens[0] == "retain")
        {
            if (tokens[2] == "true")
            {
                info.m_retain = true;
            }
            else if (tokens[2] == "false")
            {
                info.m_retain = false;
            }
            else
            {
                error("cannot parse mesh vbo");
            }
        }
        else if (tokens[0] == "usefacenormals")
        {
            if (tokens[2] == "true")
            {
                info.m_useFaceNormals = true;
            }
            else if (tokens[2] == "false")
            {
                info.m_useFaceNormals = false;
            }
            else
            {
                error("cannot parse mesh facenormals");
            }
        }
        else if (tokens[0] == "calculatenormals")
        {
            if (tokens[2] == "true")
            {
                info.m_calculateNormals = true;
            }
            else if (tokens[2] == "false")
            {
                info.m_calculateNormals = false;
            }
            else
            {
                error("cannot parse mesh facenormals");
            }
        }
        else if (tokens[0] == "autotexture")
        {
            if (tokens[2] == "true")
            {
                info.m_autoTexture = true;
            }
            else if (tokens[2] == "false")
            {
                info.m_autoTexture = false;
            }
            else
            {
                error("cannot parse mesh autotexture");
            }
        }
        else if (tokens[0] == "texture")
        {
            info.m_texture = tokens[2];
        }
        else
        {
            string err = string("unknown symbol \"")+tokens[0]+string("\"");
            error(err.c_str());
        }
    }

    return true;
}



bool Parser::parseMaterial(Material &material)
{
    vector<string> tokens;

    getNextLine(tokens);
    if (tokens[0] != "{")
    {
        error("material must start with {");
        return false;
    }
    material.setDefaults();

    //read until the closing curly brace
    while (true)
    {
        tokens.clear();
        getNextLine(tokens);
        if (tokens[0] == "}")
        {
            break;
        }
        if (tokens[1] != "=")
        {
            error("syntax error");
            return false;
        }

        //read in values
        if (tokens[0] == "diffuse")
        {
            float r, g, b;
            if (StringUtils::convertStringToNumber<float>(r, tokens[2], std::dec) &&
                    StringUtils::convertStringToNumber<float>(g, tokens[3], std::dec) &&
                    StringUtils::convertStringToNumber<float>(b, tokens[4], std::dec))
            {
                material.setDiffuse(Color3(r, g, b));
            }
            else
            {
                error("cannot parse material diffuse");
            }
        }
        else if (tokens[0] == "specular")
        {
            float r, g, b;
            if (StringUtils::convertStringToNumber<float>(r, tokens[2], std::dec) &&
                    StringUtils::convertStringToNumber<float>(g, tokens[3], std::dec) &&
                    StringUtils::convertStringToNumber<float>(b, tokens[4], std::dec))
            {
                material.setSpecular(Color3(r, g, b));
            }
            else
            {
                error("cannot parse material specular");
            }
        }
        else if (tokens[0] == "ambient")
        {
            float r, g, b;
            if (StringUtils::convertStringToNumber<float>(r, tokens[2], std::dec) &&
                    StringUtils::convertStringToNumber<float>(g, tokens[3], std::dec) &&
                    StringUtils::convertStringToNumber<float>(b, tokens[4], std::dec))
            {
                material.setAmbient(Color3(r, g, b));
            }
            else
            {
                error("cannot parse material ambient");
            }
        }
        else if (tokens[0] == "shininess")
        {
            float s;
            if (StringUtils::convertStringToNumber<float>(s, tokens[2], std::dec))
            {
                material.setShininess(s);
            }
        }
        else
        {
            string err = string("unknown symbol \"")+tokens[0]+string("\"");
            error(err.c_str());
        }
    }

    return true;
}






bool Parser::parseTexture(TextureParameters &params)
{
    vector<string> tokens;

    getNextLine(tokens);
    if (tokens[0] != "{")
    {
        error("config must start with {");
        return false;
    }

    //read until the closing curly brace
    while (true)
    {
        tokens.clear();
        getNextLine(tokens);
        if (tokens[0] == "}")
        {
            break;
        }
        if (tokens[1] != "=")
        {
            error("syntax error");
            return false;
        }
        if (tokens[0][0] == ';')
            continue;

        params.setDefaults();

        //read in values
        if (tokens[0] == "upload")
        {
            if (tokens[2] == "true")
            {
                params.m_upload = true;
            }
            else if (tokens[2] == "false")
            {
                params.m_upload = false;
            }
            else
            {
                error("cannot parse texture upload");
            }
        }
        else if (tokens[0] == "retain")
        {
            if (tokens[2] == "true")
            {
                params.m_retain = true;
            }
            else if (tokens[2] == "false")
            {
                params.m_retain = false;
            }
            else
            {
                error("cannot parse texture retain");
            }
        }
        else if (tokens[0] == "filter")
        {
            if (tokens[2] == "linear")
            {
                params.m_linear = true;
            }
            else if (tokens[2] == "nearest")
            {
                params.m_linear = false;
            }
            else
            {
                error("cannot parse texture filter");
            }
        }
        else if (tokens[0] == "wrap")
        {
            if (tokens[2] == "repeat")
            {
                params.m_repeat = true;
            }
            else if (tokens[2] == "clamp")
            {
                params.m_repeat = false;
            }
            else
            {
                error("cannot parse texture wrap mode");
            }
        }
        else if (tokens[0] == "mipmap")
        {
            if (tokens[2] == "true")
            {
                params.m_mipmap = true;
            }
            else if (tokens[2] == "false")
            {
                params.m_mipmap = false;
            }
            else
            {
                error("cannot parse texture mipmap ");
            }
        }
        else
        {
            string err = string("unknown symbol \"")+tokens[0]+string("\"");
            error(err.c_str());
        }
    }

    return true;
}

bool Parser::parseRenderTarget(Image &image)
{
    vector<string> tokens;

    int width = 0;
    int height = 0;

    getNextLine(tokens);
    if (tokens[0] != "{")
    {
        error("Rendertarget must start with {");
        return false;
    }

    //read until the closing curly brace
    while (true)
    {
        tokens.clear();
        getNextLine(tokens);
        if (tokens[0] == "}")
        {
            break;
        }
        if (tokens[1] != "=")
        {
            error("syntax error");
            return false;
        }

        //read in values
        if (tokens[0] == "width")
        {
            if (StringUtils::convertStringToNumber<int>(width, tokens[2], std::dec))
            {
            }
            else
            {
                error("cannot parse rendertarget width!");
            }
        }
        else if (tokens[0] == "height")
        {
            if (StringUtils::convertStringToNumber<int>(height, tokens[2], std::dec))
            {
            }
            else
            {
                error("cannot parse rendertarget height!");
            }
        }
        else
        {
            string err = string("unknown symbol \"")+tokens[0]+string("\"");
            error(err.c_str());
        }
    }
    if (height > 0 && width > 0)
    {
        Image *temp = ImageFactory::createEmpty(width, height);
        image = *temp;
    }
    else
    {
        error("rendertarget definition incomplete!");
    }

    return true;
}

bool Parser::parseTrigger(TriggerSystem &triggers)
{
    vector<string> tokens;

    getNextLine(tokens);
    if (tokens[0] != "{")
    {
        error("BPM must start with {");
        return false;
    }

    //read until the closing curly brace
    while (true)
    {
        tokens.clear();
        getNextLine(tokens);
        if (tokens[0] == "}")
        {
            break;
        }
        Trigger trigger;

        if (StringUtils::convertStringToNumber<int>(trigger.m_time, tokens[0], std::dec))
        {
        }
        else
        {
            error("cannot parse trigger time!");
        }
        if (StringUtils::convertStringToNumber<int>(trigger.m_length, tokens[1], std::dec))
        {
        }
        else
        {
            error("cannot parse trigger length!");
        }
        triggers.addTrigger(trigger);
    }
    return true;
}

bool Parser::parseBPM(BPM &beat)
{
    vector<string> tokens;

    float bpm = 0;
    int offset = 0;

    getNextLine(tokens);
    //first non-empty line
    if (tokens[0] != "{")
    {
        error("BPM must start with {");
        return false;
    }

    //read until the closing curly brace
    while (true)
    {
        tokens.clear();
        getNextLine(tokens);
        if (tokens[0] == "}")
        {
            break;
        }
        if (tokens[1] != "=")
        {
            error("syntax error");
            return false;
        }
        //read in values
        if (tokens[0] == "bpm")
        {
            if (StringUtils::convertStringToNumber<float>(bpm, tokens[2], std::dec))
            {
                beat.setBPM(bpm);
            }
            else
            {
                error("cannot parse BPM value!");
            }
        }
        else if (tokens[0] == "offset")
        {
            if (StringUtils::convertStringToNumber<int>(offset, tokens[2], std::dec))
            {
                beat.setOffset(offset);
            }
            else
            {
                error("cannot parse BPM offset!");
            }
        }
        else
        {
            string err = string("unknown symbol \"")+tokens[0]+string("\"");
            error(err.c_str());
        }
    }
    return true;
}


bool Parser::parseEvent(Event &e)
{
    vector<string> tokens;

    int time = 0;
    int length = 0;

    getNextLine(tokens);

    //first non-empty line
    if (tokens[0] != "{")
    {
        error("Event must start with {");
        return false;
    }

    //read until the closing curly brace
    while(true)
    {
        tokens.clear();
        getNextLine(tokens);
        if (tokens[0] == "}")
        {
            break;
        }
        if (tokens[1] != "=")
        {
            error("syntax error");
            return false;
        }

        //read in values
        if (tokens[0] == "time")
        {
            if (StringUtils::convertStringToNumber<int>(time, tokens[2], std::dec))
            {
                e.m_time = time;
                //g_debug << "parsed event time = " << e.m_time << "\n";
            }
            else
            {
                error("cannot parse event time!");
            }
        }
        else if (tokens[0] == "length")
        {
            if (StringUtils::convertStringToNumber<int>(length, tokens[2], std::dec))
            {
                e.m_length = length;
            }
            else
            {
                error("cannot parse event value!");
            }
        }
        else
        {
            string err = string("unknown symbol \"")+tokens[0]+string("\"");
            error(err.c_str());
        }
    }
    return true;
}



bool Parser::parse(string filename, Parameters &params, System &system)
{
    g_debug << "loading script " << filename << endl;

    if (readFile(filename))
    {
        //file was found and read into the memory, so it can be processed
        m_currentLineIndex = 0;

        system.clear();
        params.clear();

        vector<string> tokens;
        while (getNextLine(tokens))
        {
            //the ugly If-Else of Doom...
            //only rudimentary error checking implemented, but it's obvious of something goes wrong >;)
            if (tokens[0] == "namespace")
            {
                if (tokens.size() != 2)
                {
                    error("namespace declaration must have exactly 2 tokens");
                }
                else
                {
//                    transform(tokens[1].begin(), tokens[1].end(), tokens[1].begin(), tolower);
                    m_currentnamespace = tokens[1];
                    parseNamespace(params);
                }
            }
            else if (tokens[0] == "config")
            {
                parseConfig(system);
            }
            else if (tokens[0] == "texture")
            {
                TextureParameters* textureparams = new TextureParameters();
                parseTexture(*textureparams);
                g_textures.addTextureParameters(tokens[1], textureparams);
            }
            else if (tokens[0] == "mesh")
            {
                MeshInfo *meshinfo = new MeshInfo();
                parseMesh(*meshinfo);
                system.addMeshInfo(tokens[1], meshinfo);
            }
            else if (tokens[0] == "material")
            {
                Material *material = new Material();
                parseMaterial(*material);
                system.addMaterial(tokens[1], material);
            }
            else if (tokens[0] == "rendertarget")
            {
                Image *image = new Image();
                parseRenderTarget(*image);
                g_textures.addImage(tokens[1], image);
            }
            else if (tokens[0] == "bpm")
            {
                BPM* bpm = new BPM();
                bpm->setName(tokens[1]);
                parseBPM(*bpm);
                system.addBeat(tokens[1], bpm);
            }
            else if (tokens[0] == "trigger")
            {
                TriggerSystem *triggers = new TriggerSystem();
                parseTrigger(*triggers);
                system.addTriggers(tokens[1], triggers);
            }
            else if (tokens[0] == "event")
            {
                Event *e = new Event();
                parseEvent(*e);
                system.addEvent(tokens[1], e);
            }
            else if (tokens[0] == "camera")
            {
                Camera *c = new Camera();
                parseCamera(c);
                system.addCamera(tokens[1], c);
            }
            else
            {
                error("unknown block");
            }
        }
    }
    else
    {
        g_debug << "cannot open script file " << filename << " for reading!" << endl;
    }
    g_debug << "Parsing done!" << endl;
    return true;
}
