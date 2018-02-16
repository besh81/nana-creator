/*
 *		ctrls::menubar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/menubar.h"
#include "nana_extra/pg_items.h" // to_color
#include "style.h"
#include "tokenizer/Tokenizer.h"


namespace ctrls
{

	//menubar
	menubar::menubar(nana::window wd, properties_collection* properties, const std::string& name)
		: nana::menubar(wd)
	{
		nana::API::ignore_mouse_focus(*this, false);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::active);

		_initproperties(properties, name);
	}


	void menubar::update(properties_collection* properties)
	{
		auto pw = nana::API::get_widget(parent());
		bool inherited;
		nana::color col;

		//
		enabled(properties->property("enabled").as_bool());
		// menus
		clear(); // added in my fork -> not present in 1.5.6
		std::string menus = properties->property("menus").as_string();
		if(!menus.empty())
		{
			// menus: string with this format  ->  "item1" "item2" "item3" ...
			Tokenizer strtkn(menus);
			strtkn.setDelimiter("\"");
			std::string item;
			while((item = strtkn.next()) != "")
			{
				if(item != " ")
					push_back(item);
			}
		}
		//
		col = nana::to_color(properties->property("bgcolor").as_string(), inherited);
		bgcolor(inherited ? pw->bgcolor() : col);
		col = nana::to_color(properties->property("fgcolor").as_string(), inherited);
		fgcolor(inherited ? pw->fgcolor() : col);
	}


	void menubar::generatecode(properties_collection* properties, code_data_struct* cd, code_info_struct* ci)
	{
		// headers
		cd->hpps.add("#include <nana/gui/widgets/menubar.hpp>");

		std::string name = properties->property("name").as_string();

		// declaration
		cd->decl.push_back("nana::menubar " + name + ";");

		// init
		cd->init.push_back("// " + name);
		cd->init.push_back(name + ".create(" + ci->create + ");");
		cd->init.push_back(name + ".enabled(" + properties->property("enabled").as_string() + ");");

		// menus
		std::string menus = properties->property("menus").as_string();
		if(!menus.empty())
		{
			// menus: string with this format  ->  "item1" "item2" "item3" ...
			Tokenizer strtkn(menus);
			strtkn.setDelimiter("\"");
			std::string item;
			while((item = strtkn.next()) != "")
			{
				if(item != " ")
					cd->init.push_back(name + ".push_back(\"" + item + "\");");
			}
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


		// placement
		cd->init.push_back(ci->place + "[\"field" + std::to_string(ci->field) + "\"] << " + name + ";");
	}


	void menubar::_initproperties(properties_collection* properties, const std::string& name)
	{
		// properties - main
		properties->append("type") = CTRL_MENUBAR;
		properties->append("name") = name;
		// common
		properties->append("menus").label("Menus").category(CAT_COMMON).type(pg_type::collection) = "";
		properties->append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = enabled();

		// appearance
		properties->append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(bgcolor(), false);
		properties->append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(fgcolor(), false);
		// layout
		properties->append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties->append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
	}

}//end namespace ctrls
