/*
 *		ctrls::spinbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/spinbox.h"
#include "nana_extra/color_helper.h"


namespace ctrls
{

	//spinbox
	spinbox::spinbox(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		spn.create(*parent->nanawdg);
		ctrl::init(&spn, CTRL_SPINBOX, name);

		// common
		properties.append("value").label("Value").category(CAT_COMMON).type(pg_type::string_int) = spn.to_int();
		properties.append("begin").label("Begin").category(CAT_COMMON).type(pg_type::string_int) = spn.range_int().first;
		properties.append("last").label("Last").category(CAT_COMMON).type(pg_type::string_int) = spn.range_int().second;
		properties.append("step").label("Step").category(CAT_COMMON).type(pg_type::string_int) = 1;
		properties.append("editable").label("Editable").category(CAT_COMMON).type(pg_type::check) = spn.editable();
		// appearance
		properties.append("prefix").label("Prefix").category(CAT_APPEARANCE).type(pg_type::string) = "";
		properties.append("suffix").label("Suffix").category(CAT_APPEARANCE).type(pg_type::string) = "";
		// layout
		// ...
	}


	void spinbox::update()
	{
		ctrl::update();

		spn.range(properties.property("begin").as_int(), properties.property("last").as_int(), properties.property("step").as_int());
		spn.value(properties.property("value").as_string());
		spn.editable(properties.property("editable").as_bool());
		spn.modifier(properties.property("prefix").as_string(), properties.property("suffix").as_string());
	}


	void spinbox::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/spinbox.hpp>");
		// declaration
		cd->decl.push_back("nana::spinbox " + name + ";");
		// init
		cd->init.push_back(name + ".range(" + properties.property("begin").as_string() + ", " + properties.property("last").as_string() + ", " + properties.property("step").as_string() + ");");
		cd->init.push_back(name + ".value(\"" + properties.property("value").as_string() + "\");");
		cd->init.push_back(name + ".editable(" + properties.property("editable").as_string() + ");");
		cd->init.push_back(name + ".modifier(\"" + properties.property("prefix").as_string() + "\", \"" + properties.property("suffix").as_string() + "\");");
	}

}//end namespace ctrls
