/*
 *		ctrls::group Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/group.h"


namespace ctrls
{

	//group
	group::group(nana::window wd, const std::string& name)
		: ctrl()
	{
		grp.create(wd);
		ctrl::init(&grp, CTRL_GROUP, name);

		// common
		properties.append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = CTRL_GROUP;
		// appearance
		// ...
		// layout
		// ...
	}


	void group::update()
	{
		ctrl::update();

		grp.caption(properties.property("caption").as_string());
	}


	void group::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/group.hpp>");
		// declaration
		cd->decl.push_back("nana::group " + name + ";");
		// init
		cd->init.push_back(name + ".caption(\"" + properties.property("caption").as_string() + "\");");
	}

}//end namespace ctrls
