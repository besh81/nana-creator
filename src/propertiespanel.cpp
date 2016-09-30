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
#include "nana_extra/pg_items.h"
#include "guimanager.h"
#include "lock_guard.h"
#include "style.h"


extern guimanager	g_gui_mgr;


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
	//_pic.align(align::center, align_v::center);
	//_pic.stretchable(true);
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

		std::cout << "Changed [" << arg.item.label().c_str() << "] " << arg.item.value().c_str() << "\n";

		for(size_t i = 0; i < _properties->count(); ++i)
		{
			if(arg.item.label() == (*_properties)[i].label())
			{
				if((*_properties)[i].type() == ctrls::pg_type::choice)
				{
					auto& entries = nana::any_cast<std::vector<std::string>>((*_properties)[i]._m_prop()->type_hints);
					for(unsigned e = 0; e < entries.size(); ++e)
						if(entries[e] == arg.item.value())
						{
							(*_properties)[i].value(e);
							break;
						}
				}
				else
					(*_properties)[i].value(arg.item.value());

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


	std::string type = _properties->property("type").as_string();

	
	if(type == CTRL_LAYOUT)
	{
		_img.open("icons/horizontal_layout_dark.png");
	}
	else if(type == CTRL_PANEL)
	{
		_img.open("icons/panel_dark.png");
	}
	else if(type == CTRL_BUTTON)
	{
		_img.open("icons/button_dark.png");
	}
	else if(type == CTRL_LABEL)
	{
		_img.open("icons/label_dark.png");
	}
	else if(type == CTRL_TEXTBOX)
	{
		_img.open("icons/textbox_dark.png");
	}
	else if(type == CTRL_COMBOX)
	{
		_img.open("icons/combox_dark.png");
	}
	else if(type == CTRL_SPINBOX)
	{
		_img.open("icons/spinbox_dark.png");
	}
	else
	{
		type = "UNKNOWN !!!";
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
		else if(prop.type() == ctrls::pg_type::string_uint)
		{
			cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string_uint(prop.label(), prop.value())));
		}
		else if(prop.type() == ctrls::pg_type::choice)
		{
			auto item = cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_choice(prop.label())));
			auto pgc = static_cast<nana::pg_choice*>(item._m_pgitem());

			auto& entries = nana::any_cast<std::vector<std::string>>(prop._m_prop()->type_hints);
			for(auto s : entries)
				pgc->push_back(s);
			
			pgc->option(std::atoi(prop.value().c_str()));
		}
		else if(prop.type() == ctrls::pg_type::check)
		{
			cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_check(prop.label(), prop.value() == "true" ? true : false)));
		}
		else if(prop.type() == ctrls::pg_type::spin)
		{
			auto item = cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_spin(prop.label())));
			auto psp = static_cast<nana::pg_spin*>(item._m_pgitem());

			auto& range = nana::any_cast<std::vector<int>>(prop._m_prop()->type_hints);
			psp->range(range[0], range[1], range[2]);

			psp->value(prop.value());
		}
		else if(prop.type() == ctrls::pg_type::color)
		{
			cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_color(prop.label(), prop.value(), false)));
		}
		else if(prop.type() == ctrls::pg_type::color_inherited)
		{
			cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_color(prop.label(), prop.value(), true)));
		}
		else //nana::pg_type::string
		{
			cat.append(nana::propertygrid::pgitem_ptr(new nana::pg_string(prop.label(), prop.value())));
		}
	}

	// set image, type and name
	_pic.load(_img);
	_type_txt.caption(type);
	_name_txt.caption(_properties->property("name").as_string());
	_name_txt.editable(true);
	
	_place.collocate();
}
