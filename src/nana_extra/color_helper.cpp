/*
 *		nana::color_helper Implementation
 *
 *      part of Nana Creator (https://github.com/besh81/nana-creator)
 *      Author: besh81
 */

#include <iostream>
#include <algorithm>
#include "color_helper.h"


#define	INHERITED_COLOR		"[inherited]"


namespace nana
{
	nana::color to_color(const std::string& s)
	{
		bool inherited;
		return to_color(s, inherited);
	}


	bool is_color_inherited(const std::string& s)
	{
		return s.find(INHERITED_COLOR) == 0 ? true : false;
	}


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
					items.push_back(item.empty() ? 0 : std::clamp(std::stoi(item), 0, 255));
			}
		}
		catch(...)
		{
			// reset
			items.clear();
		}

		for(size_t i = items.size(); i < 3; ++i)
			items.push_back(0);

		return nana::color(items[0], items[1], items[2]);
	}


	nana::color to_color(const std::string& r, const std::string& g, const std::string& b)
	{
		bool inherited;
		return to_color(r + "," + g + "," + b, inherited);
	}


	std::string to_string(const nana::color& c, bool inherited)
	{
		return (inherited ? INHERITED_COLOR "," : "") + std::to_string(int(c.r())) + "," + std::to_string(int(c.g())) + "," + std::to_string(int(c.b()));
	}

}//end namespace nana
