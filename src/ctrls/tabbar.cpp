/*
 *		ctrls::tabbar Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/tabbar.h"


namespace ctrls
{

	//tabbar
	tabbar::tabbar(nana::window wd, const std::string& name)
		: ctrl()
	{
		tbb.create(wd);
		ctrl::init(&tbb, CTRL_TABBAR, name);

		// common
		// ...
		// appearance
		// ...
		// layout
		// ...
	}


	void tabbar::update()
	{
		ctrl::update();
	}


	void tabbar::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/tabbar.hpp>");
		// declaration
		cd->decl.push_back("nana::tabbar " + name + ";");
		// init
		// ...
	}

}//end namespace ctrls
