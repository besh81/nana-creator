/*
 *		ctrls::progress Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/progress.h"
#include "nana_extra/color_helper.h"


namespace ctrls
{

	//progress
	progress::progress(nana::window wd, const std::string& name)
		: ctrl()
	{
		prg.create(wd);
		ctrl::init(&prg, CTRL_PROGRESS, name);

		// common
		properties.append("value").label("Value").category(CAT_COMMON).type(pg_type::string_uint_0_100).enabled("unknown", false) = prg.value();
		properties.append("unknown").label("Unknown").category(CAT_COMMON).type(pg_type::check) = prg.unknown();
		// appearance
		// ...
		// layout
		// ...
	}


	void progress::update()
	{
		ctrl::update();

		if(properties.property("unknown").as_bool())
			prg.value(0);
		else
			prg.value(properties.property("value").as_uint());
	}


	void progress::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/progress.hpp>");
		// declaration
		cd->decl.push_back("nana::progress " + name + ";");
		// init
		if(properties.property("unknown").as_bool())
			cd->init.push_back(name + ".unknown(" + properties.property("unknown").as_string() + ");");
		else
			cd->init.push_back(name + ".value(" + properties.property("value").as_string() + ");");
	}

}//end namespace ctrls
