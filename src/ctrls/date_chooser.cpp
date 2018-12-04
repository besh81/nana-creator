/*
 *		ctrls::date_chooser Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/date_chooser.h"



namespace ctrls
{

	//date_chooser
	date_chooser::date_chooser(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		dtc.create(*parent->nanawdg);
		ctrl::init(&dtc, CTRL_DATECHOOSER, name);

		// common
		// ...
		// appearance
		// ...
		// layout
		// ...
	}


	void date_chooser::update()
	{
		ctrl::update();
	}


	void date_chooser::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/date_chooser.hpp>");
		// declaration
		cd->decl.push_back("nana::date_chooser " + name + ";");
		// init
		// ...
	}

}//end namespace ctrls
