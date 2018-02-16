/*
 *		ctrls::listbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/listbox.h"
#include "nana_extra/pg_items.h" // to_color
#include "style.h"
#include "tokenizer/Tokenizer.h"


namespace ctrls
{

	//listbox
	listbox::listbox(nana::window wd, properties_collection* properties, const std::string& name)
		: nana::listbox(wd)
	{
		nana::API::ignore_mouse_focus(*this, false);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::active);


		_initproperties(properties, name);
	}


	void listbox::update(properties_collection* properties)
	{
		auto pw = nana::API::get_widget(parent());
		bool inherited;
		nana::color col;

		//
		enabled(properties->property("enabled").as_bool());
		// columns
		clear_headers(); // added in my fork -> not present in 1.5.6
		std::string options = properties->property("columns").as_string();
		if(!options.empty())
		{
			// columns: string with this format  ->  "item1" "item2" "item3" ...
			Tokenizer strtkn(options);
			strtkn.setDelimiter("\"");
			std::string item;
			while((item = strtkn.next()) != "")
			{
				if(item != " ")
					append_header(item);
			}
		}
		//
		col = nana::to_color(properties->property("bgcolor").as_string(), inherited);
		bgcolor(inherited ? pw->bgcolor() : col);
		col = nana::to_color(properties->property("fgcolor").as_string(), inherited);
		fgcolor(inherited ? pw->fgcolor() : col);
		//
		show_header(properties->property("show_header").as_bool());
	}


	void listbox::generatecode(properties_collection* properties, code_data_struct* cd, code_info_struct* ci)
	{
		// headers
		cd->hpps.add("#include <nana/gui/widgets/listbox.hpp>");

		std::string name = properties->property("name").as_string();

		// declaration
		cd->decl.push_back("nana::listbox " + name + ";");

		// init
		cd->init.push_back("// " + name);
		cd->init.push_back(name + ".create(" + ci->create + ");");

		// columns
		std::string options = properties->property("columns").as_string();
		if(!options.empty())
		{
			// columns: string with this format  ->  "item1" "item2" "item3" ...
			Tokenizer strtkn(options);
			strtkn.setDelimiter("\"");
			std::string item;
			while((item = strtkn.next()) != "")
			{
				if(item != " ")
					cd->init.push_back(name + ".append_header(\"" + item + "\");");
			}
		}

		cd->init.push_back(name + ".checkable(" + properties->property("checkable").as_string() + ");");
		if(properties->property("single_selection").as_bool())
			cd->init.push_back(name + ".enable_single(true, true);");
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
		cd->init.push_back(name + ".show_header(" + properties->property("show_header").as_string() + ");");

		// placement
		cd->init.push_back(ci->place + "[\"field" + std::to_string(ci->field) + "\"] << " + name + ";");
	}


	void listbox::_initproperties(properties_collection* properties, const std::string& name)
	{
		// properties - main
		properties->append("type") = CTRL_LISTBOX;
		properties->append("name") = name;
		// common
		properties->append("columns").label("Columns").category(CAT_COMMON).type(pg_type::collection) = "";
		properties->append("checkable").label("Checkable").category(CAT_COMMON).type(pg_type::check) = false;
		properties->append("single_selection").label("Single selection").category(CAT_COMMON).type(pg_type::check) = false;
		properties->append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = enabled();
		// appearance
		properties->append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(bgcolor(), false);
		properties->append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(fgcolor(), false);
		properties->append("show_header").label("Show header").category(CAT_APPEARANCE).type(pg_type::check) = visible_header();
		// layout
		properties->append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties->append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
	}

}//end namespace ctrls
