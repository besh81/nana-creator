/*
 *		assetspanel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_ASSETSPANEL_H
#define NANA_CREATOR_ASSETSPANEL_H

#include <nana/gui/place.hpp>
#include <nana/gui/widgets/widget.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/treebox.hpp>


class assetspanel
	: public nana::panel<true>
{
public:
	assetspanel(nana::window wd, bool visible = true);
	~assetspanel() {}

	void selected(std::function<void(const std::string&)> f);
	void deselect();

private:
	nana::place			_place{ *this };

	nana::treebox		_assets{ *this };
};

#endif //NANA_CREATOR_ASSETSPANEL_H
