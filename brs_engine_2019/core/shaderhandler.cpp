#include "shaderhandler.h"

ShaderHandler::ShaderHandler()
{
}
ShaderHandler::~ShaderHandler()
{
}

void ShaderHandler::init()
{
}

void ShaderHandler::bindShader(string name)
{
    m_shaders[name]->bind();
}

void ShaderHandler::unbindShader()
{
    glUseProgramObjectARB(0);
}

Shader& ShaderHandler::getShader(string name)
{
    if (m_shaders.find(name) == m_shaders.end())
    {
        g_debug << "Cannot find shader " << name << "!" << endl;
    }
    return *m_shaders[name];
}

void ShaderHandler::loadShaders()
{
#ifdef _WIN32
    string shaderPath = "data\\shaders\\";
    string vertexDirectory = "data\\shaders\\*.vs";
    string fragmentDirectory = "data\\shaders\\*.fs";
#else
    string shaderPath = "data/shaders/";
    string vertexDirectory = "data/shaders/*.vs";
    string fragmentDirectory = "data/shaders/*.fs";
#endif

    vector<string> vertexFilenames;
    vector<string> fragmentFilenames;

    //fetch the list of shader files
    StringUtils::iterateDirectory(vertexDirectory, vertexFilenames);
    StringUtils::iterateDirectory(fragmentDirectory, fragmentFilenames);

    if (vertexFilenames.size() != fragmentFilenames.size())
    {
        g_debug << "Error! Vertex and fragment shader counts do not match!" << endl;
        g_debug << "    " << vertexFilenames.size() << " vertex and " << fragmentFilenames.size()
                << " fragment shaders" << endl;

#ifdef _MSC_VER
        for each (string s in vertexFilenames) {
#else
        //#if defined(__GXX_EXPERIMENTAL_CXX0X__)
        //                for (string& s: vertexFilenames) {
        //#else
        for (int i = 0; i < vertexFilenames.size(); i++) {
            string& s = vertexFilenames[i];
            //#error
#endif
            cout << "   vertex shader filename: " << s << endl;
        }
#ifdef _MSC_VER
        for each (string s in fragmentFilenames) {
#else
        //#if defined(__GXX_EXPERIMENTAL_CXX0X__)
        //                for (string& s: fragmentFilenames) {
        //#else
        for (int i = 0; i < fragmentFilenames.size(); i++) {
            string& s = fragmentFilenames[i];
            //#error
            //#error
#endif
            cout << "   fragment shader filename: " << s << endl;
        }
        return;
    }
    g_debug << "" << endl;
    g_debug << "loadShaders()" << endl;
    g_debug << "-------------" << endl;

    int shaderCount = (int)vertexFilenames.size();

    //load the shaders
    for (int i = 0; i < shaderCount; i++)
    {
#ifdef _WIN32
        string shadername = vertexFilenames[i].substr(0, vertexFilenames[i].length()-3);
#else
        //TODO: this is too dirty
        string shadername = vertexFilenames[i].substr(13, vertexFilenames[i].length()-13-3);
#endif
        g_debug << "shader #" << i << ": " << shadername << endl;

        //read in shader files
#ifdef _WIN32
        string vertexPath = shaderPath + vertexFilenames[i];
        string fragmentPath = shaderPath + fragmentFilenames[i];
#else
        string vertexPath = vertexFilenames[i];
        string fragmentPath = fragmentFilenames[i];
#endif

        char *vertexSource = loadShaderSource(vertexPath);
        char *fragmentSource = loadShaderSource(fragmentPath);

        //init shader and add it to the system
        if (vertexSource != 0 && fragmentSource != 0)
        {
            Shader *shader = new Shader();
            shader->init(shadername, vertexSource, fragmentSource);
            m_shaders[shadername] = shader;

            delete [] vertexSource;
            delete [] fragmentSource;
        }
        else
        {
            g_debug << "can't load " << shadername << ", not compiled, will not work, sky will fall etc" << endl;
        }
    }
    g_debug << "" << endl;
}

void ShaderHandler::freeShaders()
{
    map<string, Shader*>::iterator it;
    for (it = m_shaders.begin(); it != m_shaders.end(); it++)
    {
        Shader* s = (*it).second;
        s->release();
        delete s;
    }
    m_shaders.clear();
}



char *ShaderHandler::loadShaderSource(string filename)
{
    ifstream file(filename.c_str(), ios::in|ios::binary|ios::ate);
    //read in the shader file and pad it with zero
    if (file.is_open())
    {
        char *data = 0;
        int size = file.tellg();

        data = new char [size+1]; //one byte extra for the zero in the end
        file.seekg (0, ios::beg);
        file.read (data, size);
        file.close();
        data[size] = 0; //add null terminator so OpenGL doesn't fuck up
        return data;
    }
    else
    {
        g_debug << "Cannot open shader source file " << filename << endl;
        return 0;
    }
}
