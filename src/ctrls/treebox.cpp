/*
 *		ctrls::treebox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/treebox.h"
#include "nana_extra/color_helper.h"


namespace ctrls
{

	//treebox
	treebox::treebox(nana::window wd, const std::string& name)
		: ctrl()
	{
		trb.create(wd);
		ctrl::init(&trb, CTRL_TREEBOX, name);

		// common
		// ...
		// appearance
		properties.property("bgcolor") = nana::to_string(trb.bgcolor(), false);
		// layout
		// ...
	}


	void treebox::update()
	{
		ctrl::update();
	}


	void treebox::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/treebox.hpp>");
		// declaration
		cd->decl.push_back("nana::treebox " + name + ";");
		// init
		// ...
	}

}//end namespace ctrls
