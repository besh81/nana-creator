/*
 *		ctrls::checkbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include "config.h"
#include <iostream>
#include "ctrls/checkbox.h"



namespace ctrls
{

	//checkbox
	checkbox::checkbox(nana::window wd, const std::string& name)
		: ctrl()
	{
		chk.create(wd);
		ctrl::init(&chk, CTRL_CHECKBOX, name);

		// common
		properties.append("caption").label("Caption").category(CAT_COMMON).type(pg_type::string) = "";
		properties.append("check").label("Checked").category(CAT_COMMON).type(pg_type::check) = chk.checked();
		properties.append("group").label("Group name").category(CAT_COMMON).type(pg_type::string) = "";
		// appearance
		properties.append("radio").label("Radio").category(CAT_APPEARANCE).type(pg_type::check) = false;
		// layout
		// ...
	}


	void checkbox::update()
	{
		ctrl::update();

		chk.caption(properties.property("caption").as_string());
		chk.check(properties.property("check").as_bool());
		chk.radio(properties.property("radio").as_bool());

		chk.react(false); // needs to avoid user can change the state clicking on control
	}


	void checkbox::generatecode(code_data_struct* cd, code_info_struct* ci)
	{
		ctrl::generatecode(cd, ci);

		std::string name = properties.property("name").as_string();

		// headers
		cd->hpps.add("#include <nana/gui/widgets/checkbox.hpp>");
		// declaration
		cd->decl.push_back("nana::checkbox " + name + ";");
		// init
		cd->init.push_back(name + ".caption(\"" + properties.property("caption").as_string() + "\");");
		cd->init.push_back(name + ".check(" + properties.property("check").as_string() + ");");
		cd->init.push_back(name + ".radio(" + properties.property("radio").as_string() + ");");

		if(!properties.property("group").as_string().empty())
		{
			//TODO: il nome del gruppo non dovrebbe essere presente tra i vari controlli
			std::string group = "rg_" + properties.property("group").as_string();

			// check if a group with the same name already present
			bool first_time = true;
			std::string group_decl("nana::radio_group " + group + ";");
			for(auto i : cd->decl)
				if(i == group_decl)
				{
					first_time = false;
					break;
				}
			if(first_time)
				cd->decl.push_back(group_decl);

			cd->init.push_back(group + ".add(" + name + ");");
		}
	}

}//end namespace ctrls
