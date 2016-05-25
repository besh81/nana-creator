/*
 *		ctrls::button Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/button.h"
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
		caption(properties->property("caption").as_string());
	}


	void button::generatecode(properties_collection* properties, std::vector<std::string>* decl, std::vector<std::string>* init)
	{
		if(decl)
			decl->push_back("nana::button " + properties->property("name").as_string() + ";");

		if(init)
		{
			std::string var = properties->property("name").as_string();

			init->clear();
			init->push_back("// " + var);
			init->push_back(var + ".caption(\"" + properties->property("caption").as_string() + "\");");
		}
	}


	void button::_initproperties(properties_collection* properties, const std::string& name)
	{
		// properties - main
		properties->append("type") = CTRL_BUTTON;
		properties->append("header") = "nana/gui/widgets/button.hpp";
		properties->append("name") = name;
		// common
		properties->append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = "Button";
		// appearance
		//...
		// layout
		properties->append("weight").label("Weight").category(CAT_LAYOUT).type(pg_type::string_int) = -1;
		properties->append("margin").label("Margin").category(CAT_LAYOUT).type(pg_type::string_uint) = 0;
	}

}//end namespace ctrls
