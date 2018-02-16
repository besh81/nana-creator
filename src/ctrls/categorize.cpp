/*
 *		ctrls::categorize Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/categorize.h"
#include "nana_extra/pg_items.h" // to_color
#include "style.h"


namespace ctrls
{

	//categorize
	categorize::categorize(nana::window wd, properties_collection* properties, const std::string& name)
		: nana::categorize<int>(wd, "")
	{
		nana::API::ignore_mouse_focus(*this, false);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::active);

		_initproperties(properties, name);
	}


	void categorize::update(properties_collection* properties)
	{
		auto pw = nana::API::get_widget(parent());
		bool inherited;
		nana::color col;

		//
		enabled(properties->property("enabled").as_bool());

		//
		col = nana::to_color(properties->property("bgcolor").as_string(), inherited);
		bgcolor(inherited ? pw->bgcolor() : col);
		col = nana::to_color(properties->property("fgcolor").as_string(), inherited);
		fgcolor(inherited ? pw->fgcolor() : col);
	}


	void categorize::generatecode(properties_collection* properties, code_data_struct* cd, code_info_struct* ci)
	{
		// headers
		cd->hpps.add("#include <nana/gui/widgets/categorize.hpp>");

		std::string name = properties->property("name").as_string();

		// declaration
		cd->decl.push_back("nana::categorize<int> " + name + ";");

		// init
		cd->init.push_back("// " + name);
		cd->init.push_back(name + ".create(" + ci->create + ");");
		cd->init.push_back(name + ".enabled(" + properties->property("enabled").as_string() + ");");

		// color
		bool inherited;
		std::string col;
		// bg
		col = properties->property("bgcolor").as_string();
		nana::to_color(col, inherited);
		if(!inherited)
			cd->init.push_back(name + ".bgcolor(nana::color(" + col + "));");
		// fg
		col = properties->property("fgcolor").as_string();
		nana::to_color(col, inherited);
		if(!inherited)
			cd->init.push_back(name + ".fgcolor(nana::color(" + col + "));");
		//


		// placement
		cd->init.push_back(ci->place + "[\"field" + std::to_string(ci->field) + "\"] << " + name + ";");
	}


	void categorize::_initproperties(properties_collection* properties, const std::string& name)
	{
		// properties - main
		properties->append("type") = CTRL_CATEGORIZE;
		properties->append("name") = name;
		// common
		properties->append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = enabled();

		// appearance
		properties->append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(bgcolor(), false);
		properties->append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(fgcolor(), false);
		// layout
		properties->append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties->append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
	}

}//end namespace ctrls
