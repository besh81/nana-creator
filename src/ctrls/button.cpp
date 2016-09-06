/*
 *		ctrls::button Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/button.h"
#include "nana_extra/pg_items.h" // to_color
#include "style.h"


namespace ctrls
{

	//button
	button::button(nana::window wd, properties_collection* properties, const std::string& name)
		: nana::button(wd, CTRL_BUTTON_NAME)
	{
		nana::API::ignore_mouse_focus(*this, false);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::active);


		_initproperties(properties, name);
	}


	void button::update(properties_collection* properties)
	{
		auto pw = nana::API::get_widget(parent());
		bool inherited;
		nana::color col;

		//
		caption(properties->property("caption").as_string());
		//
		enabled(properties->property("enabled").as_bool());
		//
		col = nana::to_color(properties->property("bgcolor").as_string(), inherited);
		bgcolor(inherited ? pw->bgcolor() : col);
		col = nana::to_color(properties->property("fgcolor").as_string(), inherited);
		fgcolor(inherited ? pw->fgcolor() : col);
		transparent(properties->property("transparent").as_bool());
	}


	void button::generatecode(properties_collection* properties, code_struct* cc)
	{
		// headers
		cc->hpps.push_back("#include <nana/gui/widgets/button.hpp>");

		std::string name = properties->property("name").as_string();

		// declaration
		cc->decl.push_back("nana::button " + name + ";");

		// init
		cc->init.push_back("// " + name);
		cc->init.push_back(name + ".create(" + cc->create + ");");
		cc->init.push_back(name + ".caption(\"" + properties->property("caption").as_string() + "\");");
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
		// transparent
		cc->init.push_back(name + ".transparent(" + properties->property("transparent").as_string("false") + ");");

		// placement
		cc->init.push_back(cc->place + "[\"field" + std::to_string(cc->field) + "\"] << " + name + ";");
	}


	void button::_initproperties(properties_collection* properties, const std::string& name)
	{
		// properties - main
		properties->append("type") = CTRL_BUTTON;
		properties->append("name") = name;
		// common
		properties->append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = "Button";
		properties->append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = "true";
		// appearance
		properties->append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = "[inherited],200,200,200";
		properties->append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = "[inherited],0,0,0";
		properties->append("transparent").label("Transparent").category(CAT_APPEARANCE).type(pg_type::check);
		// layout
		properties->append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties->append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
	}

}//end namespace ctrls
