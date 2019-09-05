/*
 *		nana::color_helper Implementation
 *
 *      part of Nana Creator (https://github.com/besh81/nana-creator)
 *      Author: besh81
 */

#ifndef NANA_CREATOR_COLOR_HELPER_H
#define NANA_CREATOR_COLOR_HELPER_H

#include <nana/gui.hpp>


namespace nana
{
	nana::color to_color(const std::string& s);		///< Returns nana::color from the given propertygrid color string
	bool is_color_inherited(const std::string& s);	///< Returns the inherited value (true/false) from the given propertygrid color string

	nana::color to_color(const std::string& s, bool& inherited);	///< Combination of previous functions

	nana::color to_color(const std::string& r, const std::string& g, const std::string& b);	///< Returns nana::color from the given R,G,B string

	std::string to_string(const nana::color& c, bool inherited = false);	///< Returns propertygrid color string from the given nana::color and inherited state

}//end namespace nana

#endif //NANA_CREATOR_COLOR_HELPER_H
