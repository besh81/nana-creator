/*
 *		ctrls::toolbar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/toolbar.h"


namespace ctrls
{

	//toolbar
	toolbar::toolbar(nana::window wd, const std::string& name)
		: ctrl()
	{
		tlb.create(wd);
		ctrl::init(&tlb, CTRL_TOOLBAR, name);

		// common
		// ...
		// appearance
		// ...
		// layout
		properties.append("scale").label("Scale").category(CAT_APPEARANCE).type(pg_type::string_uint) = 16; // get from nana source
	}


	void toolbar::update()
	{
		ctrl::update();

		tlb.scale(properties.property("scale").as_uint());
	}


	void toolbar::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/toolbar.hpp>");
		// declaration
		cd->decl.push_back("nana::toolbar " + name + ";");
		// init
		cd->init.push_back(name + ".scale(" + properties.property("scale").as_string() + ");");
	}

}//end namespace ctrls
