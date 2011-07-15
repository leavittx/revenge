#pragma once

#include <map>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "../globals.h"

using namespace std;

class StringUtils
{
public:
    //convert a string to number
    template <class T> static bool convertStringToNumber(T& t, const string& s, ios_base& (*f)(ios_base&))
    {
        std::istringstream iss(s);
        return !(iss >> f >> t).fail();
    };

    //iterates a directory and gets all filenames from it into a vector
    static void iterateDirectory(string path, vector<string>& filenames, bool appendWildcard = true);

    //read a text file into buffer
    static bool readFile(string path, vector<string>& buffer, bool lowercase = false);
    //split the string into tokens
    static void tokenize(string str, vector<string> &tokens);
    //remove all instances of a character from a string
    static void removeChar(string &str, char remove);
};
