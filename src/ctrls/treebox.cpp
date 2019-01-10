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
	treebox::treebox(ctrl* parent, const std::string& name)
		: ctrl(parent)
	{
		trb.create(*parent->nanawdg);
		ctrl::init(&trb, CTRL_TREEBOX, name);

		// common
		properties.append("checkable").label("Checkable").category(CAT_COMMON).type(pg_type::check) = trb.checkable();
		// appearance
		// ...
		// layout
		// ...
	}


	void treebox::update()
	{
		ctrl::update();

		trb.checkable(properties.property("checkable").as_bool());
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
		if(properties.property("checkable").as_bool())
			cd->init.push_back(name + ".checkable(true);");
	}

}//end namespace ctrls
