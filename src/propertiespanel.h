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
#include "nana_extra/propgrid.h"


class propertiespanel
	: public nana::panel<true>
{
public:
	propertiespanel(nana::window wd, bool visible = true);
	~propertiespanel() {}

	void set(ctrls::properties_collection* properties);


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
};


#endif //NANA_CREATOR_PROPERTIESPANEL_H
