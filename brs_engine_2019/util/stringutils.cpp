#include "stringutils.h"

//splits a string into tokens
void StringUtils::tokenize(string str, vector<string> &tokens)
{
    stringstream ss(str);
    string buffer;
    while (ss >> buffer)
    {
        tokens.push_back(buffer);
    }
}

void StringUtils::iterateDirectory(string path, vector<string>& filenames, bool appendWildcard)
{
	filenames.clear();
	//this is a bastardized MSDN example
	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	//find the first file in the directory and append the mask with '*' to get all files
	if (appendWildcard)
	{
		hFind = FindFirstFile((path+"*").c_str(), &ffd);
	}
	else
	{
		hFind = FindFirstFile(path.c_str(), &ffd);
	}

	if (INVALID_HANDLE_VALUE == hFind) 
	{
 		return;
	} 

	// List all the files in the directory with some info about them.

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//directory
		}
		else
		{
			//file
			string filename = string(ffd.cFileName);
			//convert to lower case
			transform(filename.begin(), filename.end(), filename.begin(), tolower);
 			filenames.push_back(filename);
		}
	}
	while (FindNextFile(hFind, &ffd) != 0);

	FindClose(hFind);
}

//removes all instances of a character
void StringUtils::removeChar(string &str, char remove)
{
	string::iterator it;
    for (it = str.begin(); it < str.end(); )
    {
        if (*it == remove)
        {
            it = str.erase(it);
        }
        else
        {
            it++;
        }
    }
}


bool StringUtils::readFile(string path, vector<string>& buffer, bool lowercase)
{
    ifstream file(path.c_str(), ios::in);
	if (file.is_open())
	{
		while (!file.eof())
		{
			string line;
			getline(file, line);
			if (lowercase)
			{
				transform(line.begin(), line.end(), line.begin(), tolower);
			}
			buffer.push_back(line);
		}
		file.close();
		return true;
	}
	else
	{
		g_debug << "Could not open file " << path << "for reading" << endl;
		return false;
	}
}