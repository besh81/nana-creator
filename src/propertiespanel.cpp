/*
 *		propertiespanel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include "propertiespanel.h"
#include "ctrls/layout.h"
#include "ctrls/panel.h"
#include "pg_items.h"
#include "guimanager.h"
#include "imagemanager.h"
#include "lock_guard.h"
#include "style.h"


extern guimanager		g_gui_mgr;
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
	_caption.caption("Properties");
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
	_name_txt.events().key_press([this](const nana::arg_keyboard& arg)
	{
		if(arg.key == nana::keyboard::enter && _properties)
			g_gui_mgr.updateselectedname(_name_txt.caption());
	});

	_propgrid.events().property_changed([this](const nana::arg_propertygrid& arg)
	{
		if(_grid_setup)
			return;

		for(size_t i = 0; i < _properties->count(); ++i)
		{
			auto pi = (*_properties)[i];
			if(arg.item.label() == pi.label())
			{
				pi.value(arg.item.value());

				// look for properties bonds
				enabled_bonds(pi.name(), pi.as_bool());

				g_gui_mgr.updateselected();
				break;
			}
		}
	});
}


void propertiespanel::set(ctrls::properties_collection* properties)
{
	if(_properties == properties)
		return;


	lock_guard grid_change(&_grid_setup, true);

	_properties = properties;

	_img.close();
	_propgrid.erase();


	if(!_properties)
	{
		_type_txt.caption("");
		_name_txt.editable(false);
		_name_txt.caption("");

		_place.collocate();
		return;
	}


	// add properties
	for(size_t i = 0; i < _properties->count(); ++i)
	{
		auto prop = (*_properties)[i];
		if(prop.label().empty())
			continue;

		auto cat_idx = _propgrid.find(prop.category());
		auto cat = (cat_idx == nana::npos) ? _propgrid.append(prop.category()) : _propgrid.at(cat_idx);

		if(prop.type() == ctrls::pg_type::string_int)
		{
			cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string_int(prop.label(), prop.value())));
		}
		else if(prop.type() == ctrls::pg_type::string_uint || prop.type() == ctrls::pg_type::string_uint_0_100)
		{
			auto item = cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string_uint(prop.label(), prop.value())));

			if(prop.type() == ctrls::pg_type::string_uint_0_100)
			{
				auto psui = static_cast<nana::pg_string_uint*>(item._m_pgitem());
				psui->range(0, 100);
			}
		}
		else if(prop.type() == ctrls::pg_type::check)
		{
			cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_check(prop.label(), prop.value() == "true" ? true : false)));
		}
		else if(prop.type() == ctrls::pg_type::spin)
		{
			auto item = cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_spin(prop.label(), prop.value())));
		}
		else if(prop.type() == ctrls::pg_type::color)
		{
			cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_color(prop.label(), prop.value(), false)));
		}
		else if(prop.type() == ctrls::pg_type::color_inherited)
		{
			cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_color(prop.label(), prop.value(), true)));
		}
		else if(prop.type() == ctrls::pg_type::halign || prop.type() == ctrls::pg_type::valign || prop.type() == ctrls::pg_type::layout ||
			prop.type() == ctrls::pg_type::seekdir)
		{
			auto item = cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_choice(prop.label())));
			auto pgc = static_cast<nana::pg_choice*>(item._m_pgitem());

			if(prop.type() == ctrls::pg_type::halign)
			{
				pgc->push_back(CITEM_LEFT);
				pgc->push_back(CITEM_CENTER);
				pgc->push_back(CITEM_RIGHT);
			}
			else if(prop.type() == ctrls::pg_type::valign)
			{
				pgc->push_back(CITEM_TOP);
				pgc->push_back(CITEM_CENTER);
				pgc->push_back(CITEM_BOTTOM);
			}
			else if(prop.type() == ctrls::pg_type::layout)
			{
				pgc->push_back(CITEM_HLAYOUT);
				pgc->push_back(CITEM_VLAYOUT);
			}
			else if(prop.type() == ctrls::pg_type::seekdir)
			{
				pgc->push_back(CITEM_BILATERAL);
				pgc->push_back(CITEM_FORWD);
				pgc->push_back(CITEM_BACKWD);
			}

			pgc->option(std::atoi(prop.value().c_str()));
		}
		else if(prop.type() == ctrls::pg_type::filename || prop.type() == ctrls::pg_type::filename_img)
		{
			auto item = cat.append(nana::propertygrid::pgitem_ptr(new pg_filename(prop.label(), prop.value())));
			auto pgc = static_cast<pg_filename*>(item._m_pgitem());

			if(prop.type() == ctrls::pg_type::filename_img)
			{
				pgc->add_filter("Image Files (" CREATOR_SUPPORTED_IMG ")", CREATOR_SUPPORTED_IMG);
			}
		}
		else if(prop.type() == ctrls::pg_type::folder)
		{
			cat.append(nana::propertygrid::pgitem_ptr(new pg_folder(prop.label(), prop.value())));
		}
		else if(prop.type() == ctrls::pg_type::collection_combox || prop.type() == ctrls::pg_type::collection_toolbar)
		{
			cat.append(nana::propertygrid::pgitem_ptr(new pg_collection(prop.label(), prop.value(), prop.type())));
		}
		else //nana::pg_type::string
		{
			cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string(prop.label(), prop.value())));
		}
	}

	// look for properties bonds
	for(size_t i = 0; i < _properties->count(); ++i)
	{
		auto pi = (*_properties)[i];
		enabled_bonds(pi.name(), pi.as_bool());
	}

	// set image, type and name
	_img.open(g_img_mgr.path(_properties->property("type").as_string()));
	_pic.load(_img);
	_type_txt.caption(_properties->property("type").as_string());
	_name_txt.editable(true);
	_name_txt.caption(_properties->property("name").as_string());

	_place.collocate();
}

void propertiespanel::enabled_bonds(const std::string& name, bool value)
{
	for(size_t i = 0; i < _properties->count(); ++i)
	{
		auto prop = (*_properties)[i];
		if(name == prop.enabled())
		{
			auto cat_idx = _propgrid.find(prop.category());
			if(cat_idx == nana::npos)
				continue;

			auto cat = _propgrid.at(cat_idx);
			for(auto c : cat)
			{
				if(c.label() == prop.label())
				{
					c._m_pgitem()->enabled(value == prop.enabled_value());
					break;
				}
			}
		}
	}
}
