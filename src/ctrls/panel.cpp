/*
 *		ctrls::panel Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/panel.h"
#include "guimanager.h"
#include "nana_extra/pg_items.h" // to_color
#include "style.h"


extern guimanager	g_gui_mgr;


namespace ctrls
{

	//panel
	panel::panel(nana::window wd, properties_collection* properties, const std::string& name)
		: nana::panel<true>(wd)
	{
		_place.div("abc");
		_place.collocate();


		nana::API::ignore_mouse_focus(*this, false);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::active);


		_initproperties(properties, name);


		//TODO - spostare in guimanager
		events().mouse_enter([this]()
		{
			if(_child)
				return; // already has a child

			if(!_is_highlighted && g_gui_mgr.cursor().action != cursor_action::select)
			{
				if(g_gui_mgr.cursor().type == CTRL_LAYOUT)
				{
					_is_highlighted = true;
					_bgcolor = bgcolor();
					bgcolor(HIGHLIGHT_ON_PLACE);
				}
			}
		});

		events().mouse_leave([this]()
		{
			if(_is_highlighted)
			{
				_is_highlighted = false;
				bgcolor(_bgcolor);
			}
		});
	}


	void panel::update(properties_collection* properties)
	{
		auto pw = nana::API::get_widget(parent());
		bool inherited;
		nana::color col;

		if(properties->property("mainclass").as_bool())
			pw = nana::API::get_widget(pw->parent());

		//
		enabled(properties->property("enabled").as_bool());
		//
		col = nana::to_color(properties->property("bgcolor").as_string(), inherited);
		bgcolor(inherited ? pw->bgcolor() : col);
		col = nana::to_color(properties->property("fgcolor").as_string(), inherited);
		fgcolor(inherited ? pw->fgcolor() : col);


		_place.collocate();
	}


	void panel::generatecode(properties_collection* properties, code_struct* cc)
	{
		// headers
		cc->hpps.push_back("#include <nana/gui/widgets/panel.hpp>");

		auto name = properties->property("name").as_string();

		// declaration
		if(!properties->property("mainclass").as_bool())
			cc->decl.push_back("nana::panel<true> " + name + ";"); //todo: controllare hasbackground

		// init
		cc->init.push_back("// " + name);
		if(!properties->property("mainclass").as_bool())
		{
			cc->init.push_back(name + ".create(" + cc->create + ");");
			cc->init.push_back(name + ".enabled(" + properties->property("enabled").as_string() + ");");
			// color
			bool inherited;
			std::string col;
			// bg
			col = properties->property("bgcolor").as_string();
			nana::to_color(col, inherited);
			if(!inherited)
				cc->init.push_back(name + ".bgcolor(nana::color(" + col + "));");
			// fg
			col = properties->property("fgcolor").as_string();
			nana::to_color(col, inherited);
			if(!inherited)
				cc->init.push_back(name + ".fgcolor(nana::color(" + col + "));");
		}
		else
		{
			cc->init.push_back("enabled(" + properties->property("enabled").as_string() + ");");
			// color
			bool inherited;
			std::string col;
			// bg
			col = properties->property("bgcolor").as_string();
			nana::to_color(col, inherited);
			if(!inherited)
				cc->init.push_back(name + "bgcolor(nana::color(" + col + "));");
			// fg
			col = properties->property("fgcolor").as_string();
			nana::to_color(col, inherited);
			if(!inherited)
				cc->init.push_back(name + "fgcolor(nana::color(" + col + "));");
		}

		// placement
		if(!properties->property("mainclass").as_bool())
			cc->init.push_back(cc->place + "[\"field" + std::to_string(cc->field) + "\"] << " + name + ";");

		// children
		if(!properties->property("mainclass").as_bool())
			cc->create = name;
		cc->place = "";
	}


	void panel::_initproperties(properties_collection* properties, const std::string& name)
	{
		// properties - main
		properties->append("type") = CTRL_PANEL;
		properties->append("name") = name;
		// common
		//properties->append(property_t{ "hasbackground", "", "Has Background", "", pg_type::check }) = true;
		properties->append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = enabled();
		// appearance
		properties->append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(bgcolor(), true);
		properties->append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(fgcolor(), true);
		// layout
		properties->append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties->append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
	}


	void panel::prepareforinsert()
	{
		// reset bg color highlight
		_is_highlighted = false;
		bgcolor(_bgcolor);
	}


	bool panel::append(nana::window child)
	{
		if(haschild())
			return false;

		_child = true;

		_place.field("abc") << child;
		_place.collocate();
		return true;
	}


	bool panel::remove(nana::window child)
	{
		if(!_child)
			return false;

		_place.field_display("abc", false);
		_place.erase(child);
		_place.field_display("abc", true);
		_place.collocate();

		_child = false;
		return true;
	}

}//end namespace ctrls
