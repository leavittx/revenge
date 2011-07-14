#pragma once

#include <vector>
#include <fstream>
#include <windows.h>

using namespace std;

/*
    Debug class used for writing the log file. 

*/

class Debugger
{
public:
    Debugger(bool enabled = true);
    ~Debugger();

	void setEnabled(bool enabled);
    void flush()
    {
       m_debugFile.flush();
    }

    template<class T>
    ofstream& operator<<(const T &str)
    {
        printDate();
        m_debugFile << str;
        m_debugFile.flush();
        return m_debugFile;
    }

    template<class T>
	ofstream& operator + (const T &str)
	{
        m_debugFile << str << endl;
        m_debugFile.flush();
        return m_debugFile;
	}

private:
    void printDate();
    ofstream m_debugFile;

};

