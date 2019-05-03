/*
 *		filemanager Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <nana/gui/wvl.hpp> // define OS version
#include<iostream>
#include <stdio.h>  /* defines FILENAME_MAX */
#if defined(NANA_WINDOWS)
	#include <direct.h>
	#define GetCurrentDir _getcwd
	#define SetCurrentDir _chdir
    #if defined(__MINGW32__) || defined(__MINGW64__)
        #include <unistd.h>
        #include <sys/stat.h>
    #endif
#else
    #include <unistd.h>
    #include <sys/stat.h>
    #define GetCurrentDir getcwd
	#define SetCurrentDir chdir
#endif

#include "filemanager.h"


//filemanager
std::string filemanager::to_relative(const std::string& abspath)
{
	if(abspath.empty())
		return "";

	if(!relative_)
		return equalize_path(abspath);

	return get_relative_path(equalize_path(base_), equalize_path(abspath));
}


// equalize path characters (tolower) and dir separators
std::string equalize_path(const std::string& path, char div_from, char div_to)
{
	std::string p = path;

	// tolower
#if defined(NANA_WINDOWS)
	std::transform(p.begin(), p.end(), p.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
#endif

	// change dir separator
	auto found = p.find(div_from);
	while(found != std::string::npos)
	{
		p[found] = div_to;
		found = p.find(div_from, found + 1);
	}

	return p;
}


// Given an absolute directory (basedir) and an absolute path (abspath), returns the relative path
// Eg.: if basedir is c:\aaa\bbb and abspath is c:\aaa\ccc\file.txt will return ..\ccc\file.txt
std::string get_relative_path(const std::string& basedir, const std::string& abspath)
{
	// make sure the names are not too long or too short
	if(basedir.empty() || abspath.empty())
	{
		return abspath;
	}

	// handle Win names that are on different drives
	if(basedir[0] != abspath[0])
	{
		// not on the same drive, return absolute filename
		return abspath;
	}

	// find out how much of the basedir is in the abspath
	unsigned i = 1;
	while(i < basedir.size() && i < abspath.size() && basedir[i] == abspath[i])
	{
		++i;
	}
	if(i == basedir.size() && (abspath[i] == '/' || abspath[i - 1] == '/'))
	{
		// the whole basedir is in the abspath, trim off the basedir to get the relpath
		if(abspath[i] == '/')
		{
			// a directory name might have a trailing slash but a relative file name should not have a leading one
			++i;
		}
		return abspath.substr(i);
	}

	// find out how many levels deeper we are than the basedir
	int marker = i;
	int levels = 1;
	// count the number of levels we have to go up to get to the basedir
	while(i < basedir.size())
	{
		++i;
		if(basedir[i] == '/')
		{
			// make sure it's not a trailing slash
			i++;
			if(basedir[i] != '\0')
			{
				++levels;
			}
		}
	}
	// move abspath marker back to the start of the directory name that it has stopped in
	while(marker > 0 && abspath[marker - 1] != '/')
	{
		--marker;
	}

	// add the appropriate number of "../"
	std::string relpath;
	while(levels > 0)
	{
		--levels;
		relpath.append("../");
	}
	// copy the rest of the filename into the result string
	relpath.append(abspath.substr(marker));
	return relpath;
}


//get_working_dir
std::string get_working_dir()
{
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}


//set_working_dir
bool set_working_dir(const std::string& path)
{
	return SetCurrentDir(path.c_str()) == 0 ? true : false;
}


//file_exists
bool file_exists(const std::string& name)
{
    struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}
