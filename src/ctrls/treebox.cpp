/*
 *		ctrls::treebox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include "ctrls/treebox.h"


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
		properties.append("use_entire_line").label("Use entire line").category(CAT_APPEARANCE).type(pg_type::check) = false;
		// layout
		// ...
	}


	void treebox::update()
	{
		ctrl::update();

		trb.checkable(properties.property("checkable").as_bool());
		trb.use_entire_line(properties.property("use_entire_line").as_bool());
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
		if(properties.property("use_entire_line").as_bool())
			cd->init.push_back(name + ".use_entire_line(true);");
	}

}//end namespace ctrls
