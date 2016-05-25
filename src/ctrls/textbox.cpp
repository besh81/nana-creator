/*
 *		ctrls::textbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/textbox.h"
#include "style.h"


namespace ctrls
{

	//textbox
	textbox::textbox(nana::window wd, properties_collection* properties, const std::string& name)
		: nana::textbox(wd, CTRL_TEXTBOX_NAME)
	{
		nana::API::ignore_mouse_focus(*this, false);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::none);
		nana::API::effects_edge_nimbus(*this, nana::effects::edge_nimbus::active);


		_initproperties(properties, name);
	}


	void textbox::update(properties_collection* properties)
	{
	}


	void textbox::generatecode(properties_collection* properties, std::vector<std::string>* decl, std::vector<std::string>* init)
	{
		if(decl)
			decl->push_back("nana::textbox " + properties->property("name").as_string() + ";");

		if(init)
		{
			std::string var = properties->property("name").as_string();

			init->clear();
			init->push_back("// " + var);
		}
	}


	void textbox::_initproperties(properties_collection* properties, const std::string& name)
	{
		// properties - main
		properties->append("type") = CTRL_TEXTBOX;
		properties->append("header") = "nana/gui/widgets/textbox.hpp";
		properties->append("name") = name;
		// common
		// appearance
		// layout
		properties->append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties->append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
	}

}//end namespace ctrls
