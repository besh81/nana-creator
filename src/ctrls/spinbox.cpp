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
	spinbox::spinbox(nana::window wd, const std::string& name)
		: ctrl()
	{
		spn.create(wd);
		ctrl::init(&spn, CTRL_SPINBOX, name);

		// common
		properties.append("editable").label("Editable").category(CAT_COMMON).type(pg_type::check) = spn.editable();
		// appearance
		properties.property("bgcolor") = nana::to_string(spn.bgcolor(), false);
		// ...
		// layout
		// ...
	}


	void spinbox::update()
	{
		ctrl::update();

		spn.editable(properties.property("editable").as_bool());
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
		cd->init.push_back(name + ".editable(" + properties.property("editable").as_string() + ");");
	}

}//end namespace ctrls
