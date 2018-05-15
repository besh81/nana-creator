/*
 *		statusbar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_STATUSBAR_H
#define NANA_CREATOR_STATUSBAR_H

#include <nana/gui/place.hpp>
#include <nana/gui/widgets/widget.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/label.hpp>


class statusbar
	: public nana::panel<true>
{
public:
	statusbar(nana::window wd, bool visible = true);
	~statusbar() {}

	void clear();
	void set(const std::string& str);


private:
	nana::place		_place{ *this };

	nana::label		_caption{ *this };
};

#endif //NANA_CREATOR_STATUSBAR_H
