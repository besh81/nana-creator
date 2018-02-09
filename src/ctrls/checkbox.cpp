/*
 *		ctrls::checkbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/checkbox.h"
#include "nana_extra/pg_items.h" // to_color
#include "style.h"


namespace ctrls
{

	//checkbox
	checkbox::checkbox(nana::window wd, properties_collection* properties, const std::string& name)
		: nana::checkbox(wd, "")
	{
		nana::API::ignore_mouse_focus(*this, false);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::active);

		_initproperties(properties, name);
	}


	void checkbox::update(properties_collection* properties)
	{
		auto pw = nana::API::get_widget(parent());
		bool inherited;
		nana::color col;

		//
		caption(properties->property("caption").as_string());
		enabled(properties->property("enabled").as_bool());

		//
		col = nana::to_color(properties->property("bgcolor").as_string(), inherited);
		bgcolor(inherited ? pw->bgcolor() : col);
		col = nana::to_color(properties->property("fgcolor").as_string(), inherited);
		fgcolor(inherited ? pw->fgcolor() : col);
		//
		check(properties->property("check").as_bool());
		radio(properties->property("radio").as_bool());

		//
		react(false); // needs to avoid user can change the state clicking on control
	}


	void checkbox::generatecode(properties_collection* properties, code_data_struct* cd, code_info_struct* ci)
	{
		// headers
		cd->hpps.add("#include <nana/gui/widgets/checkbox.hpp>");

		std::string name = properties->property("name").as_string();

		// declaration
		cd->decl.push_back("nana::checkbox " + name + ";");

		// init
		cd->init.push_back("// " + name);
		cd->init.push_back(name + ".create(" + ci->create + ");");
		cd->init.push_back(name + ".caption(\"" + properties->property("caption").as_string() + "\");");
		cd->init.push_back(name + ".enabled(" + properties->property("enabled").as_string() + ");");


		// radio_group
		std::string group = "rg_" + properties->property("group").as_string();
		if(!group.empty())
		{
			//TODO: il nome del gruppo non dovrebbe essere presente tra i vari controlli

			// check if a group with the same name already present
			bool first_time = true;
			std::string group_decl("nana::radio_group " + group + ";");
			for(auto i : cd->decl)
				if(i == group_decl)
				{
					first_time = false;
					break;
				}
			if(first_time)
				cd->decl.push_back(group_decl);
		}


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
		if(!group.empty())
			cd->init.push_back(group + ".add(" + name + ");");
		cd->init.push_back(name + ".check(" + properties->property("check").as_string() + ");");
		cd->init.push_back(name + ".radio(" + properties->property("radio").as_string() + ");");

		// placement
		cd->init.push_back(ci->place + "[\"field" + std::to_string(ci->field) + "\"] << " + name + ";");
	}


	void checkbox::_initproperties(properties_collection* properties, const std::string& name)
	{
		// properties - main
		properties->append("type") = CTRL_CHECKBOX;
		properties->append("name") = name;
		// common
		properties->append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = "";
		properties->append("check").label("Checked").category(CAT_COMMON).type(pg_type::check) = checked();
		properties->append("group").label("Group name").category(CAT_COMMON).type(pg_type::string) = "";
		properties->append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = enabled();
		// appearance
		properties->append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(bgcolor(), false);
		properties->append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(fgcolor(), false);
		properties->append("radio").label("Radio").category(CAT_APPEARANCE).type(pg_type::check) = false;
		// layout
		properties->append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties->append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
	}

}//end namespace ctrls
