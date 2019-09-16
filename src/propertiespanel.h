/*****************************************************
 *	C++ code generated with Nana Creator (0.22.0)
 *	GitHub repo: https://github.com/besh81/nana-creator
 *
 * PLEASE EDIT ONLY INSIDE THE TAGS:
 *		//<*tag
 *			user code
 *		//*>
*****************************************************/

#ifndef PROPERTIESPANEL_H
#define PROPERTIESPANEL_H

#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/picture.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/textbox.hpp>
#include "nana_extra/propertygrid.h"

//<*includes
#include "ctrls/property.h"
//*>


class propertiespanel
	: public nana::panel<true>
{
public:
	propertiespanel(nana::window wd, bool visible = true)
		: nana::panel<true>(wd, visible)
	{
		init_();

		//<*ctor
		_init_ctrls();
		//*>
	}

	~propertiespanel()
	{
		//<*dtor

		//*>
	}


private:
	void init_()
	{
		_place.div("vert <vert margin=2 <weight=42 margin=2 <weight=42 margin=5 icon><vert <gap=10 arrange=[45,variable] type><gap=10 arrange=[45,variable] name>>><margin=2 propgrid>>");
		bgcolor(nana::color(204,213,240));
		// _pic
		_pic.create(*this);
		_place["icon"] << _pic;
		_pic.bgcolor(this->bgcolor());
		_pic.align(static_cast<nana::align>(1), static_cast<nana::align_v>(1));
		// _type_lbl
		_type_lbl.create(*this);
		_place["type"] << _type_lbl;
		_type_lbl.caption("Type");
		_type_lbl.transparent(true);
		_type_lbl.text_align(static_cast<nana::align>(2), static_cast<nana::align_v>(1));
		// _type
		_type.create(*this);
		_place["type"] << _type;
		_type.caption("");
		_type.transparent(true);
		_type.text_align(static_cast<nana::align>(0), static_cast<nana::align_v>(1));
		// _name_lbl
		_name_lbl.create(*this);
		_place["name"] << _name_lbl;
		_name_lbl.caption("Name");
		_name_lbl.transparent(true);
		_name_lbl.text_align(static_cast<nana::align>(2), static_cast<nana::align_v>(1));
		// _name
		_name.create(*this);
		_place["name"] << _name;
		_name.multi_lines(false);
		// _propgrid
		_propgrid.create(*this);
		_place["propgrid"] << _propgrid;

		_place.collocate();
	}


protected:
	nana::place _place{ *this };
	nana::picture _pic;
	nana::label _type_lbl;
	nana::label _type;
	nana::label _name_lbl;
	nana::textbox _name;
	nana::propertygrid _propgrid;


	//<*declarations
public:
	void name_changed(std::function<void(const std::string&)> f);
	void property_changed(std::function<void(const std::string&)> f)
	{
		_property_changed_f = f;
	}

	void set(ctrls::properties_collection* properties, std::vector<ctrls::properties_collection>* items);


private:
	void _init_ctrls();

	bool _grid_setup{ false };

	ctrls::properties_collection* _properties;
	nana::paint::image _img;

	std::function<void(const std::string&)> _property_changed_f;
	//*>
};

#endif //PROPERTIESPANEL_H

