/*
 *		nana::color_helper Implementation
 *
 *      part of Nana Creator (https://github.com/besh81/nana-creator)
 *      Author: besh81
 */

#ifndef NANA_CREATOR_COLOR_HELPER_H
#define NANA_CREATOR_COLOR_HELPER_H

#include <nana/gui/wvl.hpp>


namespace nana
{
	/// helper functions
	nana::color to_color(const std::string& s, bool& inherited);
	nana::color to_color(const std::string& s);

	nana::color to_color(const std::string& r, const std::string& g, const std::string& b, bool& inherited);
	nana::color to_color(const std::string& r, const std::string& g, const std::string& b);

	std::string to_string(const nana::color& c, bool inherited = false);

	bool is_color_inherited(const std::string& s);

}//end namespace nana

#endif //NANA_CREATOR_COLOR_HELPER_H
