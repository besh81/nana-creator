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

	if(validation_rule_f_)
		if(!validation_rule_f_(name))
			return false;

	for(auto it = list_.begin(); it != list_.end(); ++it)
	{
		if((*it) == name)
			return false;
	}

	//insert name
	list_.push_back(name);
	return true;
}

std::string namemanager::add_numbered(const std::string& name)
{
	int num = 1;
	std::string numbered = name + std::to_string(num);
			
	while(1)
	{
		auto it = list_.begin();
		for(; it != list_.end(); ++it)
		{
			if((*it) == numbered)
			{
				// increase numbered name
				numbered = name + std::to_string(++num);
				break;
			}
		}

		if(it == list_.end())
			break;
	}

	//insert numbered name
	list_.push_back(numbered);
	return numbered;
}

bool namemanager::remove(const std::string& name)
{
	if(name.empty())
		return false;

	for(auto it = list_.begin(); it != list_.end(); ++it)
	{
		if((*it) == name)
		{
			list_.erase(it);
			return true;
		}
	}

	// not found
	return false;
}

void namemanager::print(std::basic_ostream<char, std::char_traits<char> >& stream) const
{
	stream << "list:" << std::endl;
	for(auto it = list_.begin(); it != list_.end(); ++it)
	{
		stream << "\t" << *it << std::endl;
	}
}
