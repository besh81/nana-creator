/*
*      Nana C++ Library - Creator
*      Author: besh81
*/

#include <nana/gui/wvl.hpp>

#include <stdio.h>  /* defines FILENAME_MAX */
#if defined(NANA_WINDOWS)
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include<iostream>

#include "wdir.h"


std::string GetCurrentWorkingDir()
{
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}

