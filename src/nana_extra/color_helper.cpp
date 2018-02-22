/*
 *		nana::color_helper Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include <algorithm>
#include "color_helper.h"


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
				if(item == "[inherited]")
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

	std::string to_string(const nana::color& c, bool inherited)
	{
		return (inherited ? "[inherited]," : "") + std::to_string(int(c.r())) + "," + std::to_string(int(c.g())) + "," + std::to_string(int(c.b()));
	}
	/// helper functions - end

}//end namespace nana
