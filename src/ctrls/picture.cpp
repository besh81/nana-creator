/*
 *		ctrls::picture Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/picture.h"
#include "nana_extra/pg_items.h" // to_color
#include "style.h"


namespace ctrls
{

	//picture
	picture::picture(nana::window wd, properties_collection* properties, const std::string& name)
		: nana::picture(wd)
	{
		nana::API::ignore_mouse_focus(*this, false);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::active);

		_initproperties(properties, name);
	}


	void picture::update(properties_collection* properties)
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
		transparent(properties->property("transparent").as_bool());
		align(static_cast<nana::align>(properties->property("halign").as_int()),
			static_cast<nana::align_v>(properties->property("valign").as_int()));
		stretchable(properties->property("stretchable").as_bool());
	}


	void picture::generatecode(properties_collection* properties, code_data_struct* cd, code_info_struct* ci)
	{
		// headers
		cd->hpps.add("#include <nana/gui/widgets/picture.hpp>");

		std::string name = properties->property("name").as_string();

		// declaration
		cd->decl.push_back("nana::picture " + name + ";");

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
		cd->init.push_back(name + ".transparent(" + properties->property("transparent").as_string() + ");");
		cd->init.push_back(name + ".align(static_cast<nana::align>(" + properties->property("halign").as_string() + "), static_cast<nana::align_v>(" + properties->property("valign").as_string() + "));");
		cd->init.push_back(name + ".stretchable(" + properties->property("stretchable").as_string() + ");");


		// placement
		cd->init.push_back(ci->place + "[\"field" + std::to_string(ci->field) + "\"] << " + name + ";");
	}


	void picture::_initproperties(properties_collection* properties, const std::string& name)
	{
		// properties - main
		properties->append("type") = CTRL_PICTURE;
		properties->append("name") = name;
		// common
		properties->append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = enabled();

		// appearance
		properties->append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(bgcolor(), false);
		properties->append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = nana::to_string(fgcolor(), false);
		properties->append("transparent").label("Transparent").category(CAT_APPEARANCE).type(pg_type::check) = transparent();
		properties->append("halign").label("Horizontal Alignment").category(CAT_APPEARANCE).type(pg_type::choice).type_hints(
			std::vector<std::string>{ CITEM_LEFT, CITEM_CENTER, CITEM_RIGHT }) = static_cast<int>(nana::align::left);
		properties->append("valign").label("Vertical Alignment").category(CAT_APPEARANCE).type(pg_type::choice).type_hints(
			std::vector<std::string>{ CITEM_TOP, CITEM_CENTER, CITEM_BOTTOM }) = static_cast<int>(nana::align_v::top);
		properties->append("stretchable").label("Stretchable").category(CAT_APPEARANCE).type(pg_type::check) = false;
		// layout
		properties->append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties->append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
	}

}//end namespace ctrls
