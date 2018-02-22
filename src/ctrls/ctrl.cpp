/*
 *		ctrls::ctrl Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/ctrl.h"
#include "nana_extra/color_helper.h"
#include "style.h"


namespace ctrls
{

	//ctrl
	void ctrl::update()
	{
		auto pw = nana::API::get_widget(nanawdg->parent());
		bool inherited;
		nana::color col;

		nanawdg->enabled(properties.property("enabled").as_bool());

		col = nana::to_color(properties.property("bgcolor").as_string(), inherited);
		nanawdg->bgcolor(inherited ? pw->bgcolor() : col);

		col = nana::to_color(properties.property("fgcolor").as_string(), inherited);
		nanawdg->fgcolor(inherited ? pw->fgcolor() : col);
	}


	void ctrl::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		std::string name = properties.property("name").as_string();
		// create
		cd->init.push_back("// " + name);
		cd->init.push_back(name + ".create(" + ci->create + ");");
		// placement
		cd->init.push_back(ci->place + "[\"field" + std::to_string(ci->field) + "\"] << " + name + ";");
		// init
		cd->init.push_back(name + ".enabled(" + properties.property("enabled").as_string() + ");");
		// color
		bool inherited;
		std::string col;
		// bg
		col = properties.property("bgcolor").as_string();
		nana::to_color(col, inherited);
		if(!inherited)
			cd->init.push_back(name + ".bgcolor(nana::color(" + col + "));");
		// fg
		col = properties.property("fgcolor").as_string();
		nana::to_color(col, inherited);
		if(!inherited)
			cd->init.push_back(name + ".fgcolor(nana::color(" + col + "));");
	}


	void ctrl::init(nana::widget* wdg, const std::string& type, const std::string& name)
	{
		nanawdg = wdg;

		nana::API::ignore_mouse_focus(*nanawdg, false);
		nana::API::effects_edge_nimbus(*nanawdg, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*nanawdg, nana::effects::edge_nimbus::active);


		// properties - main
		properties.append("type") = type;
		properties.append("name") = name;
		// common
		properties.append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = nanawdg->enabled();
		// appearance
		properties.append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(nanawdg->bgcolor(), true);
		properties.append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(nanawdg->fgcolor(), true);
		// layout
		properties.append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties.append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
	}

}//end namespace ctrls
