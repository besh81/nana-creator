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


void propertiespanel::_init_ctrls()
{
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
	_name.events().key_press([this, f](const nana::arg_keyboard& arg)
	{
		if(arg.key == nana::keyboard::enter && _properties)
		{
			_name.edited_reset();
			f(_name.caption());
		}
	});
	_name.events().focus([this, f](const nana::arg_focus& arg)
	{
		if(!arg.getting)
		{
			// just lost focus, so capture the value left by the user, if changed
			if(_name.edited() && _properties)
			{
				_name.edited_reset();
				f(_name.caption());
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
		_pic.load(nana::paint::image(""));
		_type.caption("");
		_name.editable(false);
		_name.caption("");
		_name.edited_reset();

		_place.collocate();
		return;
	}


	// add properties
	propertygrid_helper::append(&_propgrid, _properties);
	

	// set image, type and name
	_img.open(g_img_mgr.path(_properties->property("type").as_string()));
	_pic.load(_img);
	_type.caption(_properties->property("type").as_string());
	_name.editable(true);
	_name.caption(_properties->property("name").as_string());
	_name.edited_reset();

	_place.collocate();
}

