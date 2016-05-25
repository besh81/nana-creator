/*
 *		ctrls::label Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/label.h"
#include "nana_extra/pg_items.h" // to_color
#include "style.h"


namespace ctrls
{

	//label
	label::label(nana::window wd, properties_collection* properties, const std::string& name)
		: nana::label(wd, CTRL_LABEL_NAME)
	{
		nana::API::ignore_mouse_focus(*this, false);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::active);


		_initproperties(properties, name);
	}


	void label::update(properties_collection* properties)
	{
		auto pw = nana::API::get_widget(parent());
		bool inherited;
		nana::color col;
		//
		caption(properties->property("caption").as_string());
		format(properties->property("format").as_bool());
		enabled(properties->property("enabled").as_bool());
		//
		col = nana::to_color(properties->property("bgcolor").as_string(), inherited);
		bgcolor(inherited ? pw->bgcolor() : col);
		col = nana::to_color(properties->property("fgcolor").as_string(), inherited);
		fgcolor(inherited ? pw->fgcolor() : col);
		transparent(properties->property("transparent").as_bool());
		//
		text_align(static_cast<nana::align>(properties->property("halign").as_int()),
			static_cast<nana::align_v>(properties->property("valign").as_int()));
	}


	void label::generatecode(properties_collection* properties, std::vector<std::string>* decl, std::vector<std::string>* init)
	{
		if(decl)
			decl->push_back("nana::label " + properties->property("name").as_string() + ";");

		if(init)
		{
			std::string var = properties->property("name").as_string();

			init->clear();
			init->push_back("// " + var);
			init->push_back(var + ".caption(\"" + properties->property("caption").as_string() + "\");");
			init->push_back(var + ".format(" + properties->property("format").as_string() + ");");
		}
	}


	void label::_initproperties(properties_collection* properties, const std::string& name)
	{
		// properties - main
		properties->append("type") = CTRL_LABEL;
		properties->append("header") = "nana/gui/widgets/label.hpp";
		properties->append("name") = name;
		// common
		properties->append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = "Label";
		properties->append("format").label("Format").category(CAT_COMMON).type(pg_type::check) = "false";
		properties->append("enabled").label("Enabled").category(CAT_COMMON).type(pg_type::check) = "true";
		// appearance
		properties->append("bgcolor").label("Background").category(CAT_APPEARANCE).type(pg_type::color_inherited) = "[inherited],200,200,200";
		properties->append("fgcolor").label("Foreground").category(CAT_APPEARANCE).type(pg_type::color_inherited) = "[inherited],0,0,0";
		properties->append("transparent").label("Transparent").category(CAT_APPEARANCE).type(pg_type::check);
		// layout
		properties->append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties->append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
		properties->append("halign").label("Horizontal Alignment").category(CAT_LAYOUT).type(pg_type::choice).type_hints(
			std::vector<std::string>{ CITEM_LEFT, CITEM_CENTER, CITEM_RIGHT }) = static_cast<int>(nana::align::left);
		properties->append("valign").label("Vertical Alignment").category(CAT_LAYOUT).type(pg_type::choice).type_hints(
			std::vector<std::string>{ CITEM_TOP, CITEM_CENTER, CITEM_BOTTOM }) = static_cast<int>(nana::align_v::top);
	}

}//end namespace ctrls
