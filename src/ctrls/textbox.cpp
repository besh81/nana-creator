/*
 *		ctrls::textbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/textbox.h"
#include "nana_extra/color_helper.h"


namespace ctrls
{

	//textbox
	textbox::textbox(nana::window wd, const std::string& name)
		: ctrl()
	{
		txt.create(wd);
		ctrl::init(&txt, CTRL_TEXTBOX, name);

		// common
		properties.append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = "";
		properties.append("tip_string").label("Tip").category(CAT_COMMON).type(pg_type::string) = "";
		properties.append("line_wrapped").label("Line Wrapped").category(CAT_COMMON).type(pg_type::check) = txt.line_wrapped();
		properties.append("multi_lines").label("Multiple Lines").category(CAT_COMMON).type(pg_type::check) = txt.multi_lines();
		properties.append("editable").label("Editable").category(CAT_COMMON).type(pg_type::check) = txt.editable();
		// appearance
		properties.property("bgcolor") = nana::to_string(txt.bgcolor(), false);
		// layout
		// ...
	}


	void textbox::update()
	{
		ctrl::update();

		txt.caption(properties.property("caption").as_string());
		txt.tip_string(properties.property("tip_string").as_string());
		txt.editable(properties.property("editable").as_bool());
		txt.line_wrapped(properties.property("line_wrapped").as_bool());
		txt.multi_lines(properties.property("multi_lines").as_bool());
	}


	void textbox::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/textbox.hpp>");
		// declaration
		cd->decl.push_back("nana::textbox " + name + ";");
		// init
		cd->init.push_back(name + ".caption(\"" + properties.property("caption").as_string() + "\");");
		cd->init.push_back(name + ".tip_string(\"" + properties.property("tip_string").as_string() + "\");");
		cd->init.push_back(name + ".editable(" + properties.property("editable").as_string() + ");");
		cd->init.push_back(name + ".line_wrapped(" + properties.property("line_wrapped").as_string() + ");");
		cd->init.push_back(name + ".multi_lines(" + properties.property("multi_lines").as_string() + ");");
	}

}//end namespace ctrls
