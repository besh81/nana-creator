/*
 *		propertiespanel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PROPERTIESPANEL_H
#define NANA_CREATOR_PROPERTIESPANEL_H

#include <nana/gui/place.hpp>
#include <nana/paint/image.hpp>
#include <nana/gui/widgets/widget.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include <nana/gui/widgets/picture.hpp>
#include "ctrls/property.h"
#include "nana_extra/propertygrid.h"


class propertiespanel
	: public nana::panel<true>
{
public:
	propertiespanel(nana::window wd, bool visible = true);
	~propertiespanel() {}

	void name_changed(std::function<void(const std::string&)> f);
	void property_changed(std::function<void(const std::string&)> f)
	{
		_property_changed_f = f;
	}

	void set(ctrls::properties_collection* properties, std::vector<ctrls::properties_collection>* items);


private:
	bool					_grid_setup{ false };

	ctrls::properties_collection*	_properties;

	nana::place				_place{ *this };
	nana::paint::image		_img;

	nana::label				_caption{ *this };
	nana::picture			_pic{ *this };
	nana::label				_type_lbl{ *this, "Type" };
	nana::label				_type_txt{ *this };
	nana::label				_name_lbl{ *this, "Name" };
	nana::textbox			_name_txt{ *this };

	nana::propertygrid		_propgrid{ *this };

	std::function<void(const std::string&)> _property_changed_f;
};


#endif //NANA_CREATOR_PROPERTIESPANEL_H
