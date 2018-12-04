/*
 *		ctrls::button Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/button.h"


namespace ctrls
{

	//button
	button::button(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		btn.create(*parent->nanawdg);
		ctrl::init(&btn, CTRL_BUTTON, name);

		// common
		properties.append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = CTRL_BUTTON;
		// appearance
		properties.append("transparent").label("Transparent").category(CAT_APPEARANCE).type(pg_type::check) = btn.transparent();
		// layout
		// ...
	}


	void button::update()
	{
		ctrl::update();

		btn.caption(properties.property("caption").as_string());
		btn.transparent(properties.property("transparent").as_bool());
	}


	void button::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/button.hpp>");
		// declaration
		cd->decl.push_back("nana::button " + name + ";");
		// init
		cd->init.push_back(name + ".caption(\"" + properties.property("caption").as_string() + "\");");
		if(properties.property("transparent").as_bool())
			cd->init.push_back(name + ".transparent(true);");
	}

}//end namespace ctrls
