/*
 *		ctrls::slider Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/slider.h"
#include "nana_extra/pg_items.h" // to_color
#include "style.h"


namespace ctrls
{

	//slider
	slider::slider(nana::window wd, properties_collection* properties, const std::string& name)
		: nana::slider(wd)
	{
		nana::API::ignore_mouse_focus(*this, false);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::active);

		_initproperties(properties, name);
	}


	void slider::update(properties_collection* properties)
	{
		auto pw = nana::API::get_widget(parent());
		bool inherited;
		nana::color col;

		//
		maximum(properties->property("maximum").as_uint());
		value(properties->property("value").as_uint());
		seek(static_cast<nana::drawerbase::slider::seekdir>(properties->property("seek").as_int()));
		enabled(properties->property("enabled").as_bool());

		//
		col = nana::to_color(properties->property("bgcolor").as_string(), inherited);
		bgcolor(inherited ? pw->bgcolor() : col);
		col = nana::to_color(properties->property("fgcolor").as_string(), inherited);
		fgcolor(inherited ? pw->fgcolor() : col);
		//
		vertical(properties->property("vertical").as_bool());
		transparent(properties->property("transparent").as_bool());
	}


	void slider::generatecode(properties_collection* properties, code_data_struct* cd, code_info_struct* ci)
	{
		// headers
		cd->hpps.add("#include <nana/gui/widgets/slider.hpp>");

		std::string name = properties->property("name").as_string();

		// declaration
		cd->decl.push_back("nana::slider " + name + ";");

		// init
		cd->init.push_back("// " + name);
		cd->init.push_back(name + ".create(" + ci->create + ");");
		cd->init.push_back(name + ".maximum(" + properties->property("maximum").as_string() + ");");
		cd->init.push_back(name + ".value(" + properties->property("value").as_string() + ");");
		cd->init.push_back(name + ".seek(static_cast<nana::drawerbase::slider::seekdir>(" + properties->property("seek").as_string() + "));");
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
		cd->init.push_back(name + ".vertical(" + properties->property("vertical").as_string() + ");");
		cd->init.push_back(name + ".transparent(" + properties->property("transparent").as_string() + ");");


		// placement
		cd->init.push_back(ci->place + "[\"field" + std::to_string(ci->field) + "\"] << " + name + ";");
	}


	void slider::_initproperties(properties_collection* properties, const std::string& name)
	{
		// properties - main
		properties->append("type") = CTRL_SLIDER;
		properties->append("name") = name;
		// common
		properties->append("value").label("Value").category(CAT_COMMON).type(pg_type::string_uint) = value();
		properties->append("maximum").label("Maximum").category(CAT_COMMON).type(pg_type::string_uint) = maximum();
		properties->append("seek").label("Seek").category(CAT_COMMON).type(pg_type::choice).type_hints(
			std::vector<std::string>{ CITEM_BILATERAL, CITEM_FORWD, CITEM_BACKWD }) = static_cast<int>(nana::drawerbase::slider::seekdir::bilateral);
		properties->append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = enabled();
		// appearance
		properties->append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(bgcolor(), false);
		properties->append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(fgcolor(), false);
		properties->append("vertical").label("Vertical").category(CAT_APPEARANCE).type(pg_type::check) = vertical();
		properties->append("transparent").label("Transparent").category(CAT_APPEARANCE).type(pg_type::check) = transparent();
		// layout
		properties->append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties->append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
	}

}//end namespace ctrls
