/*
 *		namemanager Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include <algorithm>
#include "namemanager.h"


//namemanager
namemanager::~namemanager()
{
	clear();
}

bool namemanager::add(const std::string& name)
{
	if(name.empty())
		return false;

	for(auto it = _list.begin(); it != _list.end(); ++it)
	{
		if((*it) == name)
			return false;
	}

	//insert name
	_list.push_back(name);
	return true;
}

std::string namemanager::add_numbered(const std::string& name)
{
	int num = 1;
	std::string numbered = name + std::to_string(num);
			
	while(1)
	{
		auto it = _list.begin();
		for(; it != _list.end(); ++it)
		{
			if((*it) == numbered)
			{
				// increase numbered name
				numbered = name + std::to_string(++num);
				break;
			}
		}

		if(it == _list.end())
			break;
	}

	//insert numbered name
	_list.push_back(numbered);
	return numbered;
}

bool namemanager::remove(const std::string& name)
{
	if(name.empty())
		return false;

	for(auto it = _list.begin(); it != _list.end(); ++it)
	{
		if((*it) == name)
		{
			_list.erase(it);
			return true;
		}
	}

	// not found
	return false;

}

void namemanager::print(std::basic_ostream<char, std::char_traits<char> >& stream) const
{
	stream << "list:" << std::endl;
	for(auto it = _list.begin(); it != _list.end(); ++it)
	{
		stream << "\t" << *it << std::endl;
	}
}
