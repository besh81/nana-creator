/*
 *		imagemanager Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include <algorithm>
#include "imagemanager.h"


//imagemanager
imagemanager::~imagemanager()
{
	clear();
}

bool imagemanager::add(const std::string& name, const std::string& path)
{
	if(name.empty() || path.empty())
		return false;

	for(auto it = _list.begin(); it != _list.end(); ++it)
	{
		if(it->name == name)
			return false;
	}

	// insert
	_list.push_back({ name, path });
	return true;
}

bool imagemanager::remove(const std::string& name)
{
	if(name.empty())
		return false;

	for(auto it = _list.begin(); it != _list.end(); ++it)
	{
		if(it->name == name)
		{
			_list.erase(it);
			return true;
		}
	}

	// not found
	return false;
}

std::string imagemanager::name(unsigned pos)
{
	if(pos >= _list.size())
		return "";

	return _list[pos].name;
}

std::string imagemanager::path(unsigned pos)
{
	if(pos >= _list.size())
		return "";

	return _list[pos].path;
}

std::string imagemanager::path(const std::string& name)
{
	if(name.empty())
		return "";

	for(auto it = _list.begin(); it != _list.end(); ++it)
	{
		if(it->name == name)
		{
			return it->path;
		}
	}

	// not found
	return "";
}
