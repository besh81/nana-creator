/*
 *		propertiespanel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "propertiespanel.h"
#include "ctrls/panel.h"
#include "propertygrid_helper.h"
#include "imagemanager.h"
#include "lock_guard.h"
#include "style.h"


extern imagemanager		g_img_mgr;


//propertiespanel
propertiespanel::propertiespanel(nana::window wd, bool visible)
	: nana::panel<true>(wd, visible)
{
	bgcolor(CREATOR_WINDOW_BG);

	_place.div("vertical <weight=20 caption><margin=2 vertical <weight=42 <margin=10 weight=42 icon><vertical <weight=21 type arrange=[50,variable]><weight=21 name arrange=[50,variable]>>><margin=[5] propgrid>>");
	

	//TEMP caption
	_caption.bgcolor(CREATOR_WINDOW_CAP_BG);
	_caption.fgcolor(CREATOR_WINDOW_CAP_FG);
	_caption.caption("  Properties");
	_caption.text_align(nana::align::left, nana::align_v::center);
	_place.field("caption") << _caption;


	_pic.bgcolor(CREATOR_WINDOW_BG);
	_place.field("icon") << _pic;

	_type_lbl.bgcolor(CREATOR_WINDOW_BG);
	_type_txt.bgcolor(CREATOR_WINDOW_BG);
	_place.field("type") << _type_lbl << _type_txt;

	_name_lbl.bgcolor(CREATOR_WINDOW_BG);
	_name_txt.multi_lines(false);
	_place.field("name") << _name_lbl << _name_txt;

	_place.field("propgrid") << _propgrid;


	_place.collocate();


	// events
	_propgrid.events().property_changed([this](const nana::arg_propertygrid& arg)
	{
		if(_grid_setup)
			return;

		auto cat = _propgrid.at(arg.item.pos().cat);

		for(size_t i = 0; i < _properties->count(); ++i)
		{
			auto pi = (*_properties)[i];
			if(arg.item.label() == pi.label() && cat.text() == pi.category())
			{
				pi.value(arg.item.value());

				// look for properties bonds
				propertygrid_helper::enabled_bonds(&_propgrid, _properties, pi.name(), pi.as_bool());

				if(_property_changed_f)
					_property_changed_f(pi.name());
				break;
			}
		}
	});
}


void propertiespanel::name_changed(std::function<void(const std::string&)> f)
{
	// events
	_name_txt.events().key_press([this, f](const nana::arg_keyboard& arg)
	{
		if(arg.key == nana::keyboard::enter && _properties)
		{
			_name_txt.edited_reset();
			f(_name_txt.caption());
		}
	});
	_name_txt.events().focus([this, f](const nana::arg_focus& arg)
	{
		if(!arg.getting)
		{
			// just lost focus, so capture the value left by the user, if changed
			if(_name_txt.edited() && _properties)
			{
				_name_txt.edited_reset();
				f(_name_txt.caption());
			}
		}
	});
}


void propertiespanel::set(ctrls::properties_collection* properties, std::vector<ctrls::properties_collection>* items)
{
	if(_properties == properties)
		return;


	lock_guard grid_change(&_grid_setup, true);

	_properties = properties;

	_img.close();
	_propgrid.erase();


	if(!_properties)
	{
		_pic.load(nana::paint::image(""));
		_type_txt.caption("");
		_name_txt.editable(false);
		_name_txt.caption("");
		_name_txt.edited_reset();

		_place.collocate();
		return;
	}


	// add properties
	propertygrid_helper::append(&_propgrid, _properties, items);
	

	// set image, type and name
	_img.open(g_img_mgr.path(_properties->property("type").as_string()));
	_pic.load(_img);
	_type_txt.caption(_properties->property("type").as_string());
	_name_txt.editable(true);
	_name_txt.caption(_properties->property("name").as_string());
	_name_txt.edited_reset();

	_place.collocate();
}

