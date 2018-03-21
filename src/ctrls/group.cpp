/*
 *		ctrls::group Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/group.h"
#include "nana_extra/color_helper.h"


namespace ctrls
{

	//group
	group::group(nana::window wd, const std::string& name)
		: ctrl()
	{
		grp.create(wd);
		grp.caption(CTRL_GROUP); //BUG: if removed the group caption is not displayed at the beginning !!!
		ctrl::init(&grp, CTRL_GROUP, name);

		grp.div("abc");
		grp.collocate();
		

		// common
		properties.append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = CTRL_GROUP;
		// appearance
		properties.property("bgcolor") = nana::to_string(grp.bgcolor(), false);
		// layout
		// ...
	}


	void group::update()
	{
		ctrl::update();

		grp.caption(properties.property("caption").as_string());

		grp.collocate();
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


		cd->init.push_back(name + ".div(\"field1\");");
		// children
		if(!ci->place.empty())
			ci->create = name;
		ci->place = name;
	}


	bool group::append(nana::window child)
	{
		if(haschild())
			return false;

		_child = true;

		grp["abc"] << child;
		grp.collocate();
		return true;
	}


	bool group::remove(nana::window child)
	{
		if(!_child)
			return false;

		grp.field_display("abc", false);
		grp.erase(child);
		grp.field_display("abc", true);
		grp.collocate();

		_child = false;
		return true;
	}


}//end namespace ctrls
