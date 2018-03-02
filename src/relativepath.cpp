/*
*		nana::relativepath Implementation
*
*      Nana C++ Library - Creator
*      Author: besh81
*/

#include <nana/gui/wvl.hpp>
#include "relativepath.h"



// equalize path characters (tolower) and slashs('/')
std::string equalize_path(const std::string& path)
{
	std::string p = path;
#if defined(NANA_WINDOWS)
	std::transform(p.begin(), p.end(), p.begin(),
		[](unsigned char c) { return std::tolower(c); }
	);
#endif

	auto found = p.find('\\');
	while(found != std::string::npos)
	{
		p[found] = '/';
		found = p.find('\\', found + 1);
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
