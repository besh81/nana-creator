/*
 *		statusbar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include "statusbar.h"
#include "style.h"


//statusbar
statusbar::statusbar(nana::window wd, bool visible)
	: nana::panel<true>(wd, visible)
{
	bgcolor(CREATOR_WINDOW_BG);

	_place.div("caption");
	_place.field("caption") << _caption;
	_place.collocate();
}


void statusbar::clear()
{
	_caption.caption("");
}


void statusbar::set(const std::string& str)
{
	_caption.caption(str);
}
