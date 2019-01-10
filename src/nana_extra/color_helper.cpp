/*
 *		nana::color_helper Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include <algorithm>
#include "color_helper.h"


#define	INHERITED_COLOR		"[inherited]"


namespace nana
{
	/// helper functions
	nana::color to_color(const std::string& s, bool& inherited)
	{
		std::stringstream ss(s);
		std::string item;
		std::vector<int> items;

		inherited = false;

		try
		{
			while(getline(ss, item, ','))
			{
				if(item == INHERITED_COLOR)
					inherited = true;
				else
					items.push_back(item.empty() ? 0 : std::stoi(item));
			}
		}
		catch(...)
		{
			// reset
			items.clear();
		}
		for(size_t i = 0; i < 3; ++i)
		{
			if(i >= items.size())
				items.push_back(0);
			else
				items[i] = std::max(0, std::min(items[i], 255));
		}

		return nana::color(items[0], items[1], items[2]);
	}


	nana::color to_color(const std::string& s)
	{
		bool inherited;
		return to_color(s, inherited);
	}


	std::string to_string(const nana::color& c, bool inherited)
	{
		return (inherited ? INHERITED_COLOR "," : "") + std::to_string(int(c.r())) + "," + std::to_string(int(c.g())) + "," + std::to_string(int(c.b()));
	}


	bool is_color_inherited(const std::string& s)
	{
		return s.find(INHERITED_COLOR) == 0 ? true : false;
	}
	/// helper functions - end

}//end namespace nana
