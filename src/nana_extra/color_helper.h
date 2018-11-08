/*
 *		nana::color_helper Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_COLOR_HELPER_H
#define NANA_CREATOR_COLOR_HELPER_H

#include <nana/gui/wvl.hpp>


namespace nana
{
	/// helper functions
	nana::color to_color(const std::string& s, bool& inherited);
	std::string to_string(const nana::color& c, bool inherited);

	bool is_color_inherited(const std::string& s);

}//end namespace nana

#endif //NANA_CREATOR_COLOR_HELPER_H
