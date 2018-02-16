/*
 *		ctrls::progress Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/progress.h"
#include "nana_extra/pg_items.h" // to_color
#include "style.h"


namespace ctrls
{

	//progress
	progress::progress(nana::window wd, properties_collection* properties, const std::string& name)
		: nana::progress(wd)
	{
		nana::API::ignore_mouse_focus(*this, false);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::active);

		_initproperties(properties, name);
	}


	void progress::update(properties_collection* properties)
	{
		auto pw = nana::API::get_widget(parent());
		bool inherited;
		nana::color col;

		//
		if(properties->property("unknown").as_bool())
			value(0);
		else
			value(properties->property("value").as_uint());

		enabled(properties->property("enabled").as_bool());

		//
		col = nana::to_color(properties->property("bgcolor").as_string(), inherited);
		bgcolor(inherited ? pw->bgcolor() : col);
		col = nana::to_color(properties->property("fgcolor").as_string(), inherited);
		fgcolor(inherited ? pw->fgcolor() : col);
	}


	void progress::generatecode(properties_collection* properties, code_data_struct* cd, code_info_struct* ci)
	{
		// headers
		cd->hpps.add("#include <nana/gui/widgets/progress.hpp>");

		std::string name = properties->property("name").as_string();

		// declaration
		cd->decl.push_back("nana::progress " + name + ";");

		// init
		cd->init.push_back("// " + name);
		cd->init.push_back(name + ".create(" + ci->create + ");");
		if(properties->property("unknown").as_bool())
			cd->init.push_back(name + ".unknown(" + properties->property("unknown").as_string() + ");");
		else
			cd->init.push_back(name + ".value(" + properties->property("value").as_string() + ");");
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


	void progress::_initproperties(properties_collection* properties, const std::string& name)
	{
		// properties - main
		properties->append("type") = CTRL_PROGRESS;
		properties->append("name") = name;
		// common
		properties->append("value").label("Value").category(CAT_COMMON).type(pg_type::string_uint).type_hints(
			std::vector<int>{ 0, 100 }) = value();
		properties->append("unknown").label("Unknown").category(CAT_COMMON).type(pg_type::check) = unknown();
		properties->append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = enabled();

		// appearance
		properties->append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(bgcolor(), false);
		properties->append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(fgcolor(), false);
		// layout
		properties->append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties->append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
	}

}//end namespace ctrls
