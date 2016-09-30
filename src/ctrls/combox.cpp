/*
 *		ctrls::combox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/combox.h"
#include "nana_extra/pg_items.h" // to_color
#include "style.h"


namespace ctrls
{

	//combox
	combox::combox(nana::window wd, properties_collection* properties, const std::string& name)
		: nana::combox(wd)
	{
		nana::API::ignore_mouse_focus(*this, false);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::active);


		_initproperties(properties, name);
	}


	void combox::update(properties_collection* properties)
	{
		auto pw = nana::API::get_widget(parent());
		bool inherited;
		nana::color col;

		//
		editable(properties->property("editable").as_bool());
		enabled(properties->property("enabled").as_bool());
		//
		col = nana::to_color(properties->property("bgcolor").as_string(), inherited);
		bgcolor(inherited ? pw->bgcolor() : col);
		col = nana::to_color(properties->property("fgcolor").as_string(), inherited);
		fgcolor(inherited ? pw->fgcolor() : col);
	}


	void combox::generatecode(properties_collection* properties, code_struct* cc)
	{
		// headers
		cc->hpps.push_back("#include <nana/gui/widgets/combox.hpp>");

		std::string name = properties->property("name").as_string();

		// declaration
		cc->decl.push_back("nana::combox " + name + ";");

		// init
		cc->init.push_back("// " + name);
		cc->init.push_back(name + ".create(" + cc->create + ");");
		cc->init.push_back(name + ".editable(" + properties->property("editable").as_string() + ");");
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

		// placement
		cc->init.push_back(cc->place + "[\"field" + std::to_string(cc->field) + "\"] << " + name + ";");
	}


	void combox::_initproperties(properties_collection* properties, const std::string& name)
	{
		// properties - main
		properties->append("type") = CTRL_COMBOX;
		properties->append("name") = name;
		// common
		properties->append("editable").label("Editable").category(CAT_COMMON).type(pg_type::check) = editable();
		properties->append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = enabled();
		// appearance
		properties->append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(bgcolor(), false);
		properties->append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(fgcolor(), false);
		// layout
		properties->append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties->append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
	}

}//end namespace ctrls
